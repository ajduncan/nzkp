/* http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#simpleserver */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <gmp.h>
#include <time.h>
#include "zkp.h"

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

unsigned int zkp_protocol_challenge(int sockfd)
{
  unsigned int e;     /* random boolean vector (challenge) */
  char buf[MAXDATASIZE];

  e = challenge();    /* Verifier sends the challenge          */

  // printf("Server: got e = %d\n", e);

  itoa(e, buf, 10);

  // printf("Server: sending %s\n", buf);

  zkp_send(buf, sockfd);
  
  return(e);
}

int zkp_send(char *data, int sockfd)
{
  if ((int) sizeof(data) > MAXDATASIZE) {
    printf("Warning, size of data (%lu) is greater than %i", sizeof(data), MAXDATASIZE);
    return(-1);
  }

  // printf("Sending: %s\n", data);
 
  return send(sockfd, data, strlen(data), 0);
}

int zkp_recv(int sockfd, char* dest)
{
  int numbytes;
  if ((numbytes = recv(sockfd, dest, MAXDATASIZE-1, 0)) == -1) {
    perror("recv");
    return(0);
  }

  dest[numbytes] = '\0';

  printf("Server: received '%s'\n", dest);
  return(1);
}

int zkp_server(void) 
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    int numbytes;
    int proverk;
    int index;

    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    mpz_t x;            /* witness   */
    unsigned int e;     /* challenge */
    mpz_t y;            /* response  */
    int proof;

    // begin code

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // set n
    mpz_set_str(n, N, 10);
    // compute keys
    compute_keys();

    printf("Server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
          get_in_addr((struct sockaddr *)&their_addr),
          s, sizeof s);
        printf("ZKP Server: got connection from %s\n", s);
        printf("Connection Time: ");
        timestamp(stdout);
        printf("\n");

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            
            /* ZKP protocol goes here */
            mpz_init(x);
            mpz_init(y);

            // Get prover/client's K:
            if (zkp_recv(new_fd, buf) == -1) {
              perror("recv");
              exit(1);
            }

            printf("Server: received prover K = '%s'\n", buf);
            proverk =  atoi(buf);

            for (index = 0; index < K; index++) {
              if (zkp_recv(new_fd, buf) == -1) {
                perror("recv");
                exit(1);
              }
              mpz_set_str(i[index], buf, 10);
              // gmp_printf("Prover public key [%i]: %Zd\n", index, i[index]);
            }
            // end get public key

            // get x
            if (zkp_recv(new_fd, buf) == -1) {
                perror("recv");
                exit(1);
            }
            mpz_set_str(x, buf, 10);
            // printf("Server: received x = '%s'\n", buf);
            // end get x
           
            // send challenge
            // printf("Server: Sending challenge, e.\n");
            e = zkp_protocol_challenge(new_fd);      

            // get y
            if (zkp_recv(new_fd, buf) == -1) {
                perror("recv");
                exit(1);
            }
            mpz_set_str(y, buf, 10);
            // printf("Server: received y = '%s'\n", buf);
            // end get y

            // gmp_printf("x = %Zd\n", x);
            // gmp_printf("y = %Zd\n", y);
            // printf("e = %i\n\n", e);

            proof = verify(x, y, e);       /* Verifier verifies if response matches */

            if (proof) {
              printf("Server: Authentication successful.\n");
            } else {
              printf("Server: Authentication failed.\n");
            }

            itoa(proof, buf, 10);
            zkp_send(buf, new_fd);

            printf("Session ended at: ");
            timestamp(stdout);
            printf("\n\n");

            mpz_clear(x);
            mpz_clear(y);

            /* End ZKP protocol */

            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}
