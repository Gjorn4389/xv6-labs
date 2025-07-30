#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ(x)   (x[0])
#define WRITE(x)  (x[1])
#define LIMIT     (280)
#define SIZE      sizeof(int)

#define SAFE_write(a, b, c) \
do { \
  if (write(a, b, c) != c) { \
    printf("%s[%d]: write fail\n", __func__, getpid()); \
    exit(1); \
  } \
} while(0)

#define SAFE_read(a, b, c) \
do { \
  if (read(a, b, c) != c) { \
    printf("%s[%d]: read fail\n", __func__, getpid()); \
    exit(1); \
  } \
} while(0)

static int safefork()
{
  int pid;

  if ((pid = fork()) < 0) {
    printf("%s[%d]: fork fail", __func__, getpid());
    exit(1);
  }
  return pid;
}

__attribute__((noreturn))
void primes(int *cur)
{
  int b;
  int next[2];

  close(WRITE(cur));
  if (read(READ(cur), &b, SIZE) != SIZE) {
    close(READ(cur));
    exit(0);
  }
  printf("prime %d\n", b);

  if (pipe(next) < 0) {
    printf("%s[%d]: pipe failed.\n", __func__, getpid());
    exit(1);
  }
  int pid = safefork();

  // It's important to fork child processes in parallel.
  // In this way, main process only has grandchild processes.
  if (pid != 0) {
    close(READ(cur));
    primes(next);
  } else {
    close(READ(next));
    int n;
    while (read(READ(cur), &n, SIZE) == SIZE) {
      if (n % b == 0) {
        continue;
      }
      SAFE_write(WRITE(next), &n, SIZE);
    }
    close(READ(cur));
    close(WRITE(next));
    wait(0);
  }
  exit(0);
}

int
main(int argc, char *argv[])
{
  int next[2];
  int pid;

  if (pipe(next) < 0) {
    printf("%s[%d]: pipe failed.\n", __func__, getpid());
    exit(1);
  }

  pid = safefork();
  if (pid == 0) {
    primes(next);
  } else {
    close(READ(next));
    for (int i = 2; i <= LIMIT; i++) {
      SAFE_write(WRITE(next), &i, SIZE);
    }
    close(WRITE(next));
    wait(0);
  }

  exit(0);
}
