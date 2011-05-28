# Makefile for the FFS Zero-Knowledge Identification Scheme implementation

CC      = gcc
LIBS    = /usr/lib/libgmp.a

zkp: zkp.o center.o prover.o verifier.o clientzkp.o ts.o itoa.o
	$(CC) zkp.c ts.c itoa.c center.c prover.c verifier.c clientzkp.c -o zkp -lm $(LIBS) -lnsl -lsocket -L /usr/lib/happycoders

clean:
	clear; rm -f zkp *~ *.o core

