/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * An input stream for writing to a TCP connection. The stream is unbuffered which makes
     * it quite inefficient for the network if small data items are repeatedly pushed to it.
     */

    class TcpOutputStream : public OutputStream {

      public:
        enum {
          E_CONNECTION_CLOSED = 1      ///< The connection was closed while we were writing to it
        };

      protected:
        TcpConnection& _conn;

      public:
        TcpOutputStream(TcpConnection& conn);

        // overrides from OutputStream

        virtual bool write(uint8_t c) override;
        virtual bool write(const void *buffer,uint32_t size) override;
        virtual bool flush() override;
        virtual bool close() override;
    };


    /**
     * Constructor
     */

    inline TcpOutputStream::TcpOutputStream(TcpConnection& conn)
      : _conn(conn) {
    }


    /**
     * Write a single byte. Not efficient!
     * @param c The byte to write
     * @return true if it worked
     */

    inline bool TcpOutputStream::write(uint8_t c) {
      return write(&c,1);
    }


    /**
     * Write a stream of bytes
     * @param buffer The buffer address
     * @param size The number of bytes to write
     */

    inline bool TcpOutputStream::write(const void *buffer,uint32_t size) {

      const uint8_t *ptr;
      uint32_t actuallySent;

      ptr=reinterpret_cast<const uint8_t *>(buffer);
      while(size) {

        // try to send everything

        if(!_conn.send(ptr,size,actuallySent))
          return false;

        // check for connection closed

        if(actuallySent==0)
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_TCP_OUTPUT_STREAM,E_CONNECTION_CLOSED);

        // something went out, decrement and loop if all did not go

        size-=actuallySent;
        ptr+=actuallySent;
      }

      return true;
    }


    /**
     * Cannot flush, not an error either
     * @return true
     */

    inline bool TcpOutputStream::flush() {
      return true;
    }


    /**
     * Cannot close, not an error either. The underlying connection must be deleted to close it
     * @return true
     */

    inline bool TcpOutputStream::close() {
      return true;
    }
  }
}
