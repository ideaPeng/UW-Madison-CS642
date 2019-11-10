#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_fail(char *name)
{
  char buf[20];
  strcpy(buf, name);
  
  /* 
   * Assume theres some useful
   * function here to check if 
   * a student has failed.
   */
  return 1;
}

int main(int argc, char *argv[])
{
  char grade;

  if(argc != 2) {
      fprintf(stderr, "target0: argc != 2\n");
      exit(EXIT_FAILURE);
  }
  
  if(check_fail(argv[1]))
    grade = 'F';
  else
    grade = 'A';

  fprintf(stdout, "Grade = %c\n", grade);

  return 0;
}
