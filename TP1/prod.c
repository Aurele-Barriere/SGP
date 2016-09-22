#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>


#define BUFFER_SIZE 1024


void producer(int fd) {
  
  char buffer[BUFFER_SIZE];
  buffer[0] = '\0';
  
  while (buffer[0] != '\n') {
    printf("<producer> Type something in :\n");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      // Problem during input
      exit (1);
    }
    write(fd, buffer, strlen(buffer));
  }
  
  close (fd);  
}


int main() {
  
  pid_t pid_cons;
  int fd_pipe;
  
  // Creating the pipe
  if (mknod("pipe", S_IFIFO | 0666,0) == -1) {
    exit(1);
  }
  
  // Forking
  pid_cons = fork();

  if (pid_cons==-1) {
    return 1; // Failed to fork
  }
  
  if (pid_cons==0) {
    // If we are the son 
    execl("cons", NULL);
  }

  else {
    // If we are the father
    if ((fd_pipe = open("pipe", O_WRONLY)) == -1) {
      exit(1);
    }
    
    producer(fd_pipe);
  }

 
  return 0;
}
