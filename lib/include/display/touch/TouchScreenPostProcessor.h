/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * @brief base class for post processors of touch screen coords
     *
     * A post processor may perform such functions as averaging successive points to
     * get a more stable result.
     */

    class TouchScreenPostProcessor {
      public:

        /**
         * Possible results from the postProcess call.
         */

        enum PostProcessAction {

          /// post processing is completed, no more samples are required
          Completed,

          /// something went wrong in the post processing, abort.
          Error,

          /// more sample are required, call postProcess again.
          MoreSamplesRequired
        };

      public:

        /**
         * Post process a point from the touch screen.
         * @param[in] point The point received from the touch screen.
         * @param[in] sequenceNumber The zero based index of this sample from the
         *  touch screen.
         */

        virtual PostProcessAction postProcess(Point& point,int sequenceNumber)=0;

        virtual ~TouchScreenPostProcessor() {}
    };
  }
}
