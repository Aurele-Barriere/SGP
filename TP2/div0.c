#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handle (int x) {
  if (x == SIGFPE) {
    printf("Division by zero \n");
  }
  exit(1);
}

int main () {
  
  struct sigaction act;
  act.sa_handler = handle;
  act.sa_flags = SIG_DFL;
  
  if (sigaction(SIGFPE, &act, NULL) == -1) {
    perror ("error:");
    exit(1);
  }

  // Testing our handler
  printf("4/2: %d\n", 4/2);
  printf("4/0: %d\n", 4/0);
  return 0;
}
