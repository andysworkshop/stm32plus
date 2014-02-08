/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief Base class for touch screen calibration algorithms.
     *
     * All touch screen calibration algorithms must be serialisable so that they can
     * save and restore their state between sessions.
     */

    class TouchScreenCalibration {

      public:
        enum {
          E_SERIALISATION_NOT_SUPPORTED=1
        };

      public:

        /**
         * Default constructor, does nothing
         */

        TouchScreenCalibration() {
        }

        /**
         * Virtual destructor, does nothing
         */

        virtual ~TouchScreenCalibration() {
        }


        /**
         * Translate a raw point from the touch screen co-ordinate space into the
         * co-ordinate space of the panel that it's attached to.
         *
         * @param[in] rawPoint The raw touch screen point.
         * @return The point translated to the display panel space.
         */

        virtual Point translate(const Point& rawPoint)=0;


        /**
         * Serialise the calibration parameters to an output stream
         * @param[in] ostream The output stream to write to.
         * @return false if it fails.
         */

        virtual bool serialise(OutputStream& ostream)=0;


        /**
         * Deserialise the calibration parameters from an input stream.
         * @param[in] istream The input stream to read the parameters from.
         * @return false if it fails.
         */

        virtual bool deserialise(InputStream& istream)=0;
    };

  }
}
