#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace ab1764 {

template <int S>
struct bitmask {

  static const uint64_t SIZE = S;
  static const uint64_t SHIFTBITS = 6;
  static const uint64_t BITSPERELEMENT = 64;
  static const uint64_t ARRAYSIZE = SIZE >> SHIFTBITS;
  static const uint64_t LASTBIT = (SIZE-1);
  static const uint64_t MSB64 = 0x8000000000000000;

  uint64_t flags_[ARRAYSIZE];

  bitmask() { ::memset(flags_, 0, sizeof(flags_)); }

  static constexpr size_t toIndex(const size_t bit) { return bit >> SHIFTBITS; }
  static constexpr size_t toOffset(const size_t bit) { return bit & ((1 << SHIFTBITS) - 1); }

  bool isSet(size_t bit) const { return flags_[toIndex(bit)] & (MSB64 >> toOffset(bit)); }
  bool isClear(size_t bit) const { return !isSet(bit); }

  void set(const size_t bit) { flags_[toIndex(bit)] |= (MSB64 >> toOffset(bit)); }
  void clear(const size_t bit) { flags_[toIndex(bit)] &= ~(MSB64 >> toOffset(bit)); }

  size_t findFirstClearBit() const {
    for (size_t i = 0; i < ARRAYSIZE; ++i) {
      const size_t flags = flags_[i]; 
      if (flags != ~0) {
        uint64_t bit = MSB64;
        for (size_t j = 0; j < BITSPERELEMENT; ++j) {
          if (!(flags & (bit >>= 1)) return (i << SHIFTBITS) + j;
        }
      }
    }
    return LASTBIT+1; // bit after last
  }
};

}
