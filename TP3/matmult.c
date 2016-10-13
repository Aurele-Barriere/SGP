#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MAT_SIZE 64
#define NB_THREADS 8
#define NB_IT 8 // MAT_SIZE / NB_THREADS



// The struct that the threads get as an argument
typedef struct {
  // Pointers to our 3 matrices
  float (*m1)[MAT_SIZE];
  float (*m2)[MAT_SIZE];
  float (*m)[MAT_SIZE];
  unsigned i; // Thread identifier
} prod_args;


// A sequential function to verify our multi-threaded computation
void prod_mat (float m1[MAT_SIZE][MAT_SIZE],
	        float m2[MAT_SIZE][MAT_SIZE],
	        float m[MAT_SIZE][MAT_SIZE]) {
  
  unsigned i,j,k;
  for (i = 0; i < MAT_SIZE; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      for (k = 0; k < MAT_SIZE; k++) {
        m[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
}


// The start-routine of the Pthreads
void prod_thread(prod_args* args) { 
  unsigned j,k,ii;
  float (*m1)[MAT_SIZE] = args->m1;
  float (*m2)[MAT_SIZE] = args->m2;
  float (*m)[MAT_SIZE] = args->m;
  unsigned i = args->i;

  // Computes the thread's assigned lines
  for (ii = 0; ii < NB_IT; ii++) {
    for (j = 0; j < MAT_SIZE; j++) {
      for (k = 0; k < MAT_SIZE; k++) {
        m[ii*NB_THREADS+i][j] += m1[ii*NB_THREADS+i][k] * m2[k][j];
      }
    }
  }
  printf ("Pthread %d: calculation over\n", i);
}


int main() {

  float m1[MAT_SIZE][MAT_SIZE], m2[MAT_SIZE][MAT_SIZE];
  float m [MAT_SIZE][MAT_SIZE];
  float m_test[MAT_SIZE][MAT_SIZE]; // The matrix in which we'll store the real result
  char test = 1; 

  // Threads array and argument array (which will be given to the start routines)
  pthread_t threads[NB_THREADS];
  prod_args args[NB_THREADS];

  unsigned i,j;

  // Creating random matrices
  for (i = 0; i < MAT_SIZE; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      m1[i][j] = (float)rand() / (float)(RAND_MAX/100);
      m2[i][j] = (float)rand() / (float)(RAND_MAX/100);
      m[i][j] = 0;
    }
  }

  // Initializes arguments and creates the threads
  for (i = 0; i < NB_THREADS; i++) {
    (args[i]).m1 = m1;
    (args[i]).m2 = m2;
    (args[i]).m = m;
    (args[i]).i = i;
    pthread_create(&threads[i], NULL, (void * (*)(void *)) prod_thread, (void*) &args[i]);
  }

  // Waits for the threads to terminate
  for (i = 0; i < NB_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  // Verification
  prod_mat (m1, m2, m_test);

  for (i = 0; i < MAT_SIZE; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      if (m[i][j] != m_test[i][j]) {
	test = 0;
      }
    }
  }

  if (test) {
    printf("The multiplication is a success\n");
  } else {
    printf("Error\n");
  }
  

  return 0;
}
