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
     * Constructor. Takes the number of samples required to do the average.
     * @param[in] samplesRequired The number of samples that will be averaged.
     */


    AveragingTouchScreenPostProcessor::AveragingTouchScreenPostProcessor(int16_t samplesRequired)
      : _samplesRequired(samplesRequired) {

    }


    /**
     * Post-process a point. Keep accepting samples until we've reached the number that
     * we're going to average then return Completed.
     */

    TouchScreenPostProcessor::PostProcessAction AveragingTouchScreenPostProcessor::postProcess(Point& point,int sequenceNumber) {

      if(sequenceNumber==0) {
        _xTotal=point.X;
        _yTotal=point.Y;
      }
      else {
        _xTotal+=point.X;
        _yTotal+=point.Y;
      }

      if(sequenceNumber==_samplesRequired-1) {

        point.X=_xTotal/_samplesRequired;
        point.Y=_yTotal/_samplesRequired;

        return Completed;
      }

      return MoreSamplesRequired;
    }
  }
}
