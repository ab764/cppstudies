#pragma once

#include <cstdint>

namespace ab764 {

template <typename T, int PAGESIZE>
class InelasticFixedSizeMemoryPool {
private:

  union Node {
    Node*  next_;
    T      data_;
  };

  struct Page {
    Page* next_;
    Node* storage_;

    Page() : next_(nullptr), storage_(nullptr) {
      storage_ = (Node*)::malloc(sizeof(T) * PAGESIZE);
      Node* p = storage_;
      for (int i = 0; i < PAGESIZE-1; ++i, ++p) {
        p->next_ = (p+1); 
      }
      p->next_ = nullptr;
    }

    ~Page() { ::free(storage_); }

    Node* getStorage() const { return storage_; }
    void setNext(Page* next) { next_ = next; ; }
    Page* getNext() const { return next_; }
  };

  Node* freeListHead_;
  Page* pageHead_;

public:

  InelasticFixedSizeMemoryPool()
   : freeListHead_(nullptr), pageHead_(nullptr) {
    pageHead_ = new Page();
    freeListHead_ = pageHead_->getStorage();
  }

  ~InelasticFixedSizeMemoryPool() {
    while (pageHead_) {
      Page* p = pageHead_;
      pageHead_ = pageHead_->getNext();
      delete p;
    }
  }

  template <typename... Args> T* alloc(Args &&... args) {
    if (freeListHead_ == nullptr) {
      Page* newpage = new Page();
      newpage->setNext(pageHead_);
      freeListHead_ = newpage->getStorage();
      pageHead_ = newpage;
    }
    T* p = &freeListHead_->data_;
    freeListHead_ = freeListHead_->next_;

    new (p) T(std::forward<Args>(args)...);
    return p;
  }

  void free(T* p) {
    p->T::~T();
    Node* pnode = reinterpret_cast<Node*>(p);
    pnode->next_ = freeListHead_;
    freeListHead_ = pnode;
  }
};

}
