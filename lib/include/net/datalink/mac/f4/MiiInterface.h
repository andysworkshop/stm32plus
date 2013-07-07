/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


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

			ClockControl<PERIPHERAL_MAC>::Off();
			SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_MII);
			ClockControl<PERIPHERAL_MAC>::On();

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

			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXCLK,TPinPackage::Pin_MII_TXCLK,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXD0,TPinPackage::Pin_MII_TXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXD1,TPinPackage::Pin_MII_TXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXD2,TPinPackage::Pin_MII_TXD2,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXD3,TPinPackage::Pin_MII_TXD3,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_TXEN,TPinPackage::Pin_MII_TXEN,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXCLK,TPinPackage::Pin_MII_RXCLK,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXD0,TPinPackage::Pin_MII_RXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXD1,TPinPackage::Pin_MII_RXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXD2,TPinPackage::Pin_MII_RXD2,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXD3,TPinPackage::Pin_MII_RXD3,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXER,TPinPackage::Pin_MII_RXER,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_RXDV,TPinPackage::Pin_MII_RXDV,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_CRS,TPinPackage::Pin_MII_CRS,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MII_COL,TPinPackage::Pin_MII_COL,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);

			 // PHY pins

			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MDC,TPinPackage::Pin_MDC,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MDIO,TPinPackage::Pin_MDIO,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
		}
	}
}
