/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB low power feature for the OTG FS PHY
     */

    class FsLowPowerFeature {

      protected:
        ExtiUsbFsWakeup _wakeup;

      public:
        struct Parameters {
        };

      protected:
        FsLowPowerFeature(const Parameters& params);
        ~FsLowPowerFeature();
    };


    /**
     * Constructor
     * @param params the config parameters
     */

    inline FsLowPowerFeature::FsLowPowerFeature(const Parameters& params)
      : _wakeup(EXTI_Mode_Interrupt,EXTI_Trigger_Rising) {

      _wakeup.clearPendingInterrupt();
    }



    /**
     * Destructor, disable EXTI interrupts
     */

    inline FsLowPowerFeature::~FsLowPowerFeature() {
      ExtiInterruptEnabler<18>::disable();
    }
  }
}
