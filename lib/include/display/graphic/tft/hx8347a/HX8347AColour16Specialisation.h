/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "display/graphic/tft/hx8347a/commands/WriteDataCmd.h"


namespace stm32plus {
  namespace display {


    /**
     * Template class holding the specialisation of HX8347AColour for 16-bit colours
     * @tparam TAccessMode The access mode class (e.g. FSMC)
     */

    template<class TAccessMode>
    class HX8347AColour<COLOURS_16BIT,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        HX8347AColour(TAccessMode& accessMode);

        typedef uint32_t tCOLOUR;

        struct UnpackedColour {
          uint16_t packed565;
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
    inline HX8347AColour<COLOURS_16BIT,TAccessMode>::HX8347AColour(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Unpack the colour from rrggbb to the internal 5-6-5 format

     * 00000000RRRRRRRRGGGGGGGGBBBBBBBB ->
     * 0000000000000000RRRRRGGGGGGBBBBB

     * @param src rrggbb
     * @param dest The unpacked colour structure
     */

    template<class TAccessMode>
    inline void HX8347AColour<COLOURS_16BIT,TAccessMode>::unpackColour(tCOLOUR src,UnpackedColour& dest) const {
      dest.packed565=(src & 0xf80000) >> 8 | (src & 0xfc00) >> 5 | (src & 0xf8) >> 3;
    }


    /**
     * Unpack the colour from components to the internal format
     * @param red
     * @param green
     * @param blue
     * @param dest
     */

    template<class TAccessMode>
    inline void HX8347AColour<COLOURS_16BIT,TAccessMode>::unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const {

      red&=0xf8;
      green&=0xfc;
      blue&=0xf8;

      dest.packed565=(((uint16_t)red) << 8) | (((uint16_t)green) << 3) | (blue >> 3);
    }


    /**
     * Write a single pixel to the current output position.
     * Assumes that the caller has already issued the beginWriting() command.
     * @param cr The pixel to write
     */

    template<class TAccessMode>
    inline void HX8347AColour<COLOURS_16BIT,TAccessMode>::writePixel(const UnpackedColour& cr) const {
      _accessMode.writeData(cr.packed565);
    }


    /**
     * Write the same colour pixel that we last wrote. This gives the access mode a chance to
     * optimise sequential pixel writes. The colour is provided for drivers that cannot optimise
     * and must fall back to a full write.
     * @param cr The same pixel to write again
     */

    template<class TAccessMode>
    inline void HX8347AColour<COLOURS_16BIT,TAccessMode>::writePixelAgain(const UnpackedColour& cr) const {
      _accessMode.writeDataAgain(cr.packed565);
    }


    /**
     * Fill a block of pixels with the same colour. This operation will issue the
     * beginWriting() command for you.
     * @param numPixels how many
     * @param cr The unpacked colour to write
     */

    template<class TAccessMode>
    inline void HX8347AColour<COLOURS_16BIT,TAccessMode>::fillPixels(uint32_t numPixels,const UnpackedColour& cr) const {

      _accessMode.writeCommand(hx8347::WriteDataCmd::Opcode);
      _accessMode.writeMultiData(numPixels,cr.packed565);
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
    inline void HX8347AColour<COLOURS_16BIT,TAccessMode>::allocatePixelBuffer(uint32_t numPixels,uint8_t*& buffer,uint32_t& bytesPerPixel) const {
      buffer=new uint8_t[numPixels*2];
      bytesPerPixel=2;
    }


    /**
     * Bulk-copy some pixels from the memory buffer to the LCD. The pixels must already be formatted ready
     * for transfer.
     * @param buffer The memory buffer
     * @param numPixels The number of pixels to transfer from the buffer
     */

    template<class TAccessMode>
    inline void HX8347AColour<COLOURS_16BIT,TAccessMode>::rawTransfer(const void *buffer,uint32_t numPixels) const {
      _accessMode.rawTransfer(buffer,numPixels);
    }
  }
}
