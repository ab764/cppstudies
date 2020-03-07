#include <iostream>

#include "inelasticfixedsizememorypool.h"

int main(const int argc, const char **argv)
{
  struct S {
    int i;

    S() { std::cout << __FUNCTION__ << " " << this << std::endl;}
    ~S() { std::cout << __FUNCTION__ << " " << this << std::endl;}
  };

  {
  ab764::InelasticFixedSizeMemoryPool<S, 10> pool;

  S* s = pool.alloc();

  s->i = 1;

  pool.free(s);
  }

  {
  ab764::InelasticFixedSizeMemoryPool<S, 5> pool;

  for (int i=0; i<11; ++i) {
    S* s = pool.alloc();
  }
  }

  std::cout << "finished" << std::endl;

  return 0;
}
