/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * @brief Linear ease. Really a dummy as it doesn't ease at all.
     */

    class LinearEase : public EasingBase {

      public:
        /// no acceleration
        virtual double easeIn(double time) const override;

        /// no acceleration
        virtual double easeOut(double time) const override;

        /// no acceleration
        virtual double easeInOut(double time) const override;
    };
  }
}
