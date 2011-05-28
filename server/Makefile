# Makefile for the FFS Zero-Knowledge Identification Scheme implementation

CC      = gcc
LIBS    = /usr/lib/libgmp.a

zkp: zkp.o ts.o center.o prover.o verifier.o serverzkp.o itoa.o
	$(CC) zkp.c ts.c itoa.c center.c prover.c verifier.c serverzkp.c -o zkp -lm $(LIBS) -lnsl -lsocket -L /usr/lib/happycoders

clean:
	clear; rm -f zkp *~ *.o core

