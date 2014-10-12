/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {
  namespace net {

    /**
     * Class for use of the MAC in RMII mode on the F4.
     * @tparam TPinPackage The type containing the enumeration of pins that we are
     * to use for RMII. e.g. MacDefaultPinPackage
     */

    template<class TPhysicalLayer,class TPinPackage>
    class RmiiInterface : public virtual TPhysicalLayer {

      public:
        struct Parameters {
        };

      protected:
        void initPins() const;

      public:
        bool initialise(const Parameters&) const;
        bool startup();
    };


    /**
     * Template alias for the common use case
     */

    template<class TPhysicalLayer>
    using DefaultRmiiInterface=RmiiInterface<TPhysicalLayer,MacDefaultPinPackage>;

    template<class TPhysicalLayer>
    using RemapRmiiInterface=RmiiInterface<TPhysicalLayer,MacRemapPinPackage>;


    /**
     * Initialise the class
     * @return always true
     */

    template<class TPhysicalLayer,class TPinPackage>
    inline bool RmiiInterface<TPhysicalLayer,TPinPackage>::initialise(const Parameters&) const {

      // configure in RMII mode

      ClockControl<PERIPHERAL_SYSCFG>::On();

      ClockControl<static_cast<PeripheralName>(TPinPackage::Mac_Peripheral)>::Off();
      SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);
      ClockControl<static_cast<PeripheralName>(TPinPackage::Mac_Peripheral)>::On();

      // initialise the pins

      initPins();
      return true;
    }


    /**
     * Startup the class
     * @return always true
     */

    template<class TPhysicalLayer,class TPinPackage>
    inline bool RmiiInterface<TPhysicalLayer,TPinPackage>::startup() {
      return true;
    }


    /**
     * Initialise the RMII pins
     */

    template<class TPhysicalLayer,class TPinPackage>
    inline void RmiiInterface<TPhysicalLayer,TPinPackage>::initPins() const {

       // RMII pins

       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::RMII_TXD0::Port,TPinPackage::RMII_TXD0::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::RMII_TXD1::Port,TPinPackage::RMII_TXD1::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::RMII_TXEN::Port,TPinPackage::RMII_TXEN::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::RMII_RXD0::Port,TPinPackage::RMII_RXD0::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::RMII_RXD1::Port,TPinPackage::RMII_RXD1::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::RMII_CRSDV::Port,TPinPackage::RMII_CRSDV::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::RMII_REFCLK::Port,TPinPackage::RMII_REFCLK::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);

       // PHY pins

       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MDC::Port,TPinPackage::MDC::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MDIO::Port,TPinPackage::MDIO::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
    }
  }
}
