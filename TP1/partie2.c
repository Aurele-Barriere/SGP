#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
  // Forked process
  printf("Son process returning 37\n");
  return 37;
}
