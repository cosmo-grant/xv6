// ping pong byte over pair of processes
// - set up pipes
// - fork
// - parent writes/reads
// - child reads/writes

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int ping[2];
  int pong[2];
  pipe(ping);
  pipe(pong);

  char buf[1] = "x";

  int pid = fork();
  if (pid == 0) {
    while (1) {
      read(ping[0], buf, 1);
      sleep(1);
      write(pong[1], buf, 1);
      printf("pong\n");
    }
  } else if (pid > 0) {
    while (1) {
      write(ping[1], buf, 1);
      printf("ping\n");
      read(pong[0], buf, 1);
      sleep(1);
    }
  } else {
    printf("fork error\n");
  }
}
