/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Fill a rectangle with the foreground colour
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::fillRectangle(const Rectangle& rc) {

      this->moveTo(rc);
      this->fillPixels((uint32_t)rc.Width * (uint32_t)rc.Height,_foreground);
    }


    /**
     * Fill a rectangle with the background colour
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::clearRectangle(const Rectangle& rc) {

      this->moveTo(rc);
      this->fillPixels((uint32_t)rc.Width * (uint32_t)rc.Height,_background);
    }


    /**
     * Convenience function to draw an outline of a rectangle by calling fillRectangle 4 times
     * Filling rectangles is much more efficient than plotting points
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::drawRectangle(const Rectangle& rect) {

      Rectangle rc(rect);

      // top

      rc.Height=1;
      fillRectangle(rc);

      // bottom

      rc.Y+=rect.Height-1;
      fillRectangle(rc);

      // left

      rc.Height=rect.Height;
      rc.Y=rect.Y;
      rc.Width=1;
      fillRectangle(rc);

      // right
      rc.X+=rect.Width-1;
      fillRectangle(rc);
    }


    /*
     * Gradient fill a rectangle from the foreground to the background colour
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::gradientFillRectangle(const Rectangle& rc,
                                                                                  Direction dir,
                                                                                  tCOLOUR first,
                                                                                  tCOLOUR last) {

      uint8_t r1,g1,b1,r2,g2,b2;
      int32_t rstep,gstep,bstep;
      int16_t raccum,gaccum,baccum,i,r,g,b,val,div,xdisp,ydisp,px;
      Rectangle rcBlock;
      tCOLOUR cr;
      UnpackedColour lineColour;

      // get the start and end colour components

      r1=first >> 16;
      g1=first >> 8;
      b1=first;

      r2=last >> 16;
      g2=last >> 8;
      b2=last;

      // calculate initial sizes and directions

      rcBlock.X=rc.X;
      rcBlock.Y=rc.Y;

      if(dir==VERTICAL) {
        rcBlock.Width=rc.Width;
        rcBlock.Height=1;
        xdisp=0;
        ydisp=1;
        div=rc.Height;
        px=rc.Width;
      }
      else {
        rcBlock.Width=1;
        rcBlock.Height=rc.Height;
        xdisp=1;
        ydisp=0;
        div=rc.Width;
        px=rc.Height;
      }

      // calculate the step values, scaled up x256 for greater precision

      rstep=(((int32_t)r2-(int32_t)r1)*256)/div;
      gstep=(((int32_t)g2-(int32_t)g1)*256)/div;
      bstep=(((int32_t)b2-(int32_t)b1)*256)/div;

      // initial colour and reset accumulators

      cr=first;
      raccum=gaccum=baccum=0;

      r=r1;
      g=g1;
      b=b1;

      for(i=0;i<div;i++) {

        // draw the line

        this->moveTo(rcBlock);
        this->unpackColour(cr,lineColour);
        this->fillPixels(px,lineColour);

        // update for the next line

        rcBlock.X+=xdisp;
        rcBlock.Y+=ydisp;

        // update the accumulators

        raccum+=rstep;
        gaccum+=gstep;
        baccum+=bstep;

        // if any accumulator has moved past 0 (scaled by 256) then add that accumulator
        // to the colour and reduce the accumulator accordingly

        if((val=raccum/256)!=0) {
          r+=val;
          raccum-=val*256;
        }

        if((val=gaccum/256)!=0) {
          g+=val;
          gaccum-=val*256;
        }

        if((val=baccum/256)!=0) {
          b+=val;
          baccum-=val*256;
        }

        // if any accumulator was reset then we have a new colour

        cr=(uint32_t)r<<16 | (uint32_t)g<<8 | b;
      }
    }
  }
}
