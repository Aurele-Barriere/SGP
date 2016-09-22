#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

void close_fd(int* fd) {
  // Closing file descriptors
  close(fd[0]);
  close(fd[1]);
}

void producer(int* fd) {
  
  char buffer[BUFFER_SIZE];
  buffer[0] = '\0';
  
  
  
  while (buffer[0] != '\n') {
    printf("<producer> Type something in :\n");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      // Problem during input
      close_fd(fd);
      exit (1);
    }
    write(fd[1], buffer, strlen(buffer));
  }
  
  
  close_fd(fd);
  
}

void consummer(int* fd) {
  
  int read_chars;
  char buffer[BUFFER_SIZE];
  
  
  while (read_chars = read(fd[0], buffer, 1)) {
    if (buffer[0] != '\n') {
      printf("                                    <consummer> %c\n", buffer[0]);
    }
    
  }
  
  if (read_chars == -1) {
    // Failed to read
    close_fd(fd);
    exit (1);
  }
  if (read_chars == 0) {
    // EOF
  }

  close_fd(fd);
  exit(1);
}

int main() {
  int pipe_created;
  int fd[2]; // The file descriptors of the pipe
  pid_t pid_cons; // Consummer's pid
  
  // Creating the pipe
  pipe_created = pipe(fd);

  if (pipe_created == -1) {
    // Failed to open
    return 1 ;
  }

  // Forking
  pid_cons = fork();

  if (pid_cons==-1) {
    return 1; // Failed to fork
  }
  
  if (pid_cons==0) {
    // If we are the son
    consummer(fd);
  }

  else {
    // If we are the father
    producer(fd);
  }

  

  return 0;
}
