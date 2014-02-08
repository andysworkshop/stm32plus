/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Non-template base class for the FTP server connection class
     */

    class FtpServerConnectionBase : public TcpConnection {

      public:

        /**
         * Parameters for this class
         */

        struct Parameters : TcpConnection::Parameters {

          uint16_t ftp_maxRequestLineLength;            ///< size includes the verb, and all parameters. Default is 200
          uint16_t ftp_outputStreamBufferMaxSize;       ///< buffer size of the stream-of-streams class. Default is 256
          uint16_t ftp_dataConnectionSendBufferSize;    ///< data connection send buffer size. Default is 2920 (2*MTU for ethernet)

          /**
           * Constructor
           */

          Parameters() {
            ftp_maxRequestLineLength=200;
            ftp_outputStreamBufferMaxSize=256;
            ftp_dataConnectionSendBufferSize=2918;
          }
        };

      protected:
        void freeDataConnectionServer();
        void freeDataConnection();

      protected:
        const Parameters& _params;        ///< reference to the parameters class

        TcpServer<FtpServerDataConnection,FtpServerConnectionBase> *_dataConnectionServer;
        FtpServerDataConnection *_dataConnection;

        uint16_t _dataConnectionPortNumber;
        TcpTextLineReceiver _commandReceiver;
        TcpOutputStreamOfStreams _outputStreams;
        FtpServerAuthenticationState _authenticationState;
        std::string _user;
        uint32_t _sendStartPosition;
        uint32_t _lastActiveTime;

      public:
        FtpServerConnectionBase(const Parameters& params);
        ~FtpServerConnectionBase();

        void clearDataConnection();       ///< this is a callback for the data connection server to clear itself
        uint16_t getDataConnectionSendBufferSize() const;
        void updateLastActiveTime();
    };


    /**
     * Constructor
     * @param params The parameters class
     */

    inline FtpServerConnectionBase::FtpServerConnectionBase(const Parameters& params)
      : TcpConnection(params),
        _params(params),
        _dataConnectionServer(nullptr),
        _dataConnection(nullptr),
        _commandReceiver(params.ftp_maxRequestLineLength),
        _outputStreams(*this,params.ftp_outputStreamBufferMaxSize),
        _authenticationState(FtpServerAuthenticationState::STARTING),
        _sendStartPosition(0) {
    }


    /**
     * Destructor, clean up data connection pointer
     */

    inline FtpServerConnectionBase::~FtpServerConnectionBase() {

      // this must be done first

      freeDataConnectionServer();

      // then the connection

      freeDataConnection();
    }


    /**
     * Free the data connection server
     */

    inline void FtpServerConnectionBase::freeDataConnectionServer() {

      if(_dataConnectionServer) {
        delete _dataConnectionServer;
        _dataConnectionServer=nullptr;
      }
    }


    /**
     * Free the data connection
     */

    inline void FtpServerConnectionBase::freeDataConnection() {

      if(_dataConnection)
        delete _dataConnection;   // this will cause a callback to clear the pointer
    }


    /**
     * Clear our reference to the data connection that has now been deleted
     */

    inline void FtpServerConnectionBase::clearDataConnection() {
      _dataConnection=nullptr;
    }


    /**
     * Get the data connection send buffer size
     */

    inline uint16_t FtpServerConnectionBase::getDataConnectionSendBufferSize() const {
      return _params.ftp_dataConnectionSendBufferSize;
    }


    /**
     * Update the last active time
     */

    inline void FtpServerConnectionBase::updateLastActiveTime() {
      _lastActiveTime=MillisecondTimer::millis();
    }
  }
}
