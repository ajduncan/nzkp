all: server

server: zkp.o ts.o center.o prover.o verifier.o serverzkp.o itoa.o
	${CC} zkp.c ts.c itoa.c center.c prover.c verifier.c serverzkp.c -o nzkpd -lm ${LIBS} -lnsl -lsocket -L /usr/lib/happycoders

clean:
	@rm -f nzkpd *~ *.o core

