/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Simple wrapper for the circular buffer used for received data that ensures
     * IRQ-safe access to the methods.
     */

    class TcpReceiveBuffer {

      protected:
        volatile circular_buffer<uint8_t> _receiveBuffer;

      public:
        TcpReceiveBuffer(uint32_t size);

        void read(uint8_t *output,uint32_t size) volatile;
        void write(const uint8_t *input,uint32_t size) volatile;

        uint32_t availableToWrite() const volatile;
        uint32_t availableToRead() const volatile;
    };


    /**
     * Constructor
     * @param size the buffer size
     */

    inline TcpReceiveBuffer::TcpReceiveBuffer(uint32_t size)
      : _receiveBuffer(size) {
    }

    inline void TcpReceiveBuffer::read(uint8_t *output,uint32_t size) volatile {
      IrqSuspend suspender;
      _receiveBuffer.read(output,size);
    }


    inline void TcpReceiveBuffer::write(const uint8_t *input,uint32_t size) volatile {
      IrqSuspend suspender;
      _receiveBuffer.write(input,size);
    }

    inline uint32_t TcpReceiveBuffer::availableToWrite() const volatile {
      IrqSuspend suspend;
      return _receiveBuffer.availableToWrite();
    }

    inline uint32_t TcpReceiveBuffer::availableToRead() const volatile {
      IrqSuspend suspend;
      return _receiveBuffer.availableToRead();
    }
  }
}

