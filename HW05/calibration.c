// Copyright [2019] <Yidong Fang>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define TEST_NUM 1000
#define CACHE_SIZE 4 * 1024

/* Performs a dummy access
   to the address specified. */
void maccess(void *addr) {
  asm volatile("movq (%0), %%rax\n" : : "c"(addr) : "rax");
}

/* Flushes the cache block corresponding
   to the address from the entire cache
   hierarchy. */
void flush(void *addr) {
  asm volatile("clflush 0(%0)\n" : : "c"(addr) : "rax");
}

/* Reads the Time Stamp Counter. */
uint64_t rdtsc() {
  uint64_t a, d;
  asm volatile("mfence");
  asm volatile("rdtsc" : "=a"(a), "=d"(d));
  a = (d << 32) | a;
  asm volatile("mfence");
  return a;
}

/* combines rdtsc and maccess */
uint64_t measure_one_block_access_time(void *addr) {
  uint64_t cycles;

  asm volatile(
      "mov %1, %%r8\n\t"
      "lfence\n\t"
      "rdtsc\n\t"
      "mov %%eax, %%edi\n\t"
      "mov (%%r8), %%r8\n\t"
      "lfence\n\t"
      "rdtsc\n\t"
      "sub %%edi, %%eax\n\t"
      : "=a"(cycles) /*output*/
      : "r"(addr)
      : "r8", "edi");

  return cycles;
}

int main(int argc, char **argv) {
  char array[CACHE_SIZE];  // modern processors have 4KB ways in L1
  memset(array, -1, CACHE_SIZE * sizeof(char));
  uint64_t hit_time = 0, miss_time = 0;
  int i;

  /* Calculate average hit time for a cache block */
  measure_one_block_access_time(array);
  for (int j = 0; j < CACHE_SIZE; j++) {
    for (i = 0; i < TEST_NUM; i++) {
      hit_time += measure_one_block_access_time(array + j);
    }
  }
  hit_time /= TEST_NUM;
  hit_time /= CACHE_SIZE;

  /* Calculate average miss time for a cache block */
  for (int j = 0; j < CACHE_SIZE; j++) {
    for (i = 0; i < TEST_NUM; i++) {
      flush(array + j);
      miss_time += measure_one_block_access_time(array + j);
    }
  }
  miss_time /= TEST_NUM;
  miss_time /= CACHE_SIZE;

  printf("%lu,%lu\n", hit_time, miss_time);
  return 0;
}
