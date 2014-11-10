/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Internal full-speed PHY implementation, inherits from the PHY base functionality
     */

    template<class... Features>
    class InternalFsPhy {

      public:
        struct Parameters : Features::Parameters... {

          uint16_t phy_rxFifoSize;      // default is 0x80

          Parameters() {
            phy_rxFifoSize=0x80;
          }
        };

      protected:
        USB_OTG_GlobalTypeDef *_phyRegisters;

      public:
        InternalFsPhy(Parameters& params);
        ~InternalFsPhy();

        bool phySupportsDma();
        uint32_t getPhyInterface() const;
        uint32_t getPhySpeed() const;
    };


    /**
     * Constructor
     * @param params Reference to the parameters structure
     */

    template<class... Features>
    inline InternalFsPhy<Features...>::InternalFsPhy(Parameters& params)
      : Features(params)...,
        _phyRegisters(USB_OTG_FS) {

      // configure DM/DP Pins

      GpioPinInitialiser::initialise(
          GPIOA,
          GPIO_Pin_11 | GPIO_Pin_12,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)OtgFsPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GPIO_AF_OTG_FS);

      // configure VBUS Pin

      GpioPinInitialiser::initialise(
          GPIOA,
          GPIO_Pin_9,
          Gpio::INPUT,
          (GPIOSpeed_TypeDef)OtgFsPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          GPIO_AF_OTG_FS);

      // configure ID pin

      GpioPinInitialiser::initialise(
          GPIOA,
          GPIO_Pin_10,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)OtgFsPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_UP,
          Gpio::OPEN_DRAIN,
          GPIO_AF_OTG_FS);

      // PHY clock on

      ClockControl<PERIPHERAL_OTG_FS>::On();
    }


    /**
     * Destructor
     */

    template<class... Features>
    inline InternalFsPhy<Features...>::~InternalFsPhy() {

      // PHY clock off

      ClockControl<PERIPHERAL_OTG_FS>::Off();
    }


    /**
     * Return true if the PHY supports DMA
     * @return false
     */

    template<class... Features>
    inline bool InternalFsPhy<Features...>::phySupportsDma() const {
      return false;
    }


    /**
     * Get the type of interface (embedded/ULPI)
     * @return PCD_PHY_EMBEDDED
     */

    template<class... Features>
    inline uint32_t InternalFsPhy<Features...>::getPhyInterface() const {
      return PCD_PHY_EMBEDDED;
    }


    /**
     * Get the interface speed
     * @return PCD_SPEED_FULL
     */

    template<class... Features>
    inline uint32_t InternalFsPhy<Features...>::getPhySpeed() const {
      return PCD_SPEED_FULL;
    }
  }
}

