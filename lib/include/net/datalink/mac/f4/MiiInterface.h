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
     * Class for use of the MAC in MII mode on the F4.
     * @tparam TPinPackage The type containing the enumeration of pins that we are
     * to use for RMII. e.g. MacDefaultPinPackage
     */

    template<class TPhysicalLayer,class TPinPackage>
    class MiiInterface : public virtual TPhysicalLayer {

      public:
        struct Parameters {
        };

      protected:
        void initPins() const;

      public:
        bool initialise(const Parameters&) const;
        bool startup();
        void outputClockOnMCO() const;
    };


    /**
     * Template alias for the common use case
     */

    template<class TPhysicalLayer>
    using DefaultMiiInterface=MiiInterface<TPhysicalLayer,MacDefaultPinPackage>;

    template<class TPhysicalLayer>
    using RemapMiiInterface=MiiInterface<TPhysicalLayer,MacRemapPinPackage>;


    /**
     * Startup the class
     * @return always true
     */

    template<class TPhysicalLayer,class TPinPackage>
    inline bool MiiInterface<TPhysicalLayer,TPinPackage>::startup() {
      return true;
    }


    /**
     * Initialise the ethernet clock.
     * @return always true
     */

    template<class TPhysicalLayer,class TPinPackage>
    inline bool MiiInterface<TPhysicalLayer,TPinPackage>::initialise(const Parameters&) const {

      // configure in MII mode

      ClockControl<PERIPHERAL_SYSCFG>::On();

      ClockControl<static_cast<PeripheralName>(TPinPackage::Mac_Peripheral)>::Off();
      SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_MII);
      ClockControl<static_cast<PeripheralName>(TPinPackage::Mac_Peripheral)>::On();

      // initialise the pins

      initPins();
      return true;
    }


    /**
     * Enable output of a 25Mhz clock on the MCO pin for clocking the PHY. This requires that the
     * STM32 is being clocked from a 25Mhz HSE
     */

    template<class TPhysicalLayer,class TPinPackage>
    void MiiInterface<TPhysicalLayer,TPinPackage>::outputClockOnMCO() const {
      RCC_MCO1Config(RCC_MCO1Source_HSE,RCC_MCO1Div_1);
    }


    /**
     * Initialise the RMII pins
     */

    template<class TPhysicalLayer,class TPinPackage>
    inline void MiiInterface<TPhysicalLayer,TPinPackage>::initPins() const {

       // MII pins

       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_TXCLK::Port,TPinPackage::MII_TXCLK::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_TXD0::Port,TPinPackage::MII_TXD0::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_TXD1::Port,TPinPackage::MII_TXD1::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_TXD2::Port,TPinPackage::MII_TXD2::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_TXD3::Port,TPinPackage::MII_TXD3::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_TXEN::Port,TPinPackage::MII_TXEN::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_RXCLK::Port,TPinPackage::MII_RXCLK::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_RXD0::Port,TPinPackage::MII_RXD0::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_RXD1::Port,TPinPackage::MII_RXD1::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_RXD2::Port,TPinPackage::MII_RXD2::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_RXD3::Port,TPinPackage::MII_RXD3::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_RXER::Port,TPinPackage::MII_RXER::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_RXDV::Port,TPinPackage::MII_RXDV::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_CRS::Port,TPinPackage::MII_CRS::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MII_COL::Port,TPinPackage::MII_COL::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);

       // PHY pins

       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MDC::Port,TPinPackage::MDC::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
       GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::MDIO::Port,TPinPackage::MDIO::Pin,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
    }
  }
}
