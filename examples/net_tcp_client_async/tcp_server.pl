#!/usr/bin/perl

use IO::Socket::INET;
use warnings;
use strict;


# flush after every write

$| = 1;

my ($socket,$clientsocket,$peeraddress,$peerport,$line,$reversed);

# create the server socket

$socket=new IO::Socket::INET (
		LocalPort => '12345',
		Proto => 'tcp',
		Listen => SOMAXCONN,
		Reuse => 1
		) or die "Failed to create server socket: $!\n";

print "SERVER Waiting for client connection on port 12345\n";


while(1) {

		# waiting for new client connection.
		
		$clientsocket=$socket->accept();

		# get the host and port number of newly connected client.

		$peeraddress=$clientsocket->peerhost();
		$peerport=$clientsocket->peerport();

		print "Accepted connection from ${peeraddress}:${peerport}\n";

		# read line, strip CR, strip LF, reverse

		$line=<$clientsocket>;
		if(defined($line)) {
				$line=~s/\r//g;
				$line=~s/\n//g;
				$reversed=scalar reverse("$line");

				print "Received '${line}', sending '${reversed}'\n";

				print $clientsocket "${reversed}\n";
		}
		else {
				print "connection closed by remote client\n";
		}
		$clientsocket->close();
}

