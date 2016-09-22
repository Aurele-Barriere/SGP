#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

  // Forking
  pid_t i;
  pid_t w;
  int e;
  int son_return;
  i = fork();
  if (i==-1) { return 1; }
  if (i==0) {
    // If we are the son, we replace or own executable with partie2
    e = execl("partie2", NULL);
  }
  else {
    w = wait(&son_return);
    if (w == i) {
      // If the returned son is the one we forked
      printf ("I am the father, %d\n", WEXITSTATUS(son_return));
    }
  }
  
  
  return 0;
}
