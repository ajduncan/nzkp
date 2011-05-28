/* http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#simpleserver  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <gmp.h>
#include "zkp.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int zkp_connect(char *hostname)
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("ZKP Client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("ZKP Client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "ZKP Client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);

    freeaddrinfo(servinfo); // all done with this structure

    return sockfd;
}

int zkp_send(char *data, int sockfd) 
{
  if ((int) sizeof(data) > MAXDATASIZE) {
    printf("Warning, size of data (%lu) is greater than %d", sizeof(data), MAXDATASIZE);
    return(-1);
  }

  printf("Client sending: '%s'\n", data);
  
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
 
  // printf("Client: received '%s'\n", dest);
  return(1);
}

int zkp_disconnect(int sockfd) 
{
    close(sockfd);
    return 0;
}
