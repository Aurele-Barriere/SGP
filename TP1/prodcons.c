#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024


void close_pipe_fd(int* fd) {
  // Closing file descriptors
  close(fd[0]);
  close(fd[1]);
}

// Generic function to check if an error occured,
// and exits then.
// Also closes the given pipe file descriptor if its not NULL
void check_perror_pipe(int n, int* fd) {
  if (n == -1) {
    perror("Error: ");
    if (fd) {
      close_pipe_fd(fd);
    }
    exit(EXIT_FAILURE);
  }
}

void producer(int* fd) {

  char buffer[BUFFER_SIZE];
  int written_chars; // number of written chars

  // Main producer loop: tries to read entry from the standard input
  // until the user submits an "empty" chain (ie a single '\n' character)
  do {

    printf("<producer> Type something in :\n");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      // Problem during input
      close_pipe_fd(fd);
      exit(EXIT_FAILURE);
    }

    written_chars = write(fd[1], buffer, strlen(buffer));
    check_perror_pipe(written_chars, fd);
  } while (buffer[0] != '\n');

  close_pipe_fd(fd);
}

void consummer(int* fd) {

  int read_chars; // number of read chars.
  char buffer[BUFFER_SIZE];

  // Main loop of the consummer process.
  // Reads characters one by one from the pipe, and display them vertically, on the side.
  do {
    read_chars = read(fd[0], buffer, 1);
    check_perror_pipe(read_chars, fd);

    if (buffer[0] != '\n') {
      // This condition ensures we do not write an extra '\n'
      printf("                                    <consummer> %c\n", buffer[0]);
    }
  } while (read_chars);

  close_pipe_fd(fd);
}

int main() {
  int pipe_created;
  int fd[2]; // The file descriptors of the pipe
  pid_t pid_cons; // Consummer's pid

  // Creating the pipe
  pipe_created = pipe(fd);
  check_perror_pipe(pipe_created, NULL);

  // Forking
  pid_cons = fork();
  check_perror_pipe(pid_cons, fd);

  if (pid_cons==0) {
    // We are in the son process, the consummer
    consummer(fd);
  } else {
    // We are in the father process, the producer
    producer(fd);
  }

  return 0;
}
