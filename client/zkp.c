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

/***************************************************************************
 * Modifications to Daniele Raffo's ZKP implementation 
 * by Andy Duncan, 10 Oct 2009 - University of Tennessee at Chattanooga
 * 
 * Client/prover version
 ***************************************************************************/

int main(int argc, char **argv) {
  mpz_t x;            /* witness */
  unsigned int e;     /* random boolean vector (challenge) */  
  mpz_t y;            /* response */
  int proof;
  int sockfd;         /* socket file descriptor */
  int index;
  int j;

  char buf[MAXDATASIZE];

  printf("Feige-Fiat-Shamir ZKP implementation\n");

  if (argc != 2) {
    fprintf(stderr, "ZKP Server Usage: %s hostname\n", argv[0]);
    exit(1);
  } else {
    fastseed = TRUE;
    // printf("Warning: fastseed enabled, using a bad random seed value!\n");
  }

  printf("\n");

  mpz_init(rndseed);

  mpz_init(n);
  mpz_set_str(n, N, 10);
  
  /* Connect to server, T times */
  for (j=0; j < T; j++) {
    printf("Iteration %d: ", j);
    timestamp(stdout);
    printf("\n");

    mpz_init(x);
    mpz_init(y);
    e     = 0;
    proof = 0;
    index = 0;

    // gmp_printf("n = %Zd\n", n);
    // printf("Computing keys.\n");
    compute_keys();                /* Prover chooses public/private keys    */

    sockfd = zkp_connect(argv[1]); 

    if (sockfd == 2) {
      return(0);
    }

    /* Prover sends public key, i[K] */
    itoa(K, buf, 10);
    zkp_send(buf, sockfd); // 5 = K
    printf("Sent 1.\n");

    for (index = 0; index < K; index++) {
      // gmp_printf("Sending: %i %Zd\n", index, i[index]);  
      mpz_get_str(buf, 10, i[index]);
      zkp_send(buf, sockfd);
      sleep(1);
    }

    /* Prover sends the witness */
    witness(x);
    mpz_get_str(buf, 10, x);
    zkp_send(buf, sockfd);  
    /* end, Prover sends the witness */

    /* Verifier/server sends the challenge, we receive */
    zkp_recv(sockfd, buf);
    e = atoi(buf);
    printf("Client: got e: %i\n", e);
    /* End server sends the challenge */

    /* Send the response to the server */
    response(y, e);                /* Prover sends the response             */
    mpz_get_str(buf, 10, y);
    zkp_send(buf, sockfd);

    /* Verifier/server sends the authentication response */
    if (!zkp_recv(sockfd, buf)) {
      printf("Client ZKP: Error receiving client data.\n");
      return(0);
    }
    proof = atoi(buf);

    zkp_disconnect(sockfd);

    if (proof) {
      printf("Authentication successful!\n");
    } else {
      printf("Authentication failed!\n");
    }

    mpz_clear(x);
    mpz_clear(y);

    printf("Iteration %d finished at: ", j);
    timestamp(stdout);
    printf("\n\n");

  }

  mpz_clear(n);
  mpz_clear(rndseed);
  return (1);
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


