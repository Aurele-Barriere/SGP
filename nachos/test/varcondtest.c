/* Testing locks */
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

SemId sema;
CondId cond;


void doublewait () {
  V(sema);
  CondWait(cond);
  n_printf("Broadcast received for first thread\n");
  V(sema);
  CondWait(cond);
  n_printf("Signal received for first thread\n");
}

void simplewait () {
  V(sema);
  CondWait(cond);
  n_printf("Broadcast received for second thread\n");
}

void signalbroadcast () {
  P(sema);
  P(sema);
  n_printf("Broadcasting\n");
  CondBroadcast(cond);
  P(sema);
  n_printf("Sending signal\n");
  CondSignal(cond);
}




int main() {
  sema = SemCreate("sema",0);
  cond = CondCreate("condition variable");
  ThreadId t1 = threadCreate("t1", doublewait);
  ThreadId t2 = threadCreate("t2", simplewait);
  ThreadId t3 = threadCreate("t3", signalbroadcast);

  Join(t1);
  Join(t2);
  Join(t3);

  SemDestroy(sema);
  CondDestroy(cond);

  n_printf("Everything is OK!\n");
  
  return 0;
}
