/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * @brief Pass-through touch-screen calibration.
     *
     * This class gives access to the raw co-ordinates from the touch screen by passing them through
     * unmodified. This is typically used by a calibration routine to avoid the chicken-and-egg situation
     * of the TouchScreen class requiring a calibration class even before it's calibrated.
     */

    class PassThroughTouchScreenCalibration : public TouchScreenCalibration {

      protected:
        float _ax,_bx,_dx;
        float _ay,_by,_dy;

      public:

        virtual ~PassThroughTouchScreenCalibration() {
        }

        // overrides from TouchScreenCalibration

        virtual Point translate(const Point& rawPoint) override;
        virtual bool serialise(OutputStream& ostream) override;
        virtual bool deserialise(InputStream& istream) override;
    };
  }
}
