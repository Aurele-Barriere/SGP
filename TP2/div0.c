#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

// Generic function to check if an error occured,
// and exits then.
void check_perror(int n) {
  if (n == -1) {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }
}

// Handles our signal
void handle (int x) {
  if (x == SIGFPE) {
    printf("Division by zero \n");
    exit(EXIT_FAILURE);
  }
}

int main () {
  int i;

  struct sigaction act;
  act.sa_handler = handle;

  // Links our handler to the signal SIGFPE (division by zero)
  check_perror(sigaction(SIGFPE, &act, NULL));

  // Testing our handler
  for (i = 5; i >= 0; --i) {
    printf(" 100 / %d = %d\n", i, 100/i);
  }

  printf("Loop completed \n"); // This is never reached

  return 0;
}
