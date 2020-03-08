#pragma once

#include <cstdint>

namespace ab764 {

template <typename T, int PAGEARRAYSIZE>
class InelasticFixedSizeMemoryPoolVector {
private:

  union Node {
    Node*  next_;
    T      data_;
  };

  struct Page {
    Node* storage_;

    Page() : storage_((Node*)::malloc(sizeof(Node) * PAGEARRAYSIZE)) {
      Node* p = storage_;
      for (int i = 0; i < PAGEARRAYSIZE-1; ++i, ++p) {
        p->next_ = (p+1);
      }
      p->next_ = nullptr;
    }

    Page(Page&& other) : storage_(other.storage_) { other.storage_ = nullptr; }

    ~Page() { if (storage_) ::free(storage_); }

    Page& operator=(const Page& other) = delete;

    Node* getStorage() const { return storage_; }
  };

  Node* freeListHead_;

  std::vector<Page> pages_;

public:

  InelasticFixedSizeMemoryPoolVector()
   : freeListHead_(nullptr) {
    pages_.reserve((1024 * 2048) / (PAGEARRAYSIZE * sizeof(T)));
    freeListHead_ = pages_.emplace_back().getStorage();
  }

  template <typename... Args> T* alloc(Args &&... args) {
    if (freeListHead_ == nullptr) {
      freeListHead_ = pages_.emplace_back().getStorage();
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
