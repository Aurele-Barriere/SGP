#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>


#define BUFFER_SIZE 1024




void consummer(int fd) {
  
  int read_chars;
  char buffer[BUFFER_SIZE];
  
  while (read_chars = read(fd, buffer, 1)) {
    if (buffer[0] != '\n') {
      printf("                                    <consummer> %c\n", buffer[0]);
    }
    
  }
  close (fd);
  exit(1);
}

int main() {
  int fd_pipe;
  fd_pipe = open("pipe", O_RDONLY);
  if (fd_pipe == -1) {
    exit(1);
  }
  consummer(fd_pipe);
  return 0;
}
