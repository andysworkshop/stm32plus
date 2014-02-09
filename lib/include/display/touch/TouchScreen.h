/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief Base class for touch screen implementations.
     *
     * A touch screen is a resistive or capacitative controller that allows the user to interact with
     * a display by touching it. Feedback to the MCU may be via polling or asynchronous modes. The TouchScreen
     * class is independent of the resolution of both the touch panel and the underlying display. That knowledge,
     * if required, is managed by the calibration system.
     */

    DECLARE_EVENT_SIGNATURE(TouchScreenReady,void());

    class TouchScreen {

      protected:
        TouchScreenCalibration* _calibration;
        TouchScreenPostProcessor* _postProcessor;

      public:

        /**
         * Error codes
         */

        enum {
          /// There is no touch in progress, or the touch ended before a sample could be completed.
          E_NO_TOUCH = 1
        };

        DECLARE_EVENT_SOURCE(TouchScreenReady);

      public:

        /**
         * Constructor
         * @param[in] calibration The calibration implementation
         * @param[in] postProcessor The touch screen post processor implementation
         */

        TouchScreen(TouchScreenCalibration& calibration,TouchScreenPostProcessor& postProcessor)
          : _calibration(&calibration),
            _postProcessor(&postProcessor) {
        }

        virtual ~TouchScreen() {}


        /**
         * Check if the screen is touched at this instant
         * @return true if touched
         */

        virtual bool isTouched() const=0;


        /**
         * Get the calibration class
         * @return The calibration class
         */

        TouchScreenCalibration& getCalibration() {
          return *_calibration;
        }


        /**
         * Get the post processor class
         * @return The post processor class
         */

        TouchScreenPostProcessor& getPostProcessor() {
          return *_postProcessor;
        }


        /**
         * Set the calibration pointer
         * @param[in] calibration The new calibration class
         */

        void setCalibration(TouchScreenCalibration& calibration) {
          _calibration=&calibration;
        }


        /**
         * Set the postprocessor
         * @param[in] postProcessor reference to the post processor class
         */

        void setPostProcessor(TouchScreenPostProcessor& postProcessor) {
          _postProcessor=&postProcessor;
        }


        /**
         * Get the current x-y co-ordinates of a touch. There must be a touch in progress and it must last for long
         * enough to get a sample.
         *
         * @param[out] point The co-ordinate on the display of the touch. The returned co-ordinate should be validated against
         *  the bounds of the display because the panel may extend beyond the display into areas such as icons illustrated
         *  on a facia, for example.
         * @return false if it fails.
         */

        virtual bool getCoordinates(Point& point)=0;
    };
  }
}
