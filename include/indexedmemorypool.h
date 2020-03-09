#pragma once

#include <cstdlib>
#include <cstdint>
#include "handle.h"

namespace ab764 {

template <typename H, typename P>
class IndexedMemoryPool {
public:
  
  using HandleType = H;
  using PayloadType = P;

  struct Node {
    HandleType handle_;
    union {
      Node* next_;
      P     data_;
    };
  };

  IndexedMemoryPool() : freeListHead_(nullptr), storage_(nullptr) {

    storage_ = (Node*)::malloc(sizeof(H::indexSize() * sizeof(Node)));

    Node* p = storage_;
    freeListHead_ = p;
    for (typename HandleType::IDType i = 0; i < H::indexSize()-1; ++i, ++p) {
      p->handle_.set(0, i);
      p->next_ = (p+1); 
    }
    p->next_ = nullptr;

  }

  ~IndexedMemoryPool() { if(storage_) ::free(storage_); }

  template <typename... Args> Node* alloc(Args &&... args) {
    if (freeListHead_ == nullptr) return nullptr;
    Node* p = freeListHead_;
    freeListHead_ = freeListHead_->next_;

    new (&p->data_) P(std::forward<Args>(args)...);
    return p;
  }

  Node* find(const HandleType handle) {
    Node* p = storage_ + handle.index();
    return (p->handle_ == handle) ? p : nullptr;
  }

  void free(const HandleType handle) {
    Node* p = storage_ + handle.index();

    assert(p->handle_ == handle);

    p->data_.P::~P();

    p->handle_.incrementCount();

    p->next_ = freeListHead_;
    freeListHead_ = p;
  }

private:

  Node*  freeListHead_;
  Node*  storage_;

};

}

