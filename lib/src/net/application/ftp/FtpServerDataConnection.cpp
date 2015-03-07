/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"

#if defined(STM32PLUS_F4_HAS_MAC) || defined(STM32PLUS_F1_CL_E)

#include "config/net_ftp.h"
#include "net/application/ftp/FtpServerConnectionBase.h"


namespace stm32plus {
  namespace net {


    /**
     * Constructor. As this is the data connection and most activity is likely to be downloading
     * we will boost the output streams buffer size to 2*MTU so that the delayed ACK avoidance
     * algorithm that sends size/2 bytes-per send will result in MTU bytes being sent per segment.
     * Typically this will result in a buffer size of 1460*2 = 2920 bytes per data connection.
     * @param params The TCP parameters
     * @param serverbase The command connection that we belong to
     */

    FtpServerDataConnection::FtpServerDataConnection(const Parameters& params,FtpServerConnectionBase *serverbase)
      : TcpConnection(params),
        _commandConnection(serverbase),
        _outputStreams(*this,serverbase->getDataConnectionSendBufferSize()),
        _direction(Direction::NOT_STARTED),
        _state(State::NOT_STARTED) {
    }


    /**
     * Destructor
     */

    FtpServerDataConnection::~FtpServerDataConnection() {
      _commandConnection->clearDataConnection();
    }


    /**
     * Handle the possibility of a write
     * @return true if there were no errors
     */

    bool FtpServerDataConnection::handleWrite() {

      uint32_t actuallySent;

      // cannot write if either end is closed

      if(isLocalEndClosed() || isRemoteEndClosed())
        return false;

      // write to the connection

      if(_outputStreams.canWriteToConnection()) {

        if(!_outputStreams.writeDataToConnection(actuallySent))
          return false;

        if(actuallySent)
          _commandConnection->updateLastActiveTime();
      }

      return true;
    }


    /**
     * Handle the possibility of a read. Read out the received data in chunks until
     * we've got it all. A chunk size is 512b which matches the SD card sector size
     * though it's likely we'll fragment somewhere during the transfer and end up straddling
     * sectors.
     * @return true if there were no errors (being closed is not an error)
     */

    bool FtpServerDataConnection::handleRead() {

      uint32_t actuallyRead,totalRead;
      uint8_t transferBuffer[UPLOAD_TRANSFER_BUFFER_SIZE];      // default is 512 bytes

      // transfer out all data

      totalRead=0;
      while(getDataAvailable()>0) {

        // receive any data available

        if(!receive(transferBuffer,UPLOAD_TRANSFER_BUFFER_SIZE,actuallyRead))
          return false;

        // write back to the stream

        if(actuallyRead>0 && !_uploadStream->write(transferBuffer,actuallyRead))
          return false;

        totalRead+=actuallyRead;
      }

      if(totalRead)
        _commandConnection->updateLastActiveTime();

      return true;
    }


    /**
     * Flush all pending data when reading
     * @return true if it worked
     */

    bool FtpServerDataConnection::flush() {

      if(_direction==Direction::DOWNLOAD) {

        while(!_outputStreams.completed())
          if(!handleWrite())
            return false;
      }

      return true;
    }


    /**
     * Check if the transfer has completed. There'll be an idle period just after the
     * connection is created so it's only finished when it's started running (streams
     * have been added) and there are no streams remaining.
     */

    bool FtpServerDataConnection::finished() const {

      // if downloading then the all streams gone == finshed
      // if uploading then the remote end closing == finished

      if(_direction==Direction::DOWNLOAD)
        return _state==State::RUNNING && _outputStreams.completed();
      else if(_direction==Direction::UPLOAD)
        return getDataAvailable()==0 && isRemoteEndClosed();
      else
        return false;
    }


    /**
     * Add a new string to the output streams
     */

    void FtpServerDataConnection::addString(const char *str) {

      std::string *newstr;

      newstr=new std::string(str);
      (*newstr)+="\r\n";

      addStream(
        new StlStringInputStream(
            newstr,true),
            true
          );
    }


    /**
     * Add a stream to the data connection
     * @param stream The stream to add
     * @param owned true if this stream should be automatically deleted
     */

    void FtpServerDataConnection::addStream(InputStream *stream,bool owned) {
      _outputStreams.addStream(stream,owned);
      _state=State::RUNNING;
    }
  }
}


#endif
