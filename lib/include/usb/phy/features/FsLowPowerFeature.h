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

      private:
        void onEvent(uint8_t extiLine);

      protected:
        FsLowPowerFeature();
        ~FsLowPowerFeature();

      public:
        bool initialise(const Parameters& params);
    };


    /**
     * Constructor
     * @param params the config parameters
     */

    inline FsLowPowerFeature::FsLowPowerFeature()
      : _wakeup(EXTI_Mode_Interrupt,EXTI_Trigger_Rising) {

      // clear anything pending

      _wakeup.clearPendingInterrupt();

      // subscribe to events

      _wakeup.ExtiInterruptEventSender.insertSubscriber(
          ExtiInterruptEventSourceSlot::bind(this,&FsLowPowerFeature::onEvent)
      );
    }


    /**
     * Initialise the class
     * @param parameters reference
     * @return true
     */

    inline bool FsLowPowerFeature::initialise(const Parameters& /* params */) {
      return true;
    }


    /**
     * Destructor, disable EXTI interrupts
     */

    inline FsLowPowerFeature::~FsLowPowerFeature() {

      ExtiInterruptEnabler<18>::disable();

      // unsubscribe from events

      _wakeup.ExtiInterruptEventSender.removeSubscriber(
          ExtiInterruptEventSourceSlot::bind(this,&FsLowPowerFeature::onEvent)
      );
    }


    /**
     * Interrupt callback from the EXTI interrupt
     */

      extern "C" void SetSysClock();

    __attribute__((noinline)) inline void FsLowPowerFeature::onEvent(uint8_t /* extiLine */) {

      // reset SLEEPDEEP bit of cortex system control register

      SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

      SetSysClock();

      // ungate PHY clock

      *reinterpret_cast<volatile uint32_t *>(USB_OTG_FS_PERIPH_BASE+USB_OTG_PCGCCTL_BASE) &= ~USB_OTG_PCGCCTL_STOPCLK;
    }
  }
}
