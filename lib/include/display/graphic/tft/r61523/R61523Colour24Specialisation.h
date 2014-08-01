/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Template class holding the specialisation of R61523Colour for 24-bit colours
     * @tparam TAccessMode The access mode class (e.g. FSMC)
     */

    template<class TAccessMode>
    class R61523Colour<COLOURS_24BIT,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        R61523Colour(TAccessMode& accessMode);

        void setColourDepth() const;

        typedef uint32_t tCOLOUR;

        struct UnpackedColour {
          uint16_t first,second;
        };

      public:
        void unpackColour(tCOLOUR src,UnpackedColour& dest) const;
        void unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const;

        void writePixel(const UnpackedColour& cr) const;
        void writePixelAgain(const UnpackedColour& cr) const;
        void fillPixels(uint32_t numPixels,const UnpackedColour& cr) const;

        void allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const;
        void rawTransfer(const void *data,uint32_t numPixels) const;
    };


    /**
     * Constructor
     */

    template<class TAccessMode>
    inline R61523Colour<COLOURS_24BIT,TAccessMode>::R61523Colour(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Set the colour depth to 24-bit
     */

    template<class TAccessMode>
    inline void R61523Colour<COLOURS_24BIT,TAccessMode>::setColourDepth() const {

      // set the pixel format to 24bpp

      _accessMode.writeCommand(r61523::SET_PIXEL_FORMAT,0x7);

      // now set DFM to 1 (2 transfers = 1 pixel) (this requires MCAP to be enabled)

      _accessMode.writeCommand(r61523::SET_FRAME_AND_INTERFACE);
      _accessMode.writeData(0x80);
      _accessMode.writeData(0x1 << 4);
    }


    /**
     * Unpack the colour from rrggbb to the internal format.
     *
     * 00000000RRRRRRRR+GGGGGGGGBBBBBBBB ->
     * 00000000BBBBBBBB,GGGGGGGGRRRRRRRR ->
     *
     * @param src rrggbb
     * @param dest The unpacked colour structure
     */

    template<class TAccessMode>
    inline void R61523Colour<COLOURS_24BIT,TAccessMode>::unpackColour(tCOLOUR src,UnpackedColour& dest) const {
      dest.first=src & 0xff;
      dest.second=(src >> 16) | (src & 0xFF00);
    }


    /**
     * Unpack the colour from components to the internal format
     * @param red
     * @param green
     * @param blue
     * @param dest
     */

    template<class TAccessMode>
    inline void R61523Colour<COLOURS_24BIT,TAccessMode>::unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const {

      dest.first=blue;
      dest.second=(static_cast<uint16_t>(green) << 8) | red;
    }


    /**
     * Write a single pixel to the current output position.
     * Assumes that the caller has already issued the beginWriting() command.
     * @param cr The pixel to write
     */

    template<class TAccessMode>
    inline void R61523Colour<COLOURS_24BIT,TAccessMode>::writePixel(const UnpackedColour& cr) const {
      _accessMode.writeData(cr.first);
      _accessMode.writeData(cr.second);
    }


    /**
     * Write the same colour pixel that we last wrote. This gives the access mode a chance to
     * optimise sequential pixel writes. The colour is provided for drivers that cannot optimise
     * and must fall back to a full write.
     * @param cr The same pixel to write again
     */

    template<class TAccessMode>
    inline void R61523Colour<COLOURS_24BIT,TAccessMode>::writePixelAgain(const UnpackedColour& cr) const {
      _accessMode.writeData(cr.first);
      _accessMode.writeData(cr.second);
    }


    /**
     * Fill a block of pixels with the same colour. This operation will issue the
     * beginWriting() command for you.
     * @param numPixels how many
     * @param cr The unpacked colour to write
     */

    template<class TAccessMode>
    inline void R61523Colour<COLOURS_24BIT,TAccessMode>::fillPixels(uint32_t numPixels,const UnpackedColour& cr) const {

      uint16_t first,second;

      _accessMode.writeCommand(r61523::MEMORY_WRITE);

      first=cr.first;
      second=cr.second;

      while(numPixels--) {
        _accessMode.writeData(first);
        _accessMode.writeData(second);
      }
    }


    /**
     * Allocate a buffer for pixel data. You supply the number of pixels and this allocates the buffer as a uint8_t[].
     * Allocated buffers should be freed with delete[]
     *
     * @param numPixels The number of pixels to allocate
     * @param buffer The output buffer
     * @param bytesPerPixel Output the number of bytes per pixel
     */

    template<class TAccessMode>
    inline void R61523Colour<COLOURS_24BIT,TAccessMode>::allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const {
      buffer=new uint8_t[numPixels*4];
      bytesPerPixel=4;
    }


    /**
     * Bulk-copy some pixels from the memory buffer to the LCD. The pixels must already be formatted ready
     * for transfer.
     * @param buffer The memory buffer
     * @param numPixels The number of pixels to transfer from the buffer
     */

    template<class TAccessMode>
    inline void R61523Colour<COLOURS_24BIT,TAccessMode>::rawTransfer(const void *buffer,uint32_t numPixels) const {
      _accessMode.rawTransfer(buffer,numPixels*2);
    }
  }
}
