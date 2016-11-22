/* rendez-vous between 3 threads */

#include "userlib/syscall.h"
#include "userlib/libnachos.h"


SemId s_free; 
SemId s_full;
int buff[8];

int fibo(unsigned n){
  if (n <= 1) {
    return 1;
  } 
  return fibo (n-1) + fibo (n-2);
}

void producer() {
  unsigned i;
  int n;

  
  for (i = 0; i < 5; i++) {
    n = fibo(i);

    // critical section
    P(s_free);
    buff[i % 8] = n;
    V(s_full);
    
    n_printf("Wrote fibo n° %d\n", i);
  }
}

void consummer() {
  unsigned i;
  int n;

  for (i = 0; i < 5; i++) {
    // critical section
    P(s_full);
    n = buff[i % 8];
    V(s_free);

    n_printf("Fibo(%d) = %d\n", i, n);
  }
}


int main() {

  s_free = SemCreate("s_free", 8);
  s_full = SemCreate("s_full", 0);

  ThreadId prod = threadCreate("prod", producer);
  ThreadId cons = threadCreate("cons", consummer);

  Join(prod);
  Join(cons);

  SemDestroy(s_free);
  SemDestroy(s_full);

  n_printf("Everything is OK!\n");
  
  return 0;
}
