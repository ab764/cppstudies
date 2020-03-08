#include <iostream>

#include "nomemorypool.h"
#include "fixedsizeelementmemorypool.h"
#include "inelasticfixedsizememorypool.h"
#include "inelasticfixedsizememorypoolvector.h"
#include "simplefixedsizearraymemorypool.h"

namespace mempooltest {

struct S {
  char buff[64];

  S() { std::cout << __FUNCTION__ << " " << this << std::endl; }
  ~S() { std::cout << __FUNCTION__ << " " << this << std::endl; }
};

const int ELEMENTSPERPAGE = 4;

template <typename T>
void memoryPoolTest(T& pool, const int count)
{
  S* arr[count];

  for (int i=0; i<count; ++i) {
    arr[i] = pool.alloc();
  }

  int allocations = 0;
  for (int i=0; i<count; ++i) {
    if (arr[i]) {
      pool.free(arr[i]);
      ++allocations;
    }
  }

  std::cout << "Total allocations/free: " << allocations << std::endl;
}

void testNoMemoryPool(const int count)
{
  ab764::NoMemoryPool<S> pool;
  memoryPoolTest<ab764::NoMemoryPool<S>>(pool, count);
}

void testFixedSizeElementMemoryPool(const int count)
{
  using Pool = ab764::FixedSizeElementMemoryPool<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool, count);
}

void testInelasticFixedSizeMemoryPool(const int count)
{
  using Pool = ab764::InelasticFixedSizeMemoryPool<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool, count);
}

void testInelasticFixedSizeMemoryPoolVector(const int count)
{
  using Pool = ab764::InelasticFixedSizeMemoryPoolVector<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool, count);
}

void testSimpleFixedSizeArrayMemoryPool(const int count)
{
  using Pool = ab764::SimpleFixedSizeArrayMemoryPool<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool, count);
}

}

int main(const int argc, const char **argv)
{
  std::cout << "start" << std::endl;

  const int count = ::atoi(argv[2]);
  const int which = ::atoi(argv[1]);
  switch (which) {
  case 1: mempooltest::testNoMemoryPool(count); break;
  case 2: mempooltest::testFixedSizeElementMemoryPool(count); break;
  case 3: mempooltest::testInelasticFixedSizeMemoryPool(count); break;
  case 4: mempooltest::testInelasticFixedSizeMemoryPoolVector(count); break;
  case 5: mempooltest::testSimpleFixedSizeArrayMemoryPool(count); break;
  }
  std::cout << "finished" << std::endl;

  return 0;
}
