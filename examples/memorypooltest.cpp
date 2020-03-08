#include <iostream>

#include "nomemorypool.h"
#include "fixedsizeelementmemorypool.h"
#include "inelasticfixedsizememorypool.h"
#include "inelasticfixedsizememorypoolvector.h"
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
const int PAGESIZEINBYTES = 512 * 1024;
const int ELEMENTSPERPAGE = PAGESIZEINBYTES / sizeof(S);

// total iterations needed to allocate 64M blocks total
const int ITERATIONS = (64 * 1024 * 1024) / ALLOCCOUNT;

template <typename T>
void memoryPoolTest(T& pool)
{
  int count = 0;
  for (int i = 0; i < ITERATIONS; ++i) {
    for (int j = 0; j < ALLOCCOUNT; ++j) {
      arr[j] = pool.alloc();
    }

    for (int j = 0; j < ALLOCCOUNT; ++j) {
      pool.free(arr[j]);
    }

    count += ALLOCCOUNT;
  }
  std::cout << "Total allocations/free: " << count << std::endl;
}

void testNoMemoryPool()
{
  ab764::NoMemoryPool<S> pool;
  memoryPoolTest<ab764::NoMemoryPool<S>>(pool);
}

void testFixedSizeElementMemoryPool()
{
  using Pool = ab764::FixedSizeElementMemoryPool<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool);
}

void testInelasticFixedSizeMemoryPool()
{
  using Pool = ab764::InelasticFixedSizeMemoryPool<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool);
}

void testInelasticFixedSizeMemoryPoolVector()
{
  using Pool = ab764::InelasticFixedSizeMemoryPoolVector<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool);
}

void testSimpleFixedSizeArrayMemoryPool()
{
  using Pool = ab764::SimpleFixedSizeArrayMemoryPool<S, ALLOCCOUNT>;
  Pool pool;
  memoryPoolTest<Pool>(pool);
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
  case 4: mempooltest::testInelasticFixedSizeMemoryPoolVector(); break;
  case 5: mempooltest::testSimpleFixedSizeArrayMemoryPool(); break;
  }
  std::cout << "finished" << std::endl;

  return 0;
}
