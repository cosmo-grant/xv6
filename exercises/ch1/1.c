// ping pong byte over pair of processes
// - set up pipes
// - fork
// - parent writes/reads
// - child reads/writes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int ping[2];
  int pong[2];
  pipe(ping);
  pipe(pong);

  if (argc < 2 || strlen(argv[1]) != 1) {
    printf("Expected 1 byte. Got %s, which is %zu bytes.\n", argv[1],
           sizeof argv[1]);
    exit(1);
  }
  char buf = argv[1][0];

  int pid = fork();
  if (pid == 0) {
    while (1) {
      read(ping[0], &buf, 1);
      sleep(1);
      write(pong[1], &buf, 1);
      printf("pong: %c\n", buf);
    }
  } else if (pid > 0) {
    while (1) {
      write(ping[1], &buf, 1);
      printf("ping: %c\n", buf);
      read(pong[0], &buf, 1);
      sleep(1);
    }
  } else {
    printf("fork error\n");
  }
}
