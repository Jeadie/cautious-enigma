/*
 * dummy exploit program
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/usr/local/bin/backup"
#define TESTING_PASSWORD "xf35+"

int main(void)
{
  char *args[5];
  char *env[1];
  
  // another way
  args[0] = TARGET;
  args[1] = "backup"; 
  args[2] = "file.txt";
  args[3] = TESTING_PASSWORD;
  args[4] = NULL;
	
  env[0] = NULL;
  // execve() executes the target program by overwriting the
  // memory of the process in which execve() is executing, i.e., 
  // execve() should never return
  if (execve(TARGET, args, env) < 0)
    fprintf(stderr, "execve failed.\n");

  exit(0);
}
