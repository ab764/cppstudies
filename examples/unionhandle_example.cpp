#include "unionhandle.h"

int main(const int argc, char **argv)
{
  using ID = ab764::Handle<uint16_t, uint8_t, uint8_t>;

  ID id;
  id.set(12, 10);

  return id.count();
}
