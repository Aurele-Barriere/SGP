/* Receiving a message from another nachos */

#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int
main()
{
  n_printf("Receiving message\n");
  int i;
  int lg = 6;
  char mess [10];
  
  i = TtyReceive(mess, lg+1);

  if (i == lg + 1) {
    n_printf("message successfully received\n");
  }

  n_printf("message: %s\n", mess);
  
  return 0;
}
