#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int pid;

  pid = fork();
  if (pid == 0) {
    printf("%d: received ping\n", getpid());
    exit(0);
  } else if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }
  wait(0);
  printf("%d: received pong\n", getpid());

  exit(0);
}
