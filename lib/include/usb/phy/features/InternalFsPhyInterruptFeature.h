/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Forward declare the IRQ handler name
 */

extern "C" void OTG_FS_IRQHandler();


namespace stm32plus {
  namespace usb {


    /**
     * Interrupt feature for the OTG FS PHY. This is automatically included by the FS PHY
     * implementation. There's no need to include it in the PHY feature list yourself.
     */

    class InternalFsPhyInterruptFeature {

      public:

        /**
         * Parameters structure
         */

        struct Parameters {
          uint8_t fsphy_nvicPriority;      // primary interrupt priority
          uint8_t fsphy_nvicSubPriority;   // secondary priority

          Parameters() {
            fsphy_nvicPriority=5;
            fsphy_nvicSubPriority=0;
          }
        };

        static UsbEventSource *_usbEventSource;

      protected:
        typedef void (*FPTR)();       // this trick will force the linker to include the ISR
        static FPTR _forceLinkage;

        Parameters _params;

      public:
        bool initialise(const Parameters& params);

      protected:
        InternalFsPhyInterruptFeature(UsbEventSource& eventSource);
        ~InternalFsPhyInterruptFeature();

        void phyEnableInterrupts() const;
        void phyDisableInterrupts() const;
    };


    /**
     * Constructor, enable the interrupts
     */

    inline InternalFsPhyInterruptFeature::InternalFsPhyInterruptFeature(UsbEventSource& eventSource) {

      // force IRQ linkage

      _forceLinkage=&OTG_FS_IRQHandler;
      _usbEventSource=&eventSource;
    }


    /**
     * Initialise the class
     * @return true
     */

    inline bool InternalFsPhyInterruptFeature::initialise(const Parameters& params) {
      _params=params;
      return true;
    }


    /**
     * Destructor, unsubscribe
     * @param eventDescriptor
     */

    inline InternalFsPhyInterruptFeature::~InternalFsPhyInterruptFeature() {

      // disable interrupts

      phyDisableInterrupts();
    }


    /**
     * Enable interrupts
     */

    inline void InternalFsPhyInterruptFeature::phyEnableInterrupts() const {
      Nvic::configureIrq(OTG_FS_IRQn,ENABLE,_params.fsphy_nvicPriority,_params.fsphy_nvicSubPriority);
    }


    /**
     * Disable the interrupts
     */

    inline void InternalFsPhyInterruptFeature::phyDisableInterrupts() const {
      Nvic::configureIrq(OTG_FS_IRQn,DISABLE,_params.fsphy_nvicPriority,_params.fsphy_nvicSubPriority);
    }
  }
}
