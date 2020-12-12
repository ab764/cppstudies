#pragma once

#include <cstdint>

namespace ab764 {

template <typename T, int PAGEARRAYSIZE>
class FixedSizeElementMemoryPool {
private:

  union Node {
    Node*  next_;
    T      data_;
  };

  struct Page {
    Page* nextPage_;
    Node* storage_;
    Node* freeListHead_;

    Page() : nextPage_(nullptr), storage_(nullptr) {
      freeListHead_ = storage_ = (Node*)::malloc(sizeof(Node) * PAGEARRAYSIZE);
      Node* p = storage_;
      for (int i = 0; i < PAGEARRAYSIZE-1; ++i, ++p) {
        p->next_ = (p+1); 
      }
      p->next_ = nullptr;
    }

    ~Page() { ::free(storage_); }

    template <typename... Args> T* alloc(Args &&... args) {
      assert(freeListHead_ != nullptr);
   
      T* p = &freeListHead_->data_;
      freeListHead_ = freeListHead_->next_;

      new (p) T(std::forward<Args>(args)...);
      return p;
    }

    bool free(T* p) {

      Node* pnode = reinterpret_cast<Node*>(p);

      if (pnode < storage_ || pnode > (storage_ + PAGEARRAYSIZE)) return false;

      // destruct the data part
      p->T::~T();
      
      // push node at the top of the free list
      pnode->next_  = freeListHead_;
      freeListHead_ = pnode;
      
      return true;
    }

    bool hasFreeSlot() const { return freeListHead_ != nullptr; }
  };

  Page* pageHead_;

public:

  FixedSizeElementMemoryPool() {
    pageHead_ = new Page();
  }

  ~FixedSizeElementMemoryPool() {
    while (pageHead_ != nullptr) {
      Page* p = pageHead_;
      pageHead_ = pageHead_->nextPage_;
      delete p;
    }
  }

  template <typename... Args> T* alloc(Args &&... args) {
    Page* page = pageHead_;
    while (page!=nullptr && !page->hasFreeSlot()) { page = page->nextPage_; }

    if (page==nullptr) {
      page = new Page();
      page->nextPage_ = pageHead_;
      pageHead_ = page;
    }

    return page->alloc(std::forward<Args>(args)...);
  }

  void free(T* p) {
    Page* page = pageHead_;
    while (page!=nullptr && !page->free(p)) { page = page->nextPage_; }
  }
};

}
