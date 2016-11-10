/* Testing locks */
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

LockId lock;
int n;

int fibo(unsigned n){
  if (n <= 1) {
    return 1;
  } 
  return fibo (n-1) + fibo (n-2);
}

void writer() {
  unsigned i;
  for (i = 0; i < 1000; i++) {
    LockAcquire(lock);
    n = n+1;
    n_printf("Loop %d: n = %d\n", i, n);
    LockRelease(lock);
  }
}

int main() {
  n = 0;
  lock = LockCreate("lock");
  ThreadId t1 = threadCreate("t1", writer);
  ThreadId t2 = threadCreate("t2", writer);

  Join(t1);
  Join(t2);

  LockDestroy(lock);

  n_printf("Everything is OK!\n");
  
  return 0;
}
