#include <stdio.h>

#include "papi_test.h"

extern int TESTS_QUIET;

#define PCHECK(e) \
  if (e!=PAPI_OK) \
    {fprintf(stderr, "Problem in PAPI call, line %d\n", __LINE__); return 1;}

#define L1WORDS 8096
#define L2WORDS 256000
#define NEVENTS 3
#define NRUNS 200

int main(int argc, char **argv) {

  double *array;

  int events[NEVENTS] = {PAPI_TOT_CYC, PAPI_L1_DCM, PAPI_L2_DCM};
  
  int retval;
  int event_code;
  int m;
  int n;
  int i;
  int j;
  int size;
  int arraysize;
  
  long_long values[NEVENTS];
  
  const PAPI_hw_info_t *hwinfo = NULL;
  const PAPI_substrate_info_t *s = NULL;
  PAPI_event_info_t info;
  PAPI_event_info_t info1;
  
  tests_quiet(argc, argv);
  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT) {
    test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);
  }

  {
    int i;
    
    for (i = 0; i < NEVENTS; i++) {
      retval = PAPI_query_event(events[i]) ;
      PCHECK(retval);
    }
  }
  
  /* declare an array that is more than twice the L2 cache size */
arraysize=2*L2WORDS;
array = (double*) malloc(arraysize*sizeof(double));

return 0;
}

