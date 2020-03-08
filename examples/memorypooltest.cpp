#include <iostream>

#include "nomemorypool.h"
#include "fixedsizeelementmemorypool.h"
#include "inelasticfixedsizememorypool.h"
#include "simplefixedsizearraymemorypool.h"

namespace mempooltest {

struct S {
  char buff[64];
};

// limit the total memory block size using this number
const int TOTALDATASETINBYTES = (2048*1024);

// allocate max of ALLOCCOUNT blocks per iteration
const int ALLOCCOUNT = (TOTALDATASETINBYTES/sizeof(S));
static S* arr[ALLOCCOUNT];

// 64k pages except SimpleFixedSizeArrayMemoryPool which cannot expand
const int PAGESIZEINBYTES = 64 * 1024;
const int ELEMENTSPERPAGE = PAGESIZEINBYTES / sizeof(S);

// total iterations needed to allocate 64M blocks total
const int ITERATIONS = (64 * 1024 * 1024) / ALLOCCOUNT;

template <typename T>
void memoryPoolTest(T& pool)
{
  int count = 0;
  for (int i = 0; i < ITERATIONS; ++i) {
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
