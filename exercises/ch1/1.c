// ping pong byte over pair of processes
// - set up pipes
// - fork
// - parent writes/reads
// - child reads/writes

#include <bits/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

volatile int keep_running = 1;

void interruptHandler(int sig) { keep_running = 0; }

int main(int argc, char *argv[]) {
  signal(SIGINT, interruptHandler);

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

  int exchanges = 0;

  int pid = fork();
  if (pid == 0) {
    while (keep_running) {
      read(ping[0], &buf, 1);
      write(pong[1], &buf, 1);
    }
  } else if (pid > 0) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    while (keep_running) {
      write(ping[1], &buf, 1);
      read(pong[0], &buf, 1);
      exchanges++;
    }

    // exchange rate
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_seconds = (end.tv_sec - start.tv_sec) +
                             (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("exchanges per second: %f\n", exchanges / elapsed_seconds);

  } else {
    printf("fork error\n");
  }
}
