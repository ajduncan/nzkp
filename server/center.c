#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gmp.h>
#include "zkp.h"
 



/*
  Publish the modulus (a Blum integer which prime factors 
  are randomly chosen and 512 bits long)
*/
void publish_modulus() {
  mpz_t rand, tmpprime, tmp, prime1, prime2;
  gmp_randstate_t state;

  mpz_init(rand);
  mpz_init(tmpprime);
  mpz_init(tmp);
  mpz_init(prime1);
  mpz_init(prime2);
  mpz_init(n);

  gmp_randinit_lc_2exp_size(state, 128); 

  /* computes 1st prime */
  setrndseed();
  gmp_randseed(state, rndseed);
  mpz_rrandomb(rand, state, 512);
  while (1) {                          /* repeat until prime is of form 4r+3 */
    mpz_nextprime(tmpprime, rand);     
    mpz_sub_ui(tmp, tmpprime, 3);
    if (mpz_divisible_ui_p(tmp, 4)) break;
    mpz_set(rand, tmpprime);
  } 
  mpz_set(prime1, tmpprime);

  /* computes 2nd prime */
  setrndseed();
  gmp_randseed(state, rndseed);
  mpz_rrandomb(rand, state, 512);
  while (1) {                    
    mpz_nextprime(tmpprime, rand);     
    mpz_sub_ui(tmp, tmpprime, 3);
    if (mpz_divisible_ui_p(tmp, 4)) break;
    mpz_set(rand, tmpprime);
  } 
  mpz_set(prime2, tmpprime);

  /* computes modulus */
  mpz_mul(n, prime1, prime2);
  // gmp_printf("Publishing modulus: %Zd\n\n", n);

  mpz_clear(rand);
  mpz_clear(tmpprime);
  mpz_clear(tmp);
  mpz_clear(prime1);
  mpz_clear(prime2);
  gmp_randclear(state);
}












