#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include "zkp.h"

static mpz_t s[K];   /* private key */
static mpz_t r;      /* random number */

/*
  Choose private and public key
*/
void compute_keys() {
  int index = 0, index2, flag;
  mpz_t candidate, inverse;
  gmp_randstate_t state;

  // printf("Computing keys\n");

  gmp_randinit_lc_2exp_size(state, 128);
  setrndseed();
  gmp_randseed(state, rndseed);
  mpz_init(candidate);
  mpz_init(inverse);

  while (index < K) {
    // printf("Iteration: %i\n ", index);
    mpz_init(i[index]);
    mpz_init(s[index]);

    mpz_urandomm(candidate, state, n);

    /* test if candidate has already been chosen as key component */
    flag = FALSE;
    for (index2 = index - 1; index2 >= 0; index2--) {
      if (mpz_cmp(s[index2], candidate) == 0) { 
	flag = TRUE; 
	break; 
      }
    }
    if (flag == TRUE) continue;
    
    mpz_mul(inverse, candidate, candidate);
    mpz_mod(inverse, inverse, n);
    if (mpz_invert(inverse, inverse, n) == 0) continue;

    mpz_set(s[index], candidate);
    mpz_set(i[index], inverse);

    index++;
  }

  // printf("\n\nPublic key:\n");
  
  // for (index = 0; index < K; index++) {
  //   gmp_printf("%i: %Zd\n", index, i[index]);
  // }
  // printf("Private key:\n");
  // for (index = 0; index < K; index++) {
  //   gmp_printf("%i: %Zd\n", index, s[index]);
  // }

  mpz_clear(candidate);
  mpz_clear(inverse);
  gmp_randclear(state);
}



/*
  Pick a random number and send the witness x (step 1 of the protocol)
*/
void witness(mpz_t x) {
  gmp_randstate_t state;

  mpz_init(r);
  mpz_init(x);

  gmp_randinit_lc_2exp_size(state, 128); 
  setrndseed();
  gmp_randseed(state, rndseed);
  
  mpz_urandomm(r, state, n);
  mpz_mul(x, r, r);
  mpz_mod(x, x, n);
  // gmp_printf("\nWitness     : %Zd\n\n", x);
  gmp_randclear(state);
}



/*
  Send the response y (step 3 of the protocol)
 */
void response(mpz_t y, unsigned int e) {
  int index;
  
  mpz_set(y, r);
  
  for (index = 0; index < K; index++) { 
    if (e & (0x1 << index)) mpz_mul(y, y, s[index]);
  }
  mpz_mod(y, y, n);

  // gmp_printf("Response    : %Zd\n\n", y);  
}
