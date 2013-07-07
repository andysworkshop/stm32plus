/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace net {


		/**
		 * subclass TCP connection so that we can tune the buffer parameters
		 */

		class MyTcpClientConnection : public TcpClientConnection {

			public:
				MyTcpClientConnection();
		};


		/**
		 * Constructor
		 */

		inline MyTcpClientConnection::MyTcpClientConnection() {

			// the TCP connection has taken a reference to the _params Parameters
			// class but it has not yet initialised itself from those parameters
			// therefore we are able to tune them for our needs

			// increase receive buffer size to 3xMSL for better throughput

			_params.tcp_receiveBufferSize=4380;
		}
	}
}
