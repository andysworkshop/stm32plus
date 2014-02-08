/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Post-processor for touch screen co-ordinates that averages a
     * number of samples to smooth out noise.
     */

    class AveragingTouchScreenPostProcessor : public TouchScreenPostProcessor {

      protected:
        int16_t _samplesRequired;
        int32_t _xTotal,_yTotal;

      public:
        AveragingTouchScreenPostProcessor(int16_t samplesRequired);
        virtual ~AveragingTouchScreenPostProcessor() {}

        // overrides from TouchScreenPostProcessor

        virtual PostProcessAction postProcess(Point& point,int sequenceNumber) override;
    };
  }
}
