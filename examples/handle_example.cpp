#include <iostream>

#include "handle.h"

using namespace std;

template <typename T, int INDEXBITSIZE>
void printHandle(const char* label, const ab764::Handle<T, INDEXBITSIZE> h)
{
  cout << label
       << "[id=" << hex << (size_t)h.id() << dec
       << ":count=" << (size_t)h.count()
       << ":index=" << (size_t)h.index()
       << "]" << endl;
}

template <typename T, int INDEXBITSIZE>
void show_details(const string& text)
{
  using ID = ab764::Handle<T, INDEXBITSIZE>;

  static_assert(sizeof(ID)==sizeof(typename ID::IDType), "Size mismatch");

  cout << text << endl;

  cout << "sizeof(ID): " << sizeof(ID) << "-byte"<< endl;
  cout << "bitSize(): " << (size_t)ID::bitSize() << "-bit" << endl;
  cout << "indexBitSize(): " << (size_t)ID::indexBitSize() << "-bit" << endl;
  cout << "indexBitMask(): " << hex << (size_t)ID::indexBitMask() << dec << endl;
  cout << "maxIndex(): " << (size_t)ID::maxIndex() << endl;
  cout << "countBitSize(): " << (size_t)ID::countBitSize() << "-bit" << endl;
  cout << "maxCount(): " << (size_t)ID::maxCount() << endl;

  ID id;

  printHandle("Initial value: ", id);

  id.set(1, 3);
  printHandle("After set(count=1, index=3): ", id);

  id.incrementCount();
  printHandle("After incrementCount(): ", id);

  id.set(ID::maxCount(), 2);
  printHandle("After set(count=ID::maxCount(), index=2): ", id);

  id.incrementCount();
  printHandle("After incrementCount(): ", id);

  ID id2 = id;
  printHandle("ID id2 = id; d2: ", id2);
}

int main(const int argc, char **argv)
{
  show_details<uint8_t, 3>("\nShowing details for 8-bit handle with 3-bit index");
  show_details<uint16_t, 8>("\nShowing details for 16-bit handle, with 8-bit index");
  show_details<uint32_t, 15>("\nShowing 32-bit handle with 15-bit index");
  show_details<uint32_t, 20>("\nShowing 32-bit handle with 20-bit index");
  show_details<uint32_t, 16>("\nShowing 32-bit handle with 16-bit index");
  show_details<uint64_t, 15>("\nShowing 64-bit handle with 15-bit index");
  show_details<uint64_t, 17>("\nShowing 64-bit handle with 17-bit index");
  show_details<uint64_t, 31>("\nShowing 64-bit handle with 31-bit index");
  show_details<uint64_t, 32>("\nShowing 64-bit handle with 32-bit index");
  show_details<uint64_t, 20>("\nShowing 64-bit handle with 20-bit index");
  return 0;
}
