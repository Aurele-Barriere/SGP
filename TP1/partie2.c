#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
  // Forked process
  printf("Hello, dad\n");
  printf("after exit\n"); // This does not happen
  return 37;
}
