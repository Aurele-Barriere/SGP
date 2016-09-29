#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

int i = 0;
jmp_buf env;

void handler (int x) {
  if (x == SIGALRM) {
    printf("Number of iterations: %d\n", i);
    // Calling the alarm
    alarm(1);
  }
  if (x == SIGINT) {
    longjmp(env, 1);
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
  
  if (sigaction(SIGALRM, &act, NULL) == -1 ||
      sigaction(SIGINT , &act, NULL) == -1) {
    perror ("error:");
    exit(1);
  }

  // Testing our handler
  int s = setjmp(env);
  sigset_t set;
  if(s) { printf("\nContext has been successfully restored\n"); }
  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGALRM);
  sigprocmask(SIG_UNBLOCK, &set, NULL);
  // Calling the alarm
  alarm(1);
  i = 0;
  fibo(44);

  return 0;
}
