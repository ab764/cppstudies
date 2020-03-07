#pragma once

#include <cstdint>

namespace ab764 {

template <typename TID, typename TC, typename TI>
union Handle {

  using IDType = TID;
  using CountType = TC;
  using IndexType = TI;

  IDType id_;
  
  struct {
    IndexType index_;
    CountType count_;
  };

  void set(CountType count, IndexType index)
  {
    count_ = count;
    index_ = index;
  }

  IDType id() const { return id_; }
  CountType count() const { return count_; }
  IndexType index() const { return index_; }

  operator std::size_t() { return id_; }

  void incrementCount() { ++count_; }
};

}
