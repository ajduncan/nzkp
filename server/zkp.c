#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <time.h>

#include "zkp.h"

/***************************************************************************
  Feige-Fiat-Shamir Zero Knowledge Identification Scheme: an implementation

          by Daniele Raffo, 25 JUN 2002 - LIX, Ecole Polytechnique
 ***************************************************************************/

int fastseed = FALSE;

int main(int argc, char **argv) {
  // mpz_init(rndseed);

  printf("Feige-Fiat-Shamir ZKP implementation (server)\n");

  if (argc > 1) {
    fastseed = TRUE;
    // printf("Warning: fastseed enabled, using a bad random seed value!\n");
  }
  // printf("\n");

  printf("Starting server up at: ");
  timestamp(stdout);
  printf("\n");

  zkp_server();  

  // mpz_clear(rndseed);
  return (0);
}



/*
  Set the random seed from /dev/random
*/
void setrndseed() {

  FILE *rnd;
  mpz_t rndtmp;
  unsigned long int idx;
  time_t t1;

  if (!fastseed) { 
    mpz_init(rndtmp);
    
    rnd = fopen("/dev/urandom", "r");
    
    for (idx = 0; idx < 128; idx++) {
      mpz_set_ui(rndtmp, (unsigned long int) getc(rnd));
      mpz_mul_2exp(rndtmp, rndtmp, idx * 8);     /* left shift */
      mpz_add(rndseed, rndseed, rndtmp);
    }
    
    fclose(rnd);
    mpz_clear(rndtmp);
  }

  else {
    /* Set a faster seed. Do not use this for cryptographic purposes! */
    mpz_set_ui(rndseed, (unsigned long int) time(&t1));
    mpz_mul_ui(rndseed, rndseed, (unsigned long int) getpid()); 
    mpz_mul_ui(rndseed, rndseed, (unsigned long int) getppid()); 
  }
}


