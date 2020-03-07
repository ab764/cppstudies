#pragma once

#include <cstdint>
#include <iostream>

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

    std::cout << "sizeof(Node): " << sizeof(Node) << std::endl;

    Node* p = reinterpret_cast<Node*>(storage_);
    freeListHead_ = p;
    std::cout << "freeListHead_: " << freeListHead_ << std::endl;
    for (int i = 0; i < SIZE-1; ++i, ++p) {
      std::cout << "setting, p->next_ = p+1: " << p << std::endl;
      p->next_ = (p+1); 
    }
    std::cout << "finally, p->next_ = nullptr: " << p << std::endl;
    p->next_ = nullptr;

  }

  ~FixedSizeArrayMemoryPool() {}

  T* alloc() {
    if (freeListHead_ == nullptr) return nullptr;
    T* p = reinterpret_cast<T*>(freeListHead_);
    freeListHead_ = freeListHead_->next_;
    std::cout << "alloc(): " << p << std::endl;
    return p;
  }

  void free(T* p) {
    if (p!=nullptr) {
      // p->~T();
      Node* pnode = reinterpret_cast<Node*>(p);
      pnode->next_ = freeListHead_;
      freeListHead_ = pnode;
      std::cout << "free() freeListHead_: " << freeListHead_ << std::endl;
    }
  }
};

}
