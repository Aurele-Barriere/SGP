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
// See prod.c for details.
void check_perror_pipe(int n, int* fd) {
  if (n == -1) {
    perror("Error: ");
    if (fd) {
      close(*fd);
    }
    exit(EXIT_FAILURE);
  }
}


void consummer(int fd) {

  int read_chars; // number of read chars.
  char buffer[BUFFER_SIZE];

  // Main loop of the consummer process.
  // Reads characters one by one from the pipe, and display them vertically, on the side.
  do {
    read_chars = read(fd, buffer, 1);
    check_perror_pipe(read_chars, &fd);

    if (buffer[0] != '\n') {
      // This condition ensures we do not write an extra '\n'
      printf("                                    <consummer> %c\n", buffer[0]);
    }
  } while (read_chars);

  close(fd);
  check_perror_pipe(unlink("pipe"), NULL);
}

int main() {
  int fd_pipe;
  fd_pipe = open("pipe", O_RDONLY);
  check_perror_pipe(fd_pipe, NULL);

  consummer(fd_pipe);
  return 0;
}
