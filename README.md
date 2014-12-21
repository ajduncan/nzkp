# nzkp #

This is the nzkp client and server software derived from Daniele Raffo's Feige-Fiat-Shamir zero-knowledge
proof identity protocol.

Original code is located here:  http://perso.crans.org/~raffo/projects.php.

## Building ##

    Make sure you have dependencies: libgmp, m4, and libsocket.

    $ make

## Install ##

    $ sudo make install

Which will install the nzkp client to: /opt/nzkp/bin/nzkp
and the nzkp server to: /opt/nzkp/bin/nzkpd

    To uninstall:

    $ sudo make uninstall

## Vagrant ##

Automated runs can be performed by simply running:

    $ vagrant up
