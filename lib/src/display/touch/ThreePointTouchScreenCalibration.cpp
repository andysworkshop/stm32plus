/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"
#include "config/display/touch.h"


namespace stm32plus {
  namespace display {


   /**
    * Default constructor, for use when serialising
    */

    ThreePointTouchScreenCalibration::ThreePointTouchScreenCalibration() {
    }


    /**
     * Constructor when you've taken three point samples from the display.
     * Calculates the coeffient values used in the co-ordinate mapping.
     */

    ThreePointTouchScreenCalibration::ThreePointTouchScreenCalibration(
            const Point& p1Panel,const Point& p2Panel,const Point& p3Panel,
            const Point& p1Touch,const Point& p2Touch,const Point& p3Touch) {

      int32_t delta,deltaX1,deltaX2,deltaX3,deltaY1,deltaY2,deltaY3;

      // intermediate values for the calculation

      delta=((int32_t)(p1Touch.X-p3Touch.X)*(int32_t)(p2Touch.Y-p3Touch.Y))-((int32_t)(p2Touch.X-p3Touch.X)*(int32_t)(p1Touch.Y-p3Touch.Y));

      deltaX1=((int32_t)(p1Panel.X-p3Panel.X)*(int32_t)(p2Touch.Y-p3Touch.Y))-((int32_t)(p2Panel.X-p3Panel.X)*(int32_t)(p1Touch.Y-p3Touch.Y));
      deltaX2=((int32_t)(p1Touch.X-p3Touch.X)*(int32_t)(p2Panel.X-p3Panel.X))-((int32_t)(p2Touch.X-p3Touch.X)*(int32_t)(p1Panel.X-p3Panel.X));
      deltaX3=p1Panel.X*((int32_t)p2Touch.X*(int32_t)p3Touch.Y - (int32_t)p3Touch.X*(int32_t)p2Touch.Y) -
              p2Panel.X*(p1Touch.X*(int32_t)p3Touch.Y - (int32_t)p3Touch.X*(int32_t)p1Touch.Y) +
              p3Panel.X*((int32_t)p1Touch.X*(int32_t)p2Touch.Y - (int32_t)p2Touch.X*(int32_t)p1Touch.Y);

      deltaY1=((int32_t)(p1Panel.Y-p3Panel.Y)*(int32_t)(p2Touch.Y-p3Touch.Y))-((int32_t)(p2Panel.Y-p3Panel.Y)*(int32_t)(p1Touch.Y-p3Touch.Y));
      deltaY2=((int32_t)(p1Touch.X-p3Touch.X)*(int32_t)(p2Panel.Y-p3Panel.Y))-((int32_t)(p2Touch.X-p3Touch.X)*(int32_t)(p1Panel.Y-p3Panel.Y));
      deltaY3=p1Panel.Y*((int32_t)p2Touch.X*(int32_t)p3Touch.Y - (int32_t)p3Touch.X*(int32_t)p2Touch.Y) -
              p2Panel.Y*((int32_t)p1Touch.X*(int32_t)p3Touch.Y - (int32_t)p3Touch.X*(int32_t)p1Touch.Y) +
              p3Panel.Y*((int32_t)p1Touch.X*(int32_t)p2Touch.Y - (int32_t)p2Touch.X*p1Touch.Y);

      // final values

      _ax=(float)deltaX1/(float)delta;
      _bx=(float)deltaX2/(float)delta;
      _dx=(float)deltaX3/(float)delta;

      _ay=(float)deltaY1/(float)delta;
      _by=(float)deltaY2/(float)delta;
      _dy=(float)deltaY3/(float)delta;
    }


    /*
     * Translate co-ordinates from raw to display.
     */

    Point ThreePointTouchScreenCalibration::translate(const Point& rawPoint) {

      Point p;

      p.X=_ax*rawPoint.X + _bx*rawPoint.Y + _dx;
      p.Y=_ay*rawPoint.X + _by*rawPoint.Y + _dy;

      return p;
    }



    /*
     * Serialise to an output stream
     */

    bool ThreePointTouchScreenCalibration::serialise(OutputStream& ostream) {

      return ostream.write(&_ax,sizeof(_ax)) &&
             ostream.write(&_bx,sizeof(_bx)) &&
             ostream.write(&_dx,sizeof(_dx)) &&
             ostream.write(&_ay,sizeof(_ay)) &&
             ostream.write(&_by,sizeof(_by)) &&
             ostream.write(&_dy,sizeof(_dy));
    }

    /*
     * Deserialise from an input stream
     */

    bool ThreePointTouchScreenCalibration::deserialise(InputStream& istream) {

      uint32_t actuallyRead;

      if(!istream.read(&_ax,sizeof(_ax),actuallyRead) ||
         !istream.read(&_bx,sizeof(_bx),actuallyRead) ||
         !istream.read(&_dx,sizeof(_dx),actuallyRead) ||
         !istream.read(&_ay,sizeof(_ay),actuallyRead) ||
         !istream.read(&_by,sizeof(_by),actuallyRead) ||
         !istream.read(&_dy,sizeof(_dy),actuallyRead))
        return false;

      return true;
    }
  }
}
