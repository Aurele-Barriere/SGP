#include <pthread.h>
#include <stdlib.h>

typedef struct {
  float m1[64][64];
  float m2[64][64];
  float m[64][64];
  unsigned i;
} prod_args;

void prod_mat (float m1[64][64], float m2[64][64], float m[64][64]) {
  unsigned i,j,k;
  for (i = 0; i < 64; i++) {
    for (j = 0; j < 64; j++) {
      for (k = 0; k < 64; k++) {
        m[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }

}

void prod_thread(prod_args* args) {
  unsigned j,k,ii;
  float m1[64][64] = args->m1;
  float m2[64][64] = args->m2;
  float m[64][64] = args->m;
  unsigned i = args->i;

  for (ii = 0; ii < 8; ii++) {
    for (j = 0; j < 64; j++) {
      for (k = 0; k < 64; k++) {
        m[ii*8+i][j] += m1[ii*8+i][k] * m2[k][j];
      }
    }
  }
}

int main() {

  float m1[64][64], m2[64][64];
  float m [64][64];

  pthread_t threads[8];
  prod_args args[8];

  unsigned i,j;

  for (i = 0; i < 64; i++) {
    for (j = 0; j < 64; j++) {
      m1[i][j] = (float)rand() / (float)(RAND_MAX/100);
      m2[i][j] = (float)rand() / (float)(RAND_MAX/100);
      m[i][j] = 0;
    }
  }

  for (i = 0; i < 8; i++) {
    args[i].m1 = m1; args[i].m2 = m2; args[i].m = m; args[i].i = i;
    pthread_create(&threads[i], NULL, prod_thread, (void*) &args[i]);
  }

  return 0;
}
