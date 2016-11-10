/* rendez-vous between 3 threads */

#include "userlib/syscall.h"
#include "userlib/libnachos.h"


SemId s1; 
SemId s2;
SemId s3;

void useful_loop (int n) {
  // computing something
  int i;
  for (i=0; i<n; i++) {
  }
}

void thread1 () {
  useful_loop(1000);
  n_printf("First thread arrived\n");
  V(s1);
  V(s1);
  P(s2);
  P(s3);
  n_printf("First thread is gone\n");
}

void thread2 () {
  useful_loop(100000);
  n_printf("Second thread arrived\n");
  P(s1);
  V(s2);
  V(s2);
  P(s3);
  n_printf("Second thread is gone\n");
}

void thread3 () {
  useful_loop(5000);
  n_printf("Third thread arrived\n");
  P(s1);
  P(s2);
  V(s3);
  V(s3);
  n_printf("Third thread is gone\n");
}


int main() {
  s1 = SemCreate("sema1", 0);
  s2 = SemCreate("sema2", 0);
  s3 = SemCreate("sema3", 0);
  
  ThreadId t1 = threadCreate((char*) "First thread", thread1);
  ThreadId t2 = threadCreate((char*) "Second thread", thread2);
  ThreadId t3 = threadCreate((char*) "Third thread", thread3);

  n_printf("00join\n");
  Join(t1);
  n_printf("1join\n");
  Join(t2);
  Join(t3);

  n_printf("Destroying semaphores\n");
  SemDestroy(s1);
  SemDestroy(s2);
  SemDestroy(s3);
  n_printf("Done destroying, bye!\n");
  return 0;
}
