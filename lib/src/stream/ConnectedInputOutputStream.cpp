/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/stream.h"


namespace stm32plus {

  /**
   * Constructor.
   * @param[in] is The input stream where data comes from.
   * @param[in] os The output stream where data goes to.
   */

  ConnectedInputOutputStream::ConnectedInputOutputStream(InputStream& is,OutputStream& os) :
    _inputStream(is), _outputStream(os) {
  }

  /**
   * Read/write while there is data. This method will continually read the input stream and write the data
   * to the output stream until the end of the input stream is reached.
   * @return false if an error occurred during the read/write operation.
   */

  bool ConnectedInputOutputStream::readWrite() {

    int16_t c;

    while((c=_inputStream.read()) > 0)
      if(!_outputStream.write(c))
        return false;

    _outputStream.flush();

    return c == InputStream::E_END_OF_STREAM ? true : false;
  }


  /**
   * Read/write up to the size specified by the user. Up to size_ bytes are read from the input stream
   * and written to the output stream. In the event of an error or end-of-stream actuallyWritten_ will
   * hold the number of bytes received.
   * @param[in] size_ The maximum amount of data to to read and write.
   * @param[out] actuallyWritten_ The amount of data written which may be less than size_.
   * @return false if it failed.
   */

  bool ConnectedInputOutputStream::readWrite(uint32_t size,uint32_t& actuallyWritten) {

    char buffer[32];
    uint32_t actuallyRead,count;

    actuallyWritten=0;

    while(size!=0) {

    // read some data

      count=size<sizeof(buffer) ? size : sizeof(buffer);

      if(!_inputStream.read(buffer,count,actuallyRead))
        return false;

    // write it

      if(actuallyRead!=0) {

        if(!_outputStream.write(buffer,actuallyRead))
          return false;

        actuallyWritten+=actuallyRead;
        size-=actuallyRead;

        _outputStream.flush();
      }
    }

    return true;
  }
}
