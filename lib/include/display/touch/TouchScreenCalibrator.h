/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    class TouchScreen;

    /**
     * @brief base class for touch screen calibration algorithms.
     *
     * This base class enforces an API on derived classes but does not hide the type of calibration algorithm since the
     * user must still choose this at runtime.
     */

    class TouchScreenCalibrator {

      public:
        virtual ~TouchScreenCalibrator() {}

        /**
         * Interactively calibrate the touch screen with the user. This routine will present a UI to the user for calibration
         * and block until it's done. The resulting calibration class will be returned in the calibrationResults parameter and
         * can be serialised to an output device for restoration in future sessions.
         *
         * @param[out] calibrationResults The results of the calibration. Caller is responsible for deleting the pointer.
         *
         * @return false if it fails
         */

        virtual bool calibrate(TouchScreenCalibration*& calibrationResults)=0;
    };
  }
}
