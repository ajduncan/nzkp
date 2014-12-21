all: client

client: zkp.o center.o prover.o verifier.o clientzkp.o ts.o itoa.o
	${CC} zkp.c ts.c itoa.c center.c prover.c verifier.c clientzkp.c -o nzkp -lm ${LIBS} -lnsl -lsocket -L /usr/lib/happycoders

clean:
	@rm -f nzkp *~ *.o core

