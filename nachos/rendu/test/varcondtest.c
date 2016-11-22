/* Testing Condition variables */
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

SemId sema;
CondId cond;

/*
  This program aims to test the behaviour of condition variables.
  However, the program shouldn't be considered as a good example outside of this particular OS.
  Indeed, in general there's no way to be sure that some thread is waiting on a condition variable. If it sends a signal (or a V, or modify a variable) to warn that it's going to wait, it could always been interrupted between the signal and the wait. Disabling interrupts to solve this would not be a good solution either as it would be a wait in a critical section and a possible source of deadlock.
  Luckily, in our OS, the scheduler will never interrupt our threads on something differerent from a P or a Wait. Thus, we can use a V on a semaphore to warn the thread that we're going to wait without being interrupted, and then the other thread can wake the other threads with Signal or Broadcast.

  In brief, this program works in our specific case, but don't test it on another OS.
 */



void doubleWait () {
  // This first thread wait for the broadcast, then for the signal on the condition variable
  V(sema);
  CondWait(cond);
  n_printf("Broadcast received for first thread\n");
  V(sema);
  CondWait(cond);
  n_printf("Signal received for first thread\n");
}

void simpleWait () {
  // The second threads only waits for the broadcast
  V(sema);
  CondWait(cond);
  n_printf("Broadcast received for second thread\n");
}

void signalBroadcast () {
  // Once the previous threads are waiting, broadcast on the condition variable
  P(sema);
  P(sema);
  n_printf("Broadcasting\n");
  CondBroadcast(cond);
  P(sema);
  // Once the first thread is waiting again, send a signal
  n_printf("Sending signal\n");
  CondSignal(cond);
}




int main() {
  sema = SemCreate("sema",0);
  cond = CondCreate("condition variable");
  ThreadId t1 = threadCreate("t1", doubleWait);
  ThreadId t2 = threadCreate("t2", simpleWait);
  ThreadId t3 = threadCreate("t3", signalBroadcast);

  Join(t1);
  Join(t2);
  Join(t3);

  SemDestroy(sema);
  CondDestroy(cond);

  n_printf("Everything is OK!\n");
  
  return 0;
}
