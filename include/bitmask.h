#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace ab1764 {

template <int S>
struct mybitset {

  static const uint64_t SIZE = S;
  static const uint64_t SHIFTBITS = 6;
  static const uint64_t BITSPERELEMENT = 64;
  static const uint64_t ARRAYSIZE = ((SIZE+15) >> SHIFTBITS);
  static const uint64_t LASTBIT = (SIZE-1);
  static const uint64_t MSB64 = 0x8000000000000000;

  uint64_t flags_[ARRAYSIZE];

  mybitset() { clearAll(); }

  static constexpr size_t toIndex(const size_t bit) { return bit >> SHIFTBITS; }
  static constexpr size_t toOffset(const size_t bit) { return bit & ((1 << SHIFTBITS) - 1); }

  bool isSet(size_t bit) const { return flags_[toIndex(bit)] & (MSB64 >> toOffset(bit)); }
  bool isClear(size_t bit) const { return !isSet(bit); }

  void set(const size_t bit) { flags_[toIndex(bit)] |= (MSB64 >> toOffset(bit)); }
  void setAll() { ::memset(flags_, 0xff, sizeof(flags_)); }
  void clear(const size_t bit) { flags_[toIndex(bit)] &= ~(MSB64 >> toOffset(bit)); }
  void clearAll() { ::memset(flags_, 0, sizeof(flags_)); }

  size_t findFirstClearBit() const {
    const auto end = flags_ + ARRAYSIZE;
    for (const uint64_t* flags = flags_; flags < end; ++flags) {
      if (*flags != ~0) {
        uint64_t bit = MSB64;
        for (size_t j = 0; j < BITSPERELEMENT; ++j) {
          if (!(*flags & bit)) return ((flags - flags_) << SHIFTBITS) + j;
          bit >>= 1;
        }
      }
    }
    return LASTBIT+1; // bit after last
  }
};

}
