/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Simple intermediate class to manage holding the TcpConnection parameters
     * instance. Client connections are unlikely to need their own subclass of the
     * parameters and can customise the members of the default instance declared here.
     */

    class TcpClientConnection : public TcpConnection {

      protected:
        TcpConnection::Parameters _params;

      public:
        TcpClientConnection();
    };


    /**
     * Constructor
     */

    inline TcpClientConnection::TcpClientConnection()
      : TcpConnection(_params) {
    }
  }
}

