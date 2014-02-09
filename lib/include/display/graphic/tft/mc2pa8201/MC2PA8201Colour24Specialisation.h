/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/mc2pa8201/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Template class holding the specialisation of MC2PA8201Colour for 24-bit colours
     * @tparam TAccessMode The access mode class (e.g. FSMC)
     */

    template<class TAccessMode,class TPanelTraits>
    class MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits> {

      private:
        TAccessMode& _accessMode;

      protected:
        MC2PA8201Colour(TAccessMode& accessMode);

        constexpr uint16_t getInterfacePixelFormat() const;

        typedef uint32_t tCOLOUR;

        struct UnpackedColour {
          uint8_t r,g,b;
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

    template<class TAccessMode,class TPanelTraits>
    inline MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits>::MC2PA8201Colour(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for 16-bit colours
     * @return 16-bit interface pixel format register setting
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline uint16_t MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits>::getInterfacePixelFormat() const {
      return 0x77;
    }


    /**
     * Unpack the colour from rrggbb to the internal 8-8-8 format
     * 00000000RRRRRRRRGGGGGGGGBBBBBBBB ->
     * RRRRRRRR,GGGGGGGG,BBBBBBBB
     * @param src rrggbb
     * @param dest The unpacked colour structure
     */

    template<class TAccessMode,class TPanelTraits>
    inline void MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits>::unpackColour(tCOLOUR src,UnpackedColour& dest) const {
      dest.r=(src >> 16) & 0xff;
      dest.g=(src >> 8) & 0xff;
      dest.b=src & 0xff;
    }


    /**
     * Unpack the colour from components to the internal format
     * @param red
     * @param green
     * @param blue
     * @param dest
     */

    template<class TAccessMode,class TPanelTraits>
    inline void MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits>::unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const {
      dest.r=red;
      dest.g=green;
      dest.b=blue;
    }


    /**
     * Write a single pixel to the current output position.
     * Assumes that the caller has already issued the beginWriting() command.
     * @param cr The pixel to write
     */

    template<class TAccessMode,class TPanelTraits>
    inline void MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits>::writePixel(const UnpackedColour& cr) const {
      _accessMode.writeData(cr.r);
      _accessMode.writeData(cr.g);
      _accessMode.writeData(cr.b);
    }

    /**
     * Write the same colour pixel that we last wrote. This gives the access mode a chance to
     * optimise sequential pixel writes. The colour is provided for drivers that cannot optimise
     * and must fall back to a full write.
     * @param cr The pixel to write
     */

    template<class TAccessMode,class TPanelTraits>
    inline void MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits>::writePixelAgain(const UnpackedColour& cr) const {
      _accessMode.writeData(cr.r);
      _accessMode.writeData(cr.g);
      _accessMode.writeData(cr.b);
    }


    /**
     * Fill a block of pixels with the same colour. This operation will issue the
     * beginWriting() command for you.
     * @param numPixels how many
     * @param cr The unpacked colour to write
     */

    template<class TAccessMode,class TPanelTraits>
    inline void MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits>::fillPixels(uint32_t numPixels,const UnpackedColour& cr) const {

      uint8_t b,g,r;

      _accessMode.writeCommand(mc2pa8201::MEMORY_WRITE);

      b=cr.b;
      g=cr.g;
      r=cr.r;

      while(numPixels--) {
        _accessMode.writeData(r);
        _accessMode.writeData(g);
        _accessMode.writeData(b);
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

    template<class TAccessMode,class TPanelTraits>
    inline void MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits>::allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const {
      buffer=new uint8_t[numPixels*3];
      bytesPerPixel=3;
    }


    /**
     * Bulk-copy some pixels from the memory buffer to the LCD. The pixels must already be formatted ready
     * for transfer.
     * @param buffer The memory buffer
     * @param numPixels The number of pixels to transfer from the buffer
     */

    template<class TAccessMode,class TPanelTraits>
    inline void MC2PA8201Colour<COLOURS_24BIT,TAccessMode,TPanelTraits>::rawTransfer(const void *buffer,uint32_t numPixels) const {
      _accessMode.rawTransfer(buffer,numPixels*3);
    }
  }
}
