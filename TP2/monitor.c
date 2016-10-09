#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

pid_t pid_son;
int son_status;
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

  for (i = 0; i<n; i++);
  printf("Loop done\n");
}

void handler (int x) {

  if (x == SIGALRM) {
    alarm(1);
    kill(pid_son, SIGALRM);
  }
}




int main () {

  pid_son = fork();
  check_perror(pid_son);

  if (pid_son == 0) {
    // This is the son process
    ptrace(PTRACE_TRACEME);
    loop(1999999999);
    exit(EXIT_SUCCESS);

  } else {

    // This is the parent process

    int r = 0; // will contain the response to ptrace(PTRACE_PEEKDATA, ...)
    char cont = 1; // will be set to 0 when the son process ends

    // Creating the sigaction
    struct sigaction act;
    act.sa_handler = handler;

    check_perror(sigaction(SIGALRM, &act, NULL));
    // check_perror(sigaction(SIGINT , &act, NULL));

    alarm(1);
    wait(&son_status);
    cont = 1;
    do {
      wait(&son_status);
      if (WIFEXITED(son_status)) {
        printf("Son process ended\n");
        cont = 0;
      } else {
        r = ptrace(PTRACE_PEEKDATA, pid_son, &i, NULL);
        printf("Number of iterations of the son: %d\n", r);
        ptrace(PTRACE_CONT, pid_son, 1, NULL);
      }
    } while (cont);
  }

  return 0;
}
