/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /*
     * Write a null terminated string of characters to the display.
     * Returns the size of the string
     */

    template<class TDevice,class TAccessMode>
    inline Size GraphicsLibrary<TDevice,TAccessMode>::writeString(const Point& p,const LzgFont& font,const char *str) {

      const char *ptr;
      Point pos(p);
      const FontChar *fc;
      int16_t width;
      Size s;

      s.Height=font.getHeight();
      s.Width=0;

      // print each character in turn

      for(ptr=str;*ptr;ptr++) {

        font.getCharacter((uint8_t)*ptr,fc);

        if(fc->Code!=' ')                   // space is special and never has any data but does have a size
          writeCharacter(pos,font,*fc);

        width=fc->PixelWidth+font.getCharacterSpacing();
        pos.X+=width;
        s.Width+=width;
      }

      return s;
    }


    /*
     * Write a single character
     */

    template<class TDevice,class TAccessMode>
    inline void GraphicsLibrary<TDevice,TAccessMode>::writeCharacter(const Point& p,const LzgFont& font,const FontChar& fc) {

      uint16_t lsb,msb,dataSize;
      uint8_t *ptr;
      Point charLocation(p);

      // extract the data size and data ptr

      ptr=const_cast<uint8_t *>(fc.Data);
      lsb=*ptr++;
      msb=*ptr++;

      dataSize=(msb<<8) | lsb;

      // set up the stream and rectangle

      LinearBufferInputOutputStream is(ptr,dataSize);
      LzgDecompressionStream lzg(is,dataSize);
      Rectangle rc(p.X,p.Y,fc.PixelWidth,font.getHeight());

      // draw the bitmap

      drawBitmap(rc,lzg);
    }
  }
}
