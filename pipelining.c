// Main reference: 1. http://pages.tacc.utexas.edu/~eijkhout/istc/istc.html

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

  clock_t begin_time;

  // 2 arrays of 3.2 GB of size each (for a total of 6.4 GB) fit in the RAM.
  size_t nn = 400000000;
  size_t total_mem_array = nn*sizeof(double);

  double *aa;
  double *aa_free;
  double *bb;
  double *bb_free;
  double ss = 0.0;
  double time_test = 0.0;

  aa = (double *) malloc(total_mem_array);
  aa_free = aa;
  bb = (double *) malloc(total_mem_array);
  bb_free = bb;

  int ii = 0;

  for (ii = 0; ii < nn; ++ii) {
    aa[ii] = 1.0;
    bb[ii] = 1.0;
  }

  // Pipelines require a number of independent operations to function
  // efficiently. An example of an operation that can not be pipelined is the
  // inner product sum ss [1]:

  begin_time = clock();
  for (ii = 0; ii < nn; ++ii) {
    ss += aa[ii]*bb[ii];
  }
  time_test = ((double) clock() - begin_time)/CLOCKS_PER_SEC;

  fprintf(stdout, "Memory arrays: %zu B.\n", 2*total_mem_array);

  fprintf(stdout, "Time test 1: %f s.\n", time_test);
  fprintf(stdout, "ss = %f\n", ss);

  // The fact that ss gets both read and written halts the addition pipeline.
  // One way to fill the floating point pipeline is to apply LOOP UNROLLING [1]:

  ss = 0.0;
  double sum1 = 0.0;
  double sum2 = 0.0;

  begin_time = clock();
  for (ii = 0; ii < nn/2 - 1; ++ii) {
    sum1 += aa[2*ii]*bb[2*ii];
    sum2 += aa[2*ii + 1]*bb[2*ii + 1];
  }
  ss = sum1 + sum2;
  time_test = ((double) clock() - begin_time)/CLOCKS_PER_SEC;

  fprintf(stdout, "Time test 2: %f s.\n", time_test);
  fprintf(stdout, "ss = %f\n", ss);

  // Note that there are two independent multiplies in between the
  // accumulations. With a little INDEXING OPTIMIZATION this becomes [1]:

  ss = 0.0;
  sum1 = 0.0;
  sum2 = 0.0;

  begin_time = clock();
  for (ii = 0; ii < nn/2 - 1; ++ii) {
    sum1 += (*(aa + 0))*(*(bb + 0));
    sum2 += (*(aa + 1))*(*(bb + 1));
    aa += 2;
    bb += 2;
  }
  ss = sum1 + sum2;
  time_test = ((double) clock() - begin_time)/CLOCKS_PER_SEC;

  fprintf(stdout, "Time test 3: %f s.\n", time_test);
  fprintf(stdout, "ss = %f\n", ss);

  // In a further optimization, we disentangle the addition and multiplication
  // part of each instruction. The hope is that while the accumulation is
  // waiting for the result of the multiplication, the intervening instructions
  // will keep the processor busy, in effect increasing the number of operations
  // per second.

  aa = aa_free;
  bb = bb_free;
  ss = 0.0;
  sum1 = 0.0;
  sum2 = 0.0;
  double temp1 = 0.0;
  double temp2 = 0.0;

  begin_time = clock();
  for (ii = 0; ii < nn/2 - 1; ++ii) {
    temp1 = (*(aa + 0))*(*(bb + 0));
    temp2 = (*(aa + 1))*(*(bb + 1));
    sum1 += temp1;
    sum2 += temp2;
    aa += 2;
    bb += 2;
  }
  ss = sum1 + sum2;
  time_test = ((double) clock() - begin_time)/CLOCKS_PER_SEC;

  fprintf(stdout, "Time test 4: %f s.\n", time_test);
  fprintf(stdout, "ss = %f\n", ss);

  // Finally, we realize that the furthest we can move the addition away from
  // the multiplication, is to put it right in front of the multiplication of
  // the next iteration.

  aa = aa_free;
  bb = bb_free;
  ss = 0.0;
  sum1 = 0.0;
  sum2 = 0.0;
  temp1 = 0.0;
  temp2 = 0.0;

  begin_time = clock();
  for (ii = 0; ii < nn/2 - 1; ++ii) {
    sum1 += temp1;
    temp1 = (*(aa + 0))*(*(bb + 0));
    sum2 += temp2;
    temp2 = (*(aa + 1))*(*(bb + 1));
    aa += 2;
    bb += 2;
  }
  ss = sum1 + sum2;
  time_test = ((double) clock() - begin_time)/CLOCKS_PER_SEC;

  fprintf(stdout, "Time test 5: %f s.\n", time_test);
  fprintf(stdout, "ss = %f\n", ss);

  free(aa_free);
  free(bb_free);

  return 0;
}
