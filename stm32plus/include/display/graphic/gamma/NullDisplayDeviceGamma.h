/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * @brief gamma class that does nothing.
     *
     * Used for displays that don't support gamma correction
     */

    class NullDisplayDeviceGamma : public DisplayDeviceGamma {

      public:
        NullDisplayDeviceGamma() : DisplayDeviceGamma(1) {}
        virtual ~NullDisplayDeviceGamma() {}
    };
  }
}
