/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Many application layer IETF protocols are text based and rely on
     * being able to process data a line at a time. This class simplifies receiving
     * data from a TcpConnection a line at a time.
     *
     * Data is read into a pre-sized std::string class a character at a time until
     * an LF is hit. Any CR characters are discarded. The trailing LF is not included
     * in the string.
     */

    class TcpTextLineReceiver {

      public:

        /**
         * Error codes
         */

        enum {
          E_RESET_REQUIRED = 1      ///< you called add() but you haven't reset() since the last line was ready()
        };

      protected:
        std::string _line;
        uint16_t _maxLength;
        bool _ready;

      public:
        TcpTextLineReceiver(uint16_t maxLength);

        bool add(TcpConnection& conn);
        bool ready() const;
        void reset();
        uint16_t length() const;

        operator const std::string&() const;
        const std::string& str() const;
    };


    /**
     * Constructor
     * @param maxLength The longest line that we can receive. Longer lines are truncated.
     */

    inline TcpTextLineReceiver::TcpTextLineReceiver(uint16_t maxLength)
      : _maxLength(maxLength) {
      reset();
    }


    /**
     * Add some more data from this TCP connection. Only data up to and including the
     * first LF will be consumed. This call will not block. It will check for data available
     * in the TCP receive buffer before calling receive(). If data is not available then it
     * will return 'true' and expect to be called back later when you have been signalled
     * that data is available.
     * @param conn The TCP connection to read from
     * @return false if the underlying call to the connection read() fails.
     */

    inline bool TcpTextLineReceiver::add(TcpConnection& conn) {

      char c;
      uint32_t actuallyRead;

      // cannot procede if you haven't called reset on the previous line

      if(_ready)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_TCP_TEXT_LINE_RECEIVER,E_RESET_REQUIRED);

      // loop while data is ready to read

      while(conn.getDataAvailable()) {

        // try to read a character

        if(!conn.receive(&c,1,actuallyRead,1) || actuallyRead!=1)
          return false;

        // if LF then done, otherwise store non-CR char

        if(c=='\n') {
          _ready=true;
          break;
        }
        else if(c!='\r' && _line.length()<_maxLength)
          _line+=c;
      }

      return true;
    }


    /**
     * Check if the line is ready
     * @return true if a line is ready to be consumed
     */

    inline bool TcpTextLineReceiver::ready() const {
      return _ready;
    }


    /**
     * Reset the class. After calling this you may call add() again to start receiving
     * a new line
     */

    inline void TcpTextLineReceiver::reset() {

      // clear the line, cancel the ready flag

      _line.clear();
      _ready=false;
    }


    /**
     * Cast to a std::string const reference
     */

    inline TcpTextLineReceiver::operator const std::string&() const {
      return _line;
    }


    /**
     * Method to return as a string reference
     * @return The string
     */

    inline const std::string& TcpTextLineReceiver::str() const {
      return _line;
    }


    /**
     * Get the string length
     * @return The length of the string
     */

    inline uint16_t TcpTextLineReceiver::length() const {
      return _line.length();
    }
  }
}
