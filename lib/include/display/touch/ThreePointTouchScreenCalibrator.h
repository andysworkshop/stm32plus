/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


namespace stm32plus {
  namespace display {


    /**
     * Three point touch screen calibration, based on the paper "Calibration in touch-screen systems"
     * by Texas Instruments. The touch screen must have been initialised with the pass-through calibration.
     */

    class ThreePointTouchScreenCalibrator : public TouchScreenCalibrator {

      public:

        /**
         * Abstract class implemented by the caller to provide
         * the GUI feedback that we need, i.e. prompts and hit-points
         */

        class GuiCallback {
          public:

          /**
           * Display the given prompt on screen. The prompt will be asking the user to
           * tap one of the points
           * @param text The prompt text
           */

            virtual void displayPrompt(const char *text)=0;

            /**
             * Display a hit point on the screen at the given location.
             * @param pt Where to display the point
             */

            virtual void displayHitPoint(const Point& pt)=0;

            /**
             * Get the panel size in pixels. The algorithm needs this for its
             * internal calculations.
             * @return The size
             */

            virtual Size getPanelSize()=0;
        };

      protected:
        volatile bool _clicked;

        GuiCallback& _gui;
        TouchScreen& _ts;

      protected:
        void getUserInput(int xpercent,int ypercent,Point& panelPoint,Point& touchPoint);
        void onTouchScreenReady();

      public:

        ThreePointTouchScreenCalibrator(TouchScreen& ts,GuiCallback& gui);
        virtual ~ThreePointTouchScreenCalibrator() {}

        // Overrides from TouchScreenCalibration
        virtual bool calibrate(TouchScreenCalibration*& calibrationResults) override;
    };
  }
}
