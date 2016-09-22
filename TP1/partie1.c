#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


// Generic function to check if an error occured,
// and exits then.
void check_perror(int n) {
  if (n == -1) {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }
}

int main() {

  pid_t pid_son;
  int son_status;

  // Forking
  pid_son = fork();
  check_perror(pid_son);

  if (pid_son == 0) {
    // This is the son process
    // Replaces the executed code. Iff there's an error the return value will be checked.
    check_perror(execl("partie2", "partie2", (char*) NULL));
  } else {
    // If we are in the father process, we wait
    // for the son to end.

    if (pid_son == wait(&son_status)) {
      // The son process that ended was the one we created.
      // This should not be a problem but we must be sure
      if (WIFEXITED(son_status)) {
        printf ("Process %d terminated normally and returned %d\n"
                , (int) pid_son, WEXITSTATUS(son_status));
      }
    } else {
      // The only possible case is that pid_son == -1, there was an error.
      check_perror(pid_son);
    }
  }

  return 0;
}
