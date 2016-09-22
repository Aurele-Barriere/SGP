#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

// Generic function to check if an error occured,
// and exits then.
// Also closes the given pipe file descriptor if its not NULL.
//
// We use a pointer to emulate an option type: if fd == NULL, it means there is no
// file to close.
void check_perror_pipe(int n, int* fd) {
  if (n == -1) {
    perror("Error: ");
    if (fd) {
      close(*fd);
    }
    exit(EXIT_FAILURE);
  }
}

void producer(int fd) {

  char buffer[BUFFER_SIZE];
  int written_chars; // number of written chars

  // Main producer loop: tries to read entry from the standard input
  // until the user submits an "empty" chain (ie a single '\n' character)
  do {

    printf("<producer> Type something in :\n");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      // Problem during input
      close(fd);
      exit(EXIT_FAILURE);
    }

    written_chars = write(fd, buffer, strlen(buffer));
    check_perror_pipe(written_chars, &fd);
  } while (buffer[0] != '\n');

  close(fd);
}


int main() {

  int fd_pipe;

  // Creates the pipe
  check_perror_pipe(mknod("pipe", S_IFIFO | 0666,0), NULL);

  // Producer process
  fd_pipe = open("pipe", O_WRONLY);
  check_perror_pipe(fd_pipe, NULL);

  producer(fd_pipe);

  return 0;
}
