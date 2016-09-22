#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

  pid_t pid_son;
  int son_status;

  // Forking

  pid_son = fork();
  if (pid_son == -1) {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }
  if (pid_son == 0) {
    // This is the son process, we replace our code
    // with partie2's code
    if (execl("partie2", "partie2", (char*) NULL) == -1) {
      perror("Error: ");
      exit(EXIT_FAILURE);
    }
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
      perror("Error: ");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
