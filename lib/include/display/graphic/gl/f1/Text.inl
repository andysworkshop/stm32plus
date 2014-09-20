/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {
  namespace display {


    /**
     * Write a single character using bitbanding
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::writeCharacterFill(const Point& p,const Font& font,const FontChar& fc) {

      const uint32_t *bitbandLocation;
      uint16_t numPixels;

      // move to the rectangle that encloses the character

      this->moveTo(
        Rectangle(
            p.X,
            p.Y,
            fc.PixelWidth,
            font.getHeight()
        )
      );

      // get the first value and the corresponding position of that value in the bitband region

      bitbandLocation=reinterpret_cast<uint32_t *> (SRAM_BB_BASE | ((reinterpret_cast<uint32_t>(fc.Data)-SRAM_BASE) << 5));

      // go from top to bottom, left to right

      this->beginWriting();

      numPixels=font.getHeight()*fc.PixelWidth;
      while(numPixels--) {

        // if there's a value at this position, write a pixel

        if(*bitbandLocation++)
          this->writePixel(_foreground);
        else
          this->writePixel(_background);
      }
    }


    /**
     * Write a single character withoug filling the background. Slower than filling the background due
     * to the movement calculations
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::writeCharacterNoFill(const Point& p,const Font& font,const FontChar& fc) {

      uint8_t needToMove;
      uint32_t numBits,*bitbandLocation;
      int16_t baseY,x,y;

      // get the font character

      bitbandLocation=reinterpret_cast<uint32_t *> (SRAM_BB_BASE | ((reinterpret_cast<uint32_t>(fc.Data)-SRAM_BASE) << 5));

      baseY=p.Y;

      x=0;
      y=baseY;

      // follow the data

      numBits=static_cast<uint32_t>(font.getHeight()) * static_cast<uint32_t>(fc.PixelWidth);
      needToMove=false;

      while(numBits--) {
        if(*bitbandLocation++) {

          // if we've been skipping then move

          if(needToMove) {
            this->moveTo(Rectangle(x+p.X,y,fc.PixelWidth-x,font.getHeight()-(y-baseY)));
            needToMove=false;
            this->beginWriting();
          }

          // write out the pixel

          this->writePixel(_foreground);

          // the position is automatically incremented horizontally but if we run out
          // of width then we'll have to move the position next time

          if(++x==fc.PixelWidth) {
            y++;
            x=0;
            needToMove=true;
          }
        } else {
          // nothing at this position, update x,y

          if(++x==fc.PixelWidth) {
            // hit the width, bump y and reset x

            x=0;
            y++;
          }
          needToMove=true;
        }
      }
    }
  }
}
