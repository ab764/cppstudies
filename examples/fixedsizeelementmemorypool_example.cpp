#include <iostream>

#include "fixedsizeelementmemorypool.h"

int main(const int argc, const char **argv)
{
  struct S {
    int i;

    S() { std::cout << __FUNCTION__ << " " << this << std::endl;}
    ~S() { std::cout << __FUNCTION__ << " " << this << std::endl;}
  };

  ab764::FixedSizeElementMemoryPool<S, 5> pool;

  S* s = pool.alloc();

  s->i = 1;

  pool.free(s);

  const int size = ::atoi(argv[1]);
  S* arr[size];
  for (int i=0; i<size; ++i) {
    arr[i] = pool.alloc();
  }

  for (int i=0; i<size; ++i) {
    pool.free(arr[i]);
  }

  std::cout << "finished" << std::endl;

  return 0;
}
