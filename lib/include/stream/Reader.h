/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Abstract class defining the methods that a reader implementation
   * must implement.
   */

  class Reader : public InputStream {

    public:

      /**
       * Read a line of text from the stream. A line is delimited by an LF or
       * a CRLF sequence or the end of file. If the line is longer than maxLength
       * then the line is truncated. The returned line will be \0 terminated meaning
       * that maxLength includes the \0 character.
       * @param buffer Where to store the line
       * @param maxLength The maximum number of characters to read
       * @return true if it worked
       */

      virtual bool readLine(char *buffer,uint32_t maxLength)=0;
  };
}
