# TP3
Aurèle Barrière & Jérémy Thibault

We implemented matrix multiplication for fixed-size matrices with 2 methods:
the first one using a sequential computation and the second using threads
in order to parallelize the computation.
This is done using the _pthreads_ library.

In order to give each threads their arguments, we defined a new structure `prod_args` which contains pointers to the matrices and a single thread identifier.
We also need to not forget to wait for the threads to end.

One difficulty appeared: we had to use the  `float (*m1)[MAT_SIZE];` syntax to define 2D arrays in the structure, since the other possibilities we tried (such as `float m1[MAT_SIZE][MAT_SIZE]`) lead to errors or warnings.

Finally, we made sure our program runs correctly by comparing our multi-threaded result to the sequential result.