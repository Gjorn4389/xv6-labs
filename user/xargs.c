#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define READ_IDX    0
#define WRITE_IDX   1
#define BUF_SIZE    512

int
safe_fork()
{
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "fork fail\n");
    exit(1);
  }
  return pid;
}

void
fmt_argv(int argc, char *argv[])
{
  for (int i = 0; i < argc - 1; i++) {
    memset(argv[i], 0, strlen(argv[i]));
    memcpy(argv[i], argv[i + 1], strlen(argv[i + 1]));
  }
  memset(argv[argc - 1], 0, strlen(argv[argc]));
}

int
readline(char *buf)
{
  char *p = buf;
  while (*p != '\0' && *p != '\n') {
    p++;
  }
  return p - buf;
}

int
main(int argc, char *argv[])
{
  char buf[BUF_SIZE] = {0};
  char *p;
  int len;

  fmt_argv(argc, argv);
  while (read(0, buf, BUF_SIZE) > 0) {
    p = buf;
    while ((len = readline(p)) > 0) {
      memcpy(argv[argc - 1], p, len);
      p += len + 1;
      int pid = safe_fork();
      if (pid == 0) {
        exec(argv[0], argv);
      } else {
        wait(0);
      }
      memset(argv[argc - 1], 0, len);
    }
    memset(buf, 0, BUF_SIZE);
  }

  exit(0);
}
