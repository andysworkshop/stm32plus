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

			// configure in MII mode and enable remap if defined by Mac_Peripheral

			ClockControl<static_cast<PeripheralName>(TPinPackage::Mac_Peripheral)>::On();

			GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);

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
		  RCC_MCOConfig(RCC_MCO_HSE);
		}


		/**
		 * Initialise the RMII pins
		 */

		template<class TPhysicalLayer,class TPinPackage>
		inline void MiiInterface<TPhysicalLayer,TPinPackage>::initPins() const {

			// MII pins

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXCLK,TPinPackage::Pin_MII_TXCLK,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXD0,TPinPackage::Pin_MII_TXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXD1,TPinPackage::Pin_MII_TXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXD2,TPinPackage::Pin_MII_TXD2,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXD3,TPinPackage::Pin_MII_TXD3,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXEN,TPinPackage::Pin_MII_TXEN,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXCLK,TPinPackage::Pin_MII_RXCLK,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXD0,TPinPackage::Pin_MII_RXD0,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXD1,TPinPackage::Pin_MII_RXD1,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXD2,TPinPackage::Pin_MII_RXD2,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXD3,TPinPackage::Pin_MII_RXD3,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXER,TPinPackage::Pin_MII_RXER,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXDV,TPinPackage::Pin_MII_RXDV,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_CRS,TPinPackage::Pin_MII_CRS,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_COL,TPinPackage::Pin_MII_COL,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);

			// PHY pins

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MDC,TPinPackage::Pin_MDC,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MDIO,TPinPackage::Pin_MDIO,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
		}
	}
}
