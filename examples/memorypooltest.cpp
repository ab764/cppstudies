#include <iostream>

#include "nomemorypool.h"
#include "fixedsizeelementmemorypool.h"
#include "inelasticfixedsizememorypool.h"
#include "simplefixedsizearraymemorypool.h"

namespace mempooltest {

struct S {
  char buff[64];
};

const int ALLOCCOUNT = 64 * 1024;
S* arr[ALLOCCOUNT];
const int PAGESIZEINKB = 64 * 1024;
const int ELEMENTSPERPAGE = PAGESIZEINKB / sizeof(S);

template <typename T>
void memoryPoolTest(T& pool)
{
  int count = 0;
  for (int i = 0; i < 1024; ++i) {
    for (int i=0; i<ALLOCCOUNT; ++i) {
      arr[i] = pool.alloc();
    }

    for (int i=0; i<ALLOCCOUNT; ++i) {
      pool.free(arr[i]);
    }

    count += ALLOCCOUNT;
  }
  std::cout << "Totall allocations/free: " << count << std::endl;
}

void testNoMemoryPool()
{
  ab764::NoMemoryPool<S> pool;
  memoryPoolTest<ab764::NoMemoryPool<S>>(pool);
}

void testFixedSizeElementMemoryPool()
{
  ab764::FixedSizeElementMemoryPool<S, ELEMENTSPERPAGE> pool;
  memoryPoolTest<ab764::FixedSizeElementMemoryPool<S, ELEMENTSPERPAGE>>(pool);
}

void testInelasticFixedSizeMemoryPool()
{
  ab764::InelasticFixedSizeMemoryPool<S, ELEMENTSPERPAGE> pool;
  memoryPoolTest<ab764::InelasticFixedSizeMemoryPool<S, ELEMENTSPERPAGE>>(pool);
}

void testSimpleFixedSizeArrayMemoryPool()
{
  ab764::SimpleFixedSizeArrayMemoryPool<S, ALLOCCOUNT> pool;
  memoryPoolTest<ab764::SimpleFixedSizeArrayMemoryPool<S, ALLOCCOUNT>>(pool);
}

}

int main(const int argc, const char **argv)
{
  std::cout << "start" << std::endl;

  const int which = ::atoi(argv[1]);
  switch (which) {
  case 1: mempooltest::testNoMemoryPool(); break;
  case 2: mempooltest::testFixedSizeElementMemoryPool(); break;
  case 3: mempooltest::testInelasticFixedSizeMemoryPool(); break;
  case 4: mempooltest::testSimpleFixedSizeArrayMemoryPool(); break;
  }
  std::cout << "finished" << std::endl;

  return 0;
}
