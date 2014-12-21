# Makefile for the FFS Zero-Knowledge Identification Scheme implementation

CC      = gcc
PREFIX  = /opt/nzkp
BINDIR  = $(PREFIX)/bin
LIBS    = /usr/local/lib/libgmp.a


all: nzkp nzkpd

nzkp: client
	@echo "Building client."
	cd client; make CC=${CC} LIBS=${LIBS}

nzkpd: server
	@echo "Building server."
	cd server; make CC=${CC} LIBS=${LIBS}

install-nzkp:
	@echo "Installing nzkp client to ${PREFIX}/bin."
	install -D client/nzkp $(BINDIR)/nzkp

install-nzkpd:
	@echo "Installing nzkp server to ${PREFIX}/bin."
	install -D server/nzkpd $(BINDIR)/nzkpd

uninstall-nzkp:
	@echo "Uninstalling nzkp client."
	-rm $(BINDIR)/nzkp

uninstall-nzkpd:
	@echo "Uninstalling nzkp server."
	-rm $(BINDIR)/nzkpd

install: install-nzkp install-nzkpd

uninstall: uninstall-nzkp uninstall-nzkpd

clean:
	@echo "Cleaning up."
	cd client; make clean
	cd server; make clean
