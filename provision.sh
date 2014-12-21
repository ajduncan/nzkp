#!/bin/sh

echo "
########################################################################
           _          
 _ __  ___| | ___ __  
| '_ \|_  / |/ / '_ \ 
| | | |/ /|   <| |_) |
|_| |_/___|_|\_\ .__/ 
               |_|    

Andrew Duncan <ajduncan@gmail.com>

########################################################################
"


# Update and upgrade
echo "Updating system..."
apt-get update > /dev/null 2>&1
echo "Upgrading system packages..."
apt-get -y upgrade > /dev/null 2>&1

# Install system dependencies.
echo "Installing system dependencies: libgmp, m4, libsocket"
apt-get -y install libgmp3-dev happycoders-libsocket-dev m4 > /dev/null 2>&1

# Build libgmp
if [ ! -d "/home/vagrant/libgmp" ];
then
	mkdir /home/vagrant/libgmp
	cd /home/vagrant/libgmp
	echo "Fetching libgmp from gmplib.org."
	wget https://ftp.gnu.org/gnu/gmp/gmp-6.0.0a.tar.bz2
	tar xjvf gmp-6.0.0a.tar.bz2 > /dev/null 2>&1
	cd gmp-6.0.0
	echo "Building libgmp."
	./configure; make; make install
	chown -R vagrant:vagrant /home/vagrant/libgmp
fi

if [ ! -d "/home/vagrant/nzkp" ];
then
	mkdir /home/vagrant/nzkp
fi

# build zkp client
echo "Building nzkp client and server."
cd /vagrant
make
make install
make clean

# Remove previous timestamp-started file.
if [ -f /home/vagrant/started ];
then
	echo "Clearing previous started file."
	rm /home/vagrant/started
fi

# Create started file with timestamp.
touch /home/vagrant/started
echo "Timestamp file created under /home/vagrant/started"

if [ -d /home/vagrant/nzkp ];
then
	echo "
########################################################################
Running experiment...

Just a stub for now.
"

echo "
Experiment complete.
########################################################################
"

fi
