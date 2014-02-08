/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Plumbing class to connect together an input and output stream.
   *
   * This class connects an input stream to an output stream. Data is read from the input
   * stream and written to the output stream
   */

  class ConnectedInputOutputStream {
    protected:
      InputStream& _inputStream;
      OutputStream& _outputStream;

    public:
      ConnectedInputOutputStream(InputStream& is,OutputStream& os);
      virtual ~ConnectedInputOutputStream() {
      }

      bool readWrite(uint32_t size,uint32_t& actuallyWritten);
      bool readWrite();
  };
}
