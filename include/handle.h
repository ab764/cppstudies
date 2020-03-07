#pragma once

#include <cstddef>

namespace ab764 {

template <typename T, int INDEXBITSIZE>
struct Handle {
private:
  T id_;

public:

  using IDType = T;

  Handle() : id_(0) {}

  void set(T count, T index) { id_ = (count << INDEXBITSIZE) | index; }

  T id() const { return id_; }
  T count() const { return id_ >> INDEXBITSIZE; }
  T index() const { return id_ & (((T)1 << INDEXBITSIZE) - (T)1); }

  operator size_t() { return id_; }

  static constexpr int bitSize() { return sizeof(T) << 3; } // assume byte is 8-bit
  static constexpr int indexBitSize() { return INDEXBITSIZE; }
  static constexpr T indexBitMask() { return ((T)1 << INDEXBITSIZE) - (T)1; }
  static constexpr int countBitSize() { return ((T)sizeof(T) << 3) - INDEXBITSIZE; }

  static constexpr T maxIndex() { return ((T)1 << INDEXBITSIZE) - (T)1; }
  static constexpr T maxCount() { return ((T)1 << countBitSize()) - (T)1; }

  void incrementCount() { id_ += ((T)1 << INDEXBITSIZE); }
};

}
