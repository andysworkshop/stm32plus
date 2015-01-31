/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * plot a point in the foreground colour
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::plotPoint(const Point& p) {

      this->moveTo(
          Rectangle(
              p.X,
              p.Y,
              1,
              1
          )
      );

      this->beginWriting();
      this->writePixel(_foreground);
    }


    /**
     * Draw a line between two points. Modified slightly by me (Andy Brown) to include the final point
     * in the line. Therefore drawing from (x,y) to (x,y) will result in a single point being plotted.
     * Freely useable in non-commercial applications as long as credits
     * to Po-Han Lin and link to http://www.edepot.com is provided in
     * source code and can been seen in compiled executable.
     * Commercial applications please inquire about licensing the algorithms.
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::drawLine(const Point& p1,const Point& p2) {

      // optimisation for straight lines. filling rectangles is much more efficient than plotting points

      if(p1.X==p2.X)
        fillRectangle(Rectangle(p1.X,std::min<int16_t>(p1.Y,p2.Y),1,std::abs(p2.Y-p1.Y)+1));
      else if(p1.Y==p2.Y)
        fillRectangle(Rectangle(std::min<int16_t>(p1.X,p2.X),p1.Y,std::abs(p2.X-p1.X)+1,1));
      else {
        int16_t x0,x1,y0,y1;

        x0=p1.X;
        y0=p1.Y;
        x1=p2.X;
        y1=p2.Y;

        if(x0>x1) {

          // the optimiser does this swap method faster than
          // the xor-trick

          int16_t t;

          t=x0;
          x0=x1;
          x1=t;

          t=y0;
          y0=y1;
          y1=t;
        }

        // calculate constants up-front

        int16_t dx=x1-x0;
        int16_t dy=std::abs(y1-y0);
        int16_t sy=y0<y1 ? 1 : -1;
        int16_t mdy=-dy;
        int16_t err=dx-dy;
        bool xinc;

        // set the drawing rectangle that we need and plot the first point

        this->moveTo(x0,y0,this->getXmax(),this->getYmax());
        this->beginWriting();
        this->writePixel(_foreground);

        while(x0!=x1 || y0!=y1) {

          int16_t e2=2*err;

          if(e2>mdy) {

            err-=dy;
            x0++;

            // make a note that X has incremented

            xinc=true;
          }
          else
            xinc=false;       // nothing happened to X

          if(x0==x1 && y0==y1) {

            if(xinc) {

              // plot the pending X increment before returning

              this->writePixelAgain(_foreground);
              break;
            }
          }

          if(e2<dx) {
            err+=dx;
            y0+=sy;

            // Y has changed. We're going to have to do a complete
            // pixel write after we've moved the bare minimum of
            // window pointers

            if(xinc)
              this->moveX(x0,this->getXmax());

            this->moveY(y0,this->getYmax());

            this->beginWriting();
            this->writePixel(_foreground);
          }
          else {

            // Y has not changed, if X has changed then all we need
            // to do is push out another pixel

            if(xinc)
              this->writePixelAgain(_foreground);
          }
        }
      }
    }
  }
}
