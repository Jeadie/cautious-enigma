/*
 * A very simple backup/restore program - use at your own risk ;)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CMD_BACKUP 0
#define CMD_RESTORE 1

#define BACKUP_DIRECTORY "/usr/share/backup"
#define FORBIDDEN_DIRECTORY "/etc"

static
int copyFile(char* src, char* dst)
{
  unsigned int i, len;
  char buffer[2560]; /* 2.5K ought to be enough for anyone*/
  FILE *source, *dest;
  int c;

  source = fopen(src, "r");
  if (source == NULL) {
    fprintf(stderr, "Failed to open source file\n");
    return -1;
  }
  
  i = 0;
  c = fgetc(source);
  while (c != EOF) {
    buffer[i]  = (unsigned char) c;
    c = fgetc(source);
    i++;
  }

  len = i;
  fclose(source);
 
  dest = fopen(dst, "w");
  if (dest == NULL) {
    fprintf(stderr, "Failed to open destination file\n");
    return -1;
  }

  for(i = 0; i < len; i++) 
    fputc(buffer[i], dest);

  fclose(dest);

  return 0;
}

static
int restorePermissions(char* target)
{
  pid_t pid;
  int status;
  char *user, *userid, *ptr;
  FILE *file;
  char buffer[64];
  mode_t mode;

  // execute "chown" to assign file ownership to user
  pid = fork();
  
  // error
  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    return -1;
  }

  // parent
  if (pid > 0) {
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) == 0 || WEXITSTATUS(status) < 0)
      return -1;
  }
  else {

    // child
    // retrieve username
    user = getenv("USER");
    // retrieve corresponding userid   
    file = fopen("/etc/passwd", "r");
    if (file == NULL) {
      fprintf(stderr, "Failed to open password file\n");
      return -1;
    }
    userid = NULL;
    while (!feof(file)) {
      if (fgets(buffer, sizeof(buffer), file) != NULL) {
	ptr = strtok(buffer, ":");
	if (strcmp(ptr, user) == 0) {
	  strtok(NULL, ":"); // password
	  userid = strtok(NULL, ":"); // userid
	  ptr = strtok(NULL, ":"); // group
	  *ptr = '\0';
	  break;
	}
      }
    }

    if (userid != NULL) 
      execlp("/bin/chown", "/bin/chown", userid, target, NULL);

    // reached only in case of error
    return -1;
  }  

  mode = S_IRUSR | S_IWUSR | S_IEXEC;
  chmod(target, mode);

  return 0;
}

static
void usage(char* parameter) 
{
  char newline = '\n';
  char output[96];
  char buffer[96];

  snprintf(buffer, sizeof(buffer),
        "Syntax: %.60s backup|restore pathname password%c", parameter, newline);

  sprintf(output, buffer);
  printf(output);
}

static
int checkPassword(char* user_pw) {
  char dummy[128];
  int accept = 0;
  char pw[8];
  char len;
  
  // *** THIS PASSWORD CHANGES ***
  // Do not hardcode this password in your sploits!
  // It will be different when the assignment is marked
  char* correct_pw = "xf35+";

  memset(pw, 0, sizeof(pw));
  memset(dummy, 0, sizeof(dummy));
  
  len = strlen(user_pw);
  if (len < 8) {
    strcpy(pw, user_pw);
  }
    
  if (strcmp(pw, correct_pw) == 0) {
    accept = 1;
  }

  return accept;
}

int main(int argc, char* argv[]) 
{
  int cmd;
  char *path, *ptr;
  char *forbidden = FORBIDDEN_DIRECTORY;
  char *src, *dst, *buffer;
  struct stat buf;

  if (argc != 4) {
    usage(argv[0]);
    return 1;
  }

  if (strlen(argv[3]) > 128) {
    fprintf(stderr, "Password is too long!\n");
    return 1;
  }
  
  if (checkPassword(argv[3]) == 0) {
    fprintf(stderr, "Password is incorrect!\n");
    return 1;
  }
 
  if (strcmp("backup", argv[1]) == 0) {
    cmd = CMD_BACKUP;
  }
  else if (strcmp("restore", argv[1]) == 0) {
    cmd = CMD_RESTORE;
  } else {
    usage(argv[0]);
    return 1;
  }

  path = argv[2];

  // prevent access to forbidden directory
  ptr = realpath(path, NULL);
  if (ptr != NULL && strstr(ptr, forbidden) == ptr) {
    fprintf(stderr, "Not allowed to access target/source %s\n", path);
    return 1;
  }

  // set up paths for copy operation
  buffer = malloc(strlen(BACKUP_DIRECTORY) + 1 + strlen(path) + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    return 1;
  }
  
  if (cmd == CMD_BACKUP) {
    src = path;

    dst = buffer;
    strcpy(dst, BACKUP_DIRECTORY);
    strcat(dst, "/");
    strcat(dst, path);
  }
  else {
    src = buffer;
    strcpy(src, BACKUP_DIRECTORY);
    strcat(src, "/");
    strcat(src, path);

    dst = path;

    // don't overwrite existing file if we don't own it
    if (stat(dst, &buf) == 0 && buf.st_uid != getuid()) {
      fprintf(stderr, "Not your file: %s\n", dst);
      return 1;
    }
  }
  
  // perform actual backup/restore operation
  if (copyFile(src, dst) < 0)
    return 1;
    
  // grant user access to restored file
  if (cmd == CMD_RESTORE) {
    if (restorePermissions(path) < 0)
      return 1;
  }

  return 0;
}
