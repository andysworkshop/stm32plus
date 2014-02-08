/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/hx8352a/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Template class holding the specialisation of HX8352AColour for 18-bit colours
     * @tparam TAccessMode The access mode class (e.g. FSMC)
     */

    template<class TAccessMode,class TPanelTraits>
    class HX8352AColour<COLOURS_18BIT,TAccessMode,TPanelTraits> {

      private:
        TAccessMode& _accessMode;

      protected:
        HX8352AColour(TAccessMode& accessMode);

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

    template<class TAccessMode,class TPanelTraits>
    inline HX8352AColour<COLOURS_18BIT,TAccessMode,TPanelTraits>::HX8352AColour(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Unpack the colour from #rrggbb to the internal 6-6-6 format

     * 00000000RRRRRRRR GGGGGGGGBBBBBBBB =>
     * RRRRRRGGGGGGBBBB 00000000000000BB
     *
     * @param src rrggbb
     * @param dest The unpacked colour structure
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AColour<COLOURS_18BIT,TAccessMode,TPanelTraits>::unpackColour(tCOLOUR src,UnpackedColour& dest) const {

      dest.first=((src & 0xfc0000) >> 8) | ((src & 0x00fc00) >> 6) | ((src & 0xfc) >> 4);
      dest.second=(src & 0xc) >> 2;
    }


    /**
     * Unpack the colour from components to the internal format
     * @param red
     * @param green
     * @param blue
     * @param dest
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AColour<COLOURS_18BIT,TAccessMode,TPanelTraits>::unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const {

      dest.first=((((uint16_t)red) & 0xfc) << 8) |
                 ((((uint16_t)green) & 0xfc) << 2) |
                 ((((uint16_t)blue) & 0xfc) >> 4);

      dest.second=(blue & 0xc) >> 2;
    }


    /**
     * Write a single pixel to the current output position.
     * Assumes that the caller has already issued the beginWriting() command.
     * @param cr The pixel to write
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AColour<COLOURS_18BIT,TAccessMode,TPanelTraits>::writePixel(const UnpackedColour& cr) const {
      _accessMode.writeData(cr.first);
      _accessMode.writeData(cr.second);
    }


    /**
     * Write the same colour pixel that we last wrote. This gives the access mode a chance to
     * optimise sequential pixel writes. The colour is provided for drivers that cannot optimise
     * and must fall back to a full write.
     * @param cr The pixel to write
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AColour<COLOURS_18BIT,TAccessMode,TPanelTraits>::writePixelAgain(const UnpackedColour& cr) const {
      _accessMode.writeData(cr.first);
      _accessMode.writeData(cr.second);
    }


    /**
     * Fill a block of pixels with the same colour. This operation will issue the
     * beginWriting() command for you.
     * @param numPixels how many
     * @param cr The unpacked colour to write
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AColour<COLOURS_18BIT,TAccessMode,TPanelTraits>::fillPixels(uint32_t numPixels,const UnpackedColour& cr) const {

      uint16_t first,second;

      _accessMode.writeCommand(hx8352a::MEMORY_WRITE);

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

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AColour<COLOURS_18BIT,TAccessMode,TPanelTraits>::allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const {
      buffer=new uint8_t[numPixels*4];
      bytesPerPixel=4;
    }


    /**
     * Bulk-copy some pixels from the memory buffer to the LCD. The pixels must already be formatted ready
     * for transfer.
     * @param buffer The memory buffer
     * @param numPixels The number of pixels to transfer from the buffer
     */

    template<class TAccessMode,class TPanelTraits>
    inline void HX8352AColour<COLOURS_18BIT,TAccessMode,TPanelTraits>::rawTransfer(const void *buffer,uint32_t numPixels) const {
      _accessMode.rawTransfer(buffer,numPixels*2);
    }
  }
}
