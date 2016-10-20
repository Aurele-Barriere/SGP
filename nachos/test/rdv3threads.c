/* rendez vous between 3 threads
 */

#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int
main()
{

  SemId s1 = SemCreate("sema1", 0);
  SemId s2 = SemCreate("sema2", 0);
  SemId s3 = SemCreate("sema3", 0);

  
  // we'll call threadCreate or newThread with a function describing each thread's behaviour
  

  
  return 0;
}
