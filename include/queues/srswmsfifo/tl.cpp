#include <assert.h>
#include <iostream>
#include <pthread.h>
#include "swsrfifo.h"

static SWSRFIFO<int> fifo;
const int COUNT = 100000000;

static void* producer(void*) {
  int count = 0;
  while (count < COUNT) {
    fifo.push_back(count);
    ++count;
  }
  return 0;
}

static void* consumer(void* param) {
  int expected = 0;
  int p = 0;
  while (expected < COUNT) {
    if (fifo.pop_front(p)) {
      ++expected;
    }
  }
  *((int*)param) = expected;
  return 0;
}

static void run_once() {
  pthread_t producer_thread_id;
  int producer_thread_ret = pthread_create(&producer_thread_id,
                                           NULL,
                                           producer,
                                           (void*)NULL);
  int count;
  pthread_t consumer_thread_id;
  int consumer_thread_ret = pthread_create(&consumer_thread_id,
                                           NULL,
                                           consumer,
                                           (void*)&count);

  pthread_join(producer_thread_id, NULL);
  pthread_join(consumer_thread_id, NULL);

  std::cout << "Producer thread returned " << producer_thread_ret << std::endl;
  std::cout << "Consumer thread returned " << consumer_thread_ret << std::endl;
  std::cout << "Count " << count << std::endl;
}

int main(int argc, char** argv) {
  run_once();
  std::cout << "sizeof(int): " << sizeof(int) << std::endl;
  std::cout << "sizeof(void*): " << sizeof(void*) << std::endl;
}

