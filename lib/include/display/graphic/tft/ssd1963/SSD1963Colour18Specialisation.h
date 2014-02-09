/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/ssd1963/commands/AllCommands.h"


namespace stm32plus {
  namespace display {


    /**
     * Template class holding the specialisation of SSD1963Colour for 18-bit colours
     * @tparam TAccessMode The access mode class (e.g. FSMC)
     * @tparam TPanelTraits Unique identifying data about this panel
     */

    template<class TAccessMode,class TPanelTraits>
    class SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits> {

      private:
        TAccessMode& _accessMode;

      protected:
        SSD1963Colour(TAccessMode& accessMode);

        constexpr uint8_t getLcdModeBits() const;

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
     * @param accessMode The access mode implementation
     */

    template<class TAccessMode,class TPanelTraits>
    inline SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::SSD1963Colour(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Get the register setting for 18 bit colours
     * @return 0 is the right setting
     */

    template<class TAccessMode,class TPanelTraits>
    constexpr inline uint8_t SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::getLcdModeBits() const {
      return 0;         // 18 bit
    }


    /**
     * Unpack the colour from rrggbb to the internal 12 bit x2 format. Don't bother masking off the
     * lower 2 bits - the SSD1963 will just ignore them.
     * 00000000RRRRRRRRGGGGGGGGBBBBBBBB ->
     * 0000RRRRRRRRGGGG,0000GGGGBBBBBBBB
     * @param src rrggbb
     * @param dest The unpacked colour structure
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::unpackColour(tCOLOUR src,UnpackedColour& dest) const {

      dest.first=(src & 0xfff000) >> 12;
      dest.second=src & 0xfff;
    }


    /**
     * Unpack the colour from components to the internal format
     * @param red
     * @param green
     * @param blue
     * @param dest
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const {

      dest.first=((uint16_t)red << 4) | (green >> 4);
      dest.second=(((uint16_t)green & 0xf) << 8) | blue;
    }


    /**
     * Write a single pixel to the current output position.
     * Assumes that the caller has already issued the beginWriting() command.
     * @param cr The pixel to write
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::writePixel(const UnpackedColour& cr) const {
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
    inline void SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::writePixelAgain(const UnpackedColour& cr) const {
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
    inline void SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::fillPixels(uint32_t numPixels,const UnpackedColour& cr) const {

      uint16_t first,second;

      _accessMode.writeCommand(ssd1963::MEMORY_WRITE);

      first=cr.first;
      second=cr.second;

      while(numPixels--) {
        _accessMode.writeData(first);
        _accessMode.writeData(second);
      }
    }


    /**
     * Allocate a buffer for pixel data. You supply the number of pixels and this allocates the buffer as a uint8_t[].
     * Allocated buffers should be freed with delete[]. There are 2 16-bit words per pixel.
     *
     * @param numPixels The number of pixels to allocate
     * @param buffer The output buffer
     * @param bytesPerPixel Output the number of bytes per pixel
     */

    template<class TAccessMode,class TPanelTraits>
    inline void SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const {
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
    inline void SSD1963Colour<COLOURS_18BIT,TAccessMode,TPanelTraits>::rawTransfer(const void *buffer,uint32_t numPixels) const {
      _accessMode.rawTransfer(buffer,numPixels*2);
    }
  }
}
