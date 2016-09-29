#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int i = 0;

void handler (int x) {
  if (x == SIGALRM) {
    printf("Number of iterations: %d\n", i);
    // Calling the alarm
    alarm(1);
  }
}


int fibo (int n) {
  i += 1;
  if (n==0 || n==1) { return 1;}
  else { return fibo(n-1) + fibo(n-2); }
}

int main () {

  // Creating the sigaction
  struct sigaction act;
  act.sa_handler = handler;
  act.sa_flags = SIG_DFL;
  
  if (sigaction(SIGALRM, &act, NULL) == -1) {
    perror ("error:");
    exit(1);
  }

  // Calling the alarm
  alarm(1);
  
  // Testing our handler
  fibo(43);


  return 0;
}
