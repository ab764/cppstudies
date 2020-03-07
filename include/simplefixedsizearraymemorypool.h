#pragma once

#include <cstdint>

namespace ab764 {

template <typename T, int SIZE>
class FixedSizeArrayMemoryPool {
private:

  union Node {
    Node*  next_;
    T      data_;
  };

  Node*          freeListHead_;
  unsigned char  storage_[SIZE * sizeof(T)];

public:
  FixedSizeArrayMemoryPool() : freeListHead_(nullptr) {

    Node* p = reinterpret_cast<Node*>(storage_);
    freeListHead_ = p;
    for (int i = 0; i < SIZE-1; ++i, ++p) {
      p->next_ = (p+1); 
    }
    p->next_ = nullptr;

  }

  ~FixedSizeArrayMemoryPool() {}

  template <typename... Args> T* alloc(Args &&... args) {
    if (freeListHead_ == nullptr) return nullptr;
    T* p = reinterpret_cast<T*>(freeListHead_);
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
