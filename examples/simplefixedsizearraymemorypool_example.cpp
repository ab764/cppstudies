#include <iostream>

#include "simplefixedsizearraypool.h"

int main(const int argc, const char **argv)
{
  struct S {
    int i;

    S() { std::cout << __FUNCTION__ << std::endl;}
    ~S() { std::cout << __FUNCTION__ << std::endl;}
  };

  {
  ab764::FixedSizeArrayMemoryPool<S, 10> pool;

  S* s = pool.alloc();

  s->i = 1;

  pool.free(s);
  }

  {
  ab764::FixedSizeArrayMemoryPool<S, 10> pool;

  for (int i=0; i<11; ++i) {
    S* s = pool.alloc();
    pool.free(s);
  }
  }

  std::cout << "finished" << std::endl;

  return 0;
}
