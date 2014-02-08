/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    class FtpServerConnectionBase;

    /**
     * Data connection for the FTP server. There can be only one data connection at any one time
     */

    class FtpServerDataConnection : public TcpConnection {

      public:

        /**
         * Possible data transfer directions
         */

        enum class Direction : uint8_t {
          NOT_STARTED,
          UPLOAD,
          DOWNLOAD
        };


        /**
         * General constants
         */

        enum {
          UPLOAD_TRANSFER_BUFFER_SIZE = 512   // amount read from receive buffer and written to output stream (512b matches SD Card sector size)
        };


        /**
         * Data connection parameters
         */

        struct Parameters : TcpConnection::Parameters {

          /**
           * Constructor
           */

          Parameters() {
            tcp_receiveBufferSize=1024;       // for uploads, increase the receive buffer to 1Kb
          }
        };

      protected:
        FtpServerConnectionBase *_commandConnection;
        TcpOutputStreamOfStreams _outputStreams;
        Direction _direction;
        scoped_ptr<OutputStream> _uploadStream;

        enum class State : uint8_t {
          NOT_STARTED,
          RUNNING
        } _state;

      public:
        FtpServerDataConnection(const Parameters& params,FtpServerConnectionBase *serverbase);
        ~FtpServerDataConnection();

        bool handleWrite();
        bool handleRead();

        bool flush();
        bool finished() const;

        void addString(const char *str);
        void addStream(InputStream *stream,bool owned);
        void setUploadStream(OutputStream *stream);
        void setDirection(Direction dir);
        Direction getDirection() const;
    };


    /**
     * Set the direction
     * @param dir
     */

    inline void FtpServerDataConnection::setDirection(Direction dir) {
      _direction=dir;
    }


    /**
     * Get the direction
     * @return
     */

    inline FtpServerDataConnection::Direction FtpServerDataConnection::getDirection() const {
      return _direction;
    }


    /**
     * Set a new upload stream
     * @param stream The upload stream
     */

    inline void FtpServerDataConnection::setUploadStream(OutputStream *stream) {
      _uploadStream.reset(stream);
    }
  }
}
