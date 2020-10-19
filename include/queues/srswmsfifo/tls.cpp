#include <iostream>
#include <thread>
#include "swsrfifo.h"

static SWSRFIFO<int> fifo;
const int COUNT = 100000000;

void producerFn() {
  int count = 0;
  while (count < COUNT) {
    fifo.push_back(count);
    ++count;
  }
}

void consumerFn(int* param) {
  int consumed = 0;
  int p = 0;
  while (consumed < COUNT) {
    if (fifo.pop_front(p)) {
      ++consumed;
      //std::cout << "popped: " << p << std::endl;
    } else {
      std::this_thread::yield();
    }
  }
  *param = consumed;
}

int main(int argc, char** argv) {
  int count = 0;

  std::thread producer(producerFn);
  std::thread consumer(consumerFn, &count);

  producer.join();
  consumer.join();

  std::cout << "Count " << count << std::endl;
  return 0;
}
