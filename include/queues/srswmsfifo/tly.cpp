#include <iostream>
#include <pthread.h>
#include "swsrfifo.h"

static SWSRFIFO<int> fifo;
const int COUNT = 100000000;

static void* producerFn(void*) {
  int count = 0;
  while (count < COUNT) {
    fifo.push_back(count);
    ++count;
  }
  return 0;
}

static void* consumerFn(void* param) {
  int consumed = 0;
  int p = 0;
  while (consumed < COUNT) {
    if (fifo.pop_front(p)) {
      ++consumed;
      //std::cout << "popped: " << p << std::endl;
    } else {
      sched_yield();
    }
  }
  *((int*)param) = consumed;
  return 0;
}

int main(int argc, char** argv) {
  int count;

  pthread_t producer_thread_id;
  pthread_create(&producer_thread_id, NULL, producerFn, NULL);
  pthread_t consumer_thread_id;
  pthread_create(&consumer_thread_id, NULL, consumerFn, &count);
  
  pthread_join(producer_thread_id, NULL);
  pthread_join(consumer_thread_id, NULL);
  
  std::cout << "Count " << count << std::endl;
}
