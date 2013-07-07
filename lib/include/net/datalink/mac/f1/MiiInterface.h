/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace net {

		/**
		 * Class for use of the MAC in MII mode on the F107. The only available pin package is MacDefaultPinPackage.
		 */

		template<class TPhysicalLayer>
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
		using DefaultMiiInterface=MiiInterface<TPhysicalLayer>;


		/**
		 * Startup the class
		 * @return always true
		 */

		template<class TPhysicalLayer>
		inline bool MiiInterface<TPhysicalLayer>::startup() {
			return true;
		}


		/**
		 * Initialise the ethernet clock.
		 * @return always true
		 */

		template<class TPhysicalLayer>
		inline bool MiiInterface<TPhysicalLayer>::initialise(const Parameters&) const {

			// configure in MII mode

			ClockControl<PERIPHERAL_MAC>::On();

			GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);

			// initialise the pins

			initPins();
			return true;
		}


		/**
		 * Enable output of a 25Mhz clock on the MCO pin for clocking the PHY. This requires that the
		 * STM32 is being clocked from a 25Mhz HSE
		 */

		template<class TPhysicalLayer>
		void MiiInterface<TPhysicalLayer>::outputClockOnMCO() const {
		  RCC_MCOConfig(RCC_MCO_HSE);
		}


		/**
		 * Initialise the RMII pins
		 */

		template<class TPhysicalLayer>
		inline void MiiInterface<TPhysicalLayer>::initPins() const {

			// MII pins

			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_TXCLK,MacDefaultPinPackage::Pin_MII_TXCLK,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_TXD0,MacDefaultPinPackage::Pin_MII_TXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_TXD1,MacDefaultPinPackage::Pin_MII_TXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_TXD2,MacDefaultPinPackage::Pin_MII_TXD2,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_TXD3,MacDefaultPinPackage::Pin_MII_TXD3,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_TXEN,MacDefaultPinPackage::Pin_MII_TXEN,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_RXCLK,MacDefaultPinPackage::Pin_MII_RXCLK,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_RXD0,MacDefaultPinPackage::Pin_MII_RXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_RXD1,MacDefaultPinPackage::Pin_MII_RXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_RXD2,MacDefaultPinPackage::Pin_MII_RXD2,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_RXD3,MacDefaultPinPackage::Pin_MII_RXD3,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_RXER,MacDefaultPinPackage::Pin_MII_RXER,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_RXDV,MacDefaultPinPackage::Pin_MII_RXDV,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_CRS,MacDefaultPinPackage::Pin_MII_CRS,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MII_COL,MacDefaultPinPackage::Pin_MII_COL,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);

			// PHY pins

			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MDC,MacDefaultPinPackage::Pin_MDC,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MDIO,MacDefaultPinPackage::Pin_MDIO,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
		}
	}
}
