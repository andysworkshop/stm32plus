/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/Size.h"


namespace stm32plus {
  namespace display {

    /**
     * @brief Class for frame buffers.
     *
     * A frame buffer contains raw pixels that can be directly transferred to the display
     */

    template<typename T>
    class FrameBuffer {

      protected:
        T *_buffer;
        uint32_t *_bitbandAddress;

        Size _pixelDimensions;
        bool _preAllocated;

      public:

        /**
         * Constructor
         * @param pixelDimensions (w,h) in pixels
         * @param typeCount Number of types in this frame buffer
         * @param preAllocated pointer to buffer it was allocated outside here
         */

        FrameBuffer(const Size& pixelDimensions,const uint32_t typeCount,T *preAllocated=nullptr)
          : _buffer(preAllocated),
            _pixelDimensions(pixelDimensions) {

          // allocate an internal buffer

          if(_buffer==nullptr) {
            _buffer=new T[typeCount];
            _preAllocated=false;
          }
          else
            _preAllocated=true;

          _bitbandAddress=reinterpret_cast<uint32_t *>(SRAM_BB_BASE | ((reinterpret_cast<uint32_t>(_buffer)-SRAM_BASE) << 5));
        }


        /**
         * Destructor
         */

        ~FrameBuffer() {
          if(!_preAllocated)
            delete [] _buffer;
        }

        /**
         * Get the pixel dimensions of the frame buffer (width, height).
         * @return a Size structure
         */

        const Size& getSize() const {
          return _pixelDimensions;
        }


        /**
         * Return a reference to a value in the FB
         * @param index The index into the array
         * @return The value reference
         */

        T& operator[](uint32_t index) {
          return _buffer[index];
        }


        /**
         * Get the base buffer address
         * @return The buffer address
         */

        T* getBuffer() {
          return _buffer;
        }


        /**
         * Get the address in the bitband region of the framebuffer
         * @return The bitband address
         */

        uint32_t *getBitbandAddress() {
          return _bitbandAddress;
        }
    };
  }
}
