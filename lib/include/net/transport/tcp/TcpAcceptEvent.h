/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace net {


		/**
		 * TCP connection accept event
		 */

		class TcpServerBase;

		struct TcpAcceptEvent : NetEventDescriptor {

			TcpServerBase& server;							///< server that raised this event
			TcpConnection *connection;					///< new connection reference
			bool accepted;


			/**
			 * Constructor
			 * @param The connection smart pointer
			 */

			TcpAcceptEvent(TcpServerBase&s,TcpConnection *c)
				: NetEventDescriptor(NetEventDescriptor::NetEventType::TCP_ACCEPT),
				  server(s),
				  connection(c),
				  accepted(false) {
			}


			/**
			 * The client MUST call this to accept the connection. If not then the server
			 * will delete the connection.
			 * @return the connection
			 */

			TcpConnection *acceptConnection() {
				accepted=true;
				return connection;
			}
		};
	}
}
