#pragma once

#include <cstdint>

namespace ab764 {

template <typename T, int ARRAYSIZE>
class SimpleFixedSizeArrayMemoryPool {
private:

  union Node {
    Node*  next_;
    T      data_;
  };

  Node*  freeListHead_;
  Node*  storage_;

public:
  SimpleFixedSizeArrayMemoryPool() : freeListHead_(nullptr), storage_(nullptr) {

    storage_ = (Node*)::malloc(sizeof(Node) * ARRAYSIZE);
    Node* p = storage_;
    freeListHead_ = p;
    for (int i = 0; i < ARRAYSIZE-1; ++i, ++p) {
      p->next_ = (p+1); 
    }
    p->next_ = nullptr;

  }

  ~SimpleFixedSizeArrayMemoryPool() { ::free(storage_); }

  template <typename... Args> T* alloc(Args &&... args) {
    if (freeListHead_ == nullptr) return nullptr;
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
