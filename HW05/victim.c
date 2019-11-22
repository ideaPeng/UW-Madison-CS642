#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/* Lengths of each row in the CSV file. */
int csv_offsets[] = {
    126, 88, 98, 101, 89,  100, 93,  84, 98,  88,  110, 91, 101, 92,  91,
    90,  88, 98, 87,  94,  88,  96,  95, 97,  90,  92,  97, 88,  101, 96,
    90,  90, 89, 93,  92,  99,  95,  89, 82,  95,  95,  88, 96,  96,  95,
    102, 95, 99, 96,  95,  89,  103, 90, 97,  102, 88,  87, 89,  90,  90,
    97,  88, 86, 96,  92,  91,  105, 89, 95,  101, 97,  96, 100, 93,  91,
    88,  85, 90, 101, 107, 91,  90,  91, 111, 100};

/* Return the size of a file */
long file_size(const char *filename) {
  struct stat s;
  if (stat(filename, &s) != 0) {
    printf("Error reading file stats !\n");
    return 1;
  }
  return s.st_size;
}

/* Function to access to perform a dummy access
   to the address specified. */
void maccess(void *addr) {
  asm volatile("movq (%0), %%rax\n" : : "c"(addr) : "rax");
}

/* This program takes the indices of employees
   recieving a promotion as inputs. It then accesses
   the CSV to obtain the details of the
   corresponding employees. */
int main(int argc, char *argv[]) {
  char *filename = "Records.csv";
  int fd = open(filename, O_RDONLY);
  int num_promotions;
  int i, j;
  int offset = 0;

  if (argc < 2) {
    printf("Usage: victim num_promotions indices...\n");
    return 1;
  }

  num_promotions = atoi(argv[1]);

  if (num_promotions < 0 || argc < num_promotions + 2) {
    printf("Incorrect number of promotions!\n");
    return 1;
  }

  int promotion_indices[num_promotions];

  for (i = 0; i < num_promotions; i++) {
    promotion_indices[i] = atoi(argv[i + 2]);
    /* Index starts from. First line has the column headings */
    if (promotion_indices[i] < 1 || promotion_indices[i] > 84) {
      printf("Error: Invalid index.\n");
      return 1;
    }
  }

  unsigned char *addr = (unsigned char *)mmap(0, file_size(filename), PROT_READ,
                                              MAP_SHARED, fd, 0);
  if (addr == (void *)-1 || addr == (void *)0) {
    printf("error: failed to mmap\n");
    return 1;
  }

  /* Access entries corresponding to the input indices. */
  for (i = 0; i < num_promotions; i++) {
    offset = 0;
    /* Loop to find the true address of the input index. */
    for (j = 0; j < promotion_indices[i]; j++)
      offset += csv_offsets[j];

    /* Debug print just to help you figure out which line
       will be accessed. */
    printf("\nPromotion %d:\n", i + 1);
    for (j = 0; j < csv_offsets[promotion_indices[i]]; j++)
      printf("%c", *(addr + offset + j));

    /* Imitate access to the entry in the CSV file.
       Also makes the attack easier. ;) */
    for (j = 0; j < 5000; j++) {
      maccess(addr + offset);
      usleep(50);
    }
    sleep(1);
  }

  return 0;
}
