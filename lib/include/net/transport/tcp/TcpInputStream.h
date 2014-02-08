/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * @brief An input stream for reading from a TCP connection. The stream is unbuffered.
     */

    class TcpInputStream : public InputStream {

      protected:
        TcpConnection& _conn;

      public:
        TcpInputStream(TcpConnection& conn);

        // overrides from InputStream

        virtual int16_t read() override;
        virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
        virtual bool skip(uint32_t howMuch) override;
        virtual bool available() override;
        virtual bool reset() override;
        virtual bool close() override;
    };


    /**
     * Constructor
     * @param conn The connection to read from
     */

    inline TcpInputStream::TcpInputStream(TcpConnection& conn)
      : _conn(conn) {
    }


    /**
     * Read a single byte
     * @return the byte, E_STREAM_ERROR or E_END_OF_STREAM
     */

    inline int16_t TcpInputStream::read() {

      uint8_t c;
      uint32_t actuallyRead;

      if(!read(&c,1,actuallyRead))
        return E_STREAM_ERROR;

      if(actuallyRead==1)
        return c;

      return E_END_OF_STREAM;
    }


    /**
     * Read a block of bytes. This call will block until some bytes are available.
     * @param buffer Where to read out to
     * @param size The maximum number to read
     * @param actuallyRead How many were read
     * @return true if some bytes were read. Zero bytes are a success condition that indicate the end of the stream
     */

    inline bool TcpInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {
      return _conn.receive(buffer,size,actuallyRead);
    }


    /**
     * Cannot skip
     * @return false
     */

    inline bool TcpInputStream::skip(uint32_t /* howMuch */) {
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_TCP_INPUT_STREAM,E_OPERATION_NOT_SUPPORTED);
    }


    /**
     * Check if there are byte(s) available for immediate consumption
     * @return true if bytes are available
     */

    inline bool TcpInputStream::available() {
      return _conn.getDataAvailable()>0;
    }


    /**
     * Cannot reset to start
     * @return false
     */

    inline bool TcpInputStream::reset() {
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_TCP_INPUT_STREAM,E_OPERATION_NOT_SUPPORTED);
    }


    /**
     * Cannot close, but it's not an error either. The underlying connection must be
     * deleted to close the connection
     * @return true
     */

    inline bool TcpInputStream::close() {
      return true;
    }
  }
}
