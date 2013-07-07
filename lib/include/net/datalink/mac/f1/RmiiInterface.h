/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
	namespace net {

		/**
		 * Class for use of the MAC in RMII mode on the F107. The only available pin package is MacDefaultPinPackage.
		 */

		template<class TPhysicalLayer>
		class RmiiInterface : public virtual TPhysicalLayer {

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
		using DefaultRmiiInterface=RmiiInterface<TPhysicalLayer>;


		/**
		 * Initialise the class
		 * @return always true
		 */

		template<class TPhysicalLayer>
		inline bool RmiiInterface<TPhysicalLayer>::initialise(const Parameters&) const {

			// configure in RMII mode

			ClockControl<PERIPHERAL_MAC>::On();

			GPIO_PinRemapConfig(GPIO_Remap_ETH,ENABLE);
			GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);

			// initialise the pins

			initPins();
			return true;
		}


		/**
		 * Startup the class
		 * @return always true
		 */

		template<class TPhysicalLayer>
		inline bool RmiiInterface<TPhysicalLayer>::startup() {
			return true;
		}


		/**
		 * Enable output of a 25Mhz clock on the MCO pin for clocking the PHY. This requires that the
		 * STM32 is being clocked from a 25Mhz HSE
		 */

		template<class TPhysicalLayer>
		void RmiiInterface<TPhysicalLayer>::outputClockOnMCO() const {

		  // set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz)

			RCC_PLL3Config(RCC_PLL3Mul_10);

			// Enable PLL3

			RCC_PLL3Cmd(ENABLE);

			// Wait till PLL3 is ready

			while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY)==RESET);

			// Get clock PLL3 clock on PA8 pin

			RCC_MCOConfig(RCC_MCO_PLL3CLK);
		}


		/**
		 * Initialise the RMII pins
		 */

		template<class TPhysicalLayer>
		inline void RmiiInterface<TPhysicalLayer>::initPins() const {

			// RMII pins

			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_RMII_TXD0,MacDefaultPinPackage::Pin_RMII_TXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_RMII_TXD1,MacDefaultPinPackage::Pin_RMII_TXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_RMII_TXEN,MacDefaultPinPackage::Pin_RMII_TXEN,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_RMII_RXD0,MacDefaultPinPackage::Pin_RMII_RXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_RMII_RXD1,MacDefaultPinPackage::Pin_RMII_RXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_RMII_CRSDV,MacDefaultPinPackage::Pin_RMII_CRSDV,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_RMII_REFCLK,MacDefaultPinPackage::Pin_RMII_REFCLK,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);

			// PHY pins

			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MDC,MacDefaultPinPackage::Pin_MDC,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)MacDefaultPinPackage::Port_MDIO,MacDefaultPinPackage::Pin_MDIO,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
		}
	}
}
