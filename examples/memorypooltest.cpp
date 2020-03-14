#include <iostream>

#include "nomemorypool.h"
#include "fixedsizeelementmemorypool.h"
#include "indexedmemorypool.h"
#include "indexedmemorypoolwithbitset.h"
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
const int PAGESIZEINBYTES = 64 * 1024;
const int ELEMENTSPERPAGE = PAGESIZEINBYTES / sizeof(S);

// total iterations needed to allocate 64M blocks total
const int ITERATIONS = (64 * 1024 * 1024) / ALLOCCOUNT;

template <typename T>
void memoryPoolTest(T& pool)
{
  std::cout << "ITERATIONS: " << ITERATIONS << std::endl;
  std::cout << "ALLOCCOUNT: " << ALLOCCOUNT << std::endl;
  int count = 0;
  for (int i = 0; i < ITERATIONS; ++i) {
    for (int j = 0; j < ALLOCCOUNT; ++j) {
      arr[j] = pool.alloc();
      ++count;
    }

    for (int j = 0; j < ALLOCCOUNT; ++j) {
      pool.free(arr[j]);
    }

  }
  std::cout << "Total allocations/free: " << count << std::endl;
}

void testNoMemoryPool()
{
  std::cout << __FUNCTION__ << " start" << std::endl;
  ab764::NoMemoryPool<S> pool;
  memoryPoolTest<ab764::NoMemoryPool<S>>(pool);
}

void testFixedSizeElementMemoryPool()
{
  std::cout << __FUNCTION__ << " start" << std::endl;
  using Pool = ab764::FixedSizeElementMemoryPool<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool);
}

void testInelasticFixedSizeMemoryPool()
{
  std::cout << __FUNCTION__ << " start" << std::endl;
  using Pool = ab764::InelasticFixedSizeMemoryPool<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool);
}

void testInelasticFixedSizeMemoryPoolVector()
{
  std::cout << __FUNCTION__ << " start" << std::endl;
  using Pool = ab764::InelasticFixedSizeMemoryPoolVector<S, ELEMENTSPERPAGE>;
  Pool pool;
  memoryPoolTest<Pool>(pool);
}

void testSimpleFixedSizeArrayMemoryPool()
{
  std::cout << __FUNCTION__ << " start" << std::endl;
  using Pool = ab764::SimpleFixedSizeArrayMemoryPool<S, ALLOCCOUNT>;
  Pool pool;
  memoryPoolTest<Pool>(pool);
}

template <typename T>
void testIndexedMemoryPool(T& pool)
{
  std::cout << "pool.SIZE: " << pool.SIZE << std::endl;
  std::cout << "ITERATIONS: " << ITERATIONS << std::endl;
  std::cout << "ALLOCCOUNT: " << ALLOCCOUNT << std::endl;

  typename T::HandleType handles[ALLOCCOUNT];
  int count = 0;
  for (int i = 0; i < ITERATIONS; ++i) {
    for (int j = 0; j < pool.SIZE; ++j) {
      handles[j] = pool.alloc()->handle_;
      ++count;
    }

    for (int j = 0; j < pool.SIZE; ++j) {
      pool.free(handles[j]);
    }

  }
  std::cout << "Total allocations/free: " << count << std::endl;
}

void testIndexedMemoryPoolFreeList()
{
  std::cout << __FUNCTION__ << " start" << std::endl;
  using Handle = ab764::Handle<uint32_t, 17>; // need 17 bits for 0..128k
  using Pool = ab764::IndexedMemoryPool<Handle, S, ALLOCCOUNT>;
  Pool pool;
  testIndexedMemoryPool<Pool>(pool);
}

void testIndexedMemoryPoolBitset()
{
  std::cout << __FUNCTION__ << " start" << std::endl;
  using Handle = ab764::Handle<uint32_t, 17>; // need 17 bits for 0..128k
  using Pool = ab764::IndexedMemoryPoolWithBitset<Handle, S, ALLOCCOUNT>;
  Pool pool;
  testIndexedMemoryPool<Pool>(pool);
}

}

int main(const int argc, const char **argv)
{
  std::cout << "start" << std::endl;

  const int which = ::atoi(argv[1]);
  switch (which) {
  case 1: mempooltest::testNoMemoryPool(); break;
  case 2: mempooltest::testFixedSizeElementMemoryPool(); break;
  case 3: mempooltest::testIndexedMemoryPoolFreeList(); break;
  case 4: mempooltest::testIndexedMemoryPoolBitset(); break;
  case 5: mempooltest::testInelasticFixedSizeMemoryPool(); break;
  case 6: mempooltest::testInelasticFixedSizeMemoryPoolVector(); break;
  case 7: mempooltest::testSimpleFixedSizeArrayMemoryPool(); break;
  }
  std::cout << "finished" << std::endl;

  return 0;
}
