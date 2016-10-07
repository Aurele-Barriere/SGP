#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

pid_t pid_son;
int i = 0;

// Generic function to check if an error occured,
// and exits then.
void check_perror(int n) {
  if (n == -1) {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }
}

void loop (int n) {
  for (i = 0; i<n; i++) {
    // Nothing
  }
  printf("Loop done");
}

void handler (int x) {

  if (x == SIGALRM) {

    alarm(1);
    kill(pid_son, SIGALRM);
    wait(NULL);
    ptrace(PTRACE_PEEKDATA, pid_son, &i, NULL);
    printf("Number of iterations of the son: %d\n", i);
    ptrace(PTRACE_CONT, pid_son, 1, NULL);
  }
}




int main () {

  int son_status;



  pid_son = fork();
  check_perror(pid_son);

  if (pid_son == 0) {
    // This is the son process
    ptrace(PTRACE_TRACEME);
    loop(1999999999);

  }
  else {
    // This is the parent process
    // Creating the sigaction
    struct sigaction act;
    act.sa_handler = handler;
    
    if (sigaction(SIGALRM, &act, NULL) == -1 ||
	sigaction(SIGINT , &act, NULL) == -1) {
      perror ("error:");
      exit(1);
    }
    //wait(&son_status);
    alarm(1);

  }


  return 0;
}
