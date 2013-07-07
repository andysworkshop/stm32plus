/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


using namespace stm32plus;
using namespace stm32plus::net;


/**
 * Types that define the network stack
 */

typedef PhysicalLayer<DP83848C> MyPhysicalLayer;
typedef DatalinkLayer<MyPhysicalLayer,DefaultRmiiInterface,Mac> MyDatalinkLayer;
typedef NetworkLayer<MyDatalinkLayer,DefaultIp,Arp> MyNetworkLayer;
typedef TransportLayer<MyNetworkLayer,Icmp,Udp,Tcp> MyTransportLayer;
typedef ApplicationLayer<MyTransportLayer,DhcpClient> MyApplicationLayer;
typedef NetworkStack<MyApplicationLayer> MyNetworkStack;


/**
 * Structure to hold a user specification
 */

struct FtpUser {

	std::string name;
	std::string passwordHash;
	std::string passwordSalt;
	bool writeAccess;

	static FtpUser createAnonymous() {

		FtpUser user;

		user.name="anonymous";
		user.writeAccess=false;

		return user;
	}
};


/**
 * Structure to hold FTP parameters that need to be passed to the connections
 */

struct FtpParameters {

	// parameters read from the card

	std::string serverGreeting;				/// no default: you have to set one
	std::string serverGoodbye;				/// no default: you have to set one
	std::string rootDirectory;				/// the root directory for this server: default is ""
	bool anonymousPermitted;					/// default is false
	uint32_t idleTimeout;							/// default is 120 seconds
	std::vector<FtpUser> users;				/// user specifications, no defaults. anonymous always gets entry zero if enabled.

	// internal references created during server startup

	FileSystem *fs;
	Tcp<MyNetworkLayer> *tcpImpl;


	/**
	 * Constructor
	 */

	FtpParameters() {
		anonymousPermitted=false;
		idleTimeout=120;
	}
};
