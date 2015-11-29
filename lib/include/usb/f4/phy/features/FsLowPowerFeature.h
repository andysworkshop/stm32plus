/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB low power feature for the OTG FS PHY. This feature subscribes to the
     * EXTI line 18 (USB FS wakeup) event and wakes up the MCU when it happens. This relies
     * on you having a SetSysClock() "C" method in your system startup file.
     */

    class FsLowPowerFeature : public PhyFeatureBase {

      protected:
        ExtiUsbFsWakeup _wakeup;

      public:
        struct Parameters {
        };

      private:
        void onWakeupEvent(uint8_t extiLine);
        void onUsbEvent(UsbEventDescriptor& event);

      protected:
        FsLowPowerFeature(UsbEventSource& eventSource);
        ~FsLowPowerFeature();

      public:
        bool initialise(const Parameters& params);
    };


    /**
     * Constructor
     * @param params the config parameters
     */

    inline FsLowPowerFeature::FsLowPowerFeature(UsbEventSource& eventSource)
      : PhyFeatureBase(eventSource),
        _wakeup(EXTI_Mode_Interrupt,EXTI_Trigger_Rising) {

      // clear anything pending

      _wakeup.clearPendingInterrupt();

      // subscribe to wakeup events

      _wakeup.ExtiInterruptEventSender.insertSubscriber(
          ExtiInterruptEventSourceSlot::bind(this,&FsLowPowerFeature::onWakeupEvent)
      );

      // subscribe to USB events

      _eventSource.UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&FsLowPowerFeature::onUsbEvent)
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

      // unsubscribe from wakeup events

      _wakeup.ExtiInterruptEventSender.removeSubscriber(
          ExtiInterruptEventSourceSlot::bind(this,&FsLowPowerFeature::onWakeupEvent)
      );

      // unsubscribe from USB events

      _eventSource.UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&FsLowPowerFeature::onUsbEvent)
      );
    }


    /**
     * Interrupt callback from the EXTI interrupt
     */

      extern "C" void SetSysClock();

    __attribute__((noinline)) inline void FsLowPowerFeature::onWakeupEvent(uint8_t /* extiLine */) {

      // reset SLEEPDEEP bit of cortex system control register

      SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

      SetSysClock();

      // ungate PHY clock

      *reinterpret_cast<volatile uint32_t *>(USB_OTG_FS_PERIPH_BASE+USB_OTG_PCGCCTL_BASE) &= ~USB_OTG_PCGCCTL_STOPCLK;
    }


    /**
     * Event handler for usb events
     * @param event The event descriptor
     */

    __attribute__((noinline)) inline void FsLowPowerFeature::onUsbEvent(UsbEventDescriptor& event) {

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::DEVICE_IRQ_SUSPEND:
          *reinterpret_cast<volatile uint32_t *>(USB_OTG_FS_PERIPH_BASE+USB_OTG_PCGCCTL_BASE) |= USB_OTG_PCGCCTL_STOPCLK;
          break;

        case UsbEventDescriptor::EventType::DEVICE_IRQ_RESUME:
          *reinterpret_cast<volatile uint32_t *>(USB_OTG_FS_PERIPH_BASE+USB_OTG_PCGCCTL_BASE) &= ~USB_OTG_PCGCCTL_STOPCLK;
          break;

        default:
          break;
      }
    }
  }
}
