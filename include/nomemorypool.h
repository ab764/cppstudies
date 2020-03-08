#pragma once

#include <cstdint>

namespace ab764 {

template <typename T>
class NoMemoryPool {
public:
  template <typename... Args> T* alloc(Args &&... args) {
    return new T(std::forward<Args>(args)...);
  }

  void free(T* p) {
    delete p;
  }
};

}
