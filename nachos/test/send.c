/* Sending a message to another nachos */

#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int
main()
{
  n_printf("Sending message\n");
  int i;

  char* mess = "Hello!";
  int lg = 6;
  
  i = TtySend(mess);

  if (i == lg + 1) {
    n_printf("message successfully sent\n");
  }
    
  
  return 0;
}
