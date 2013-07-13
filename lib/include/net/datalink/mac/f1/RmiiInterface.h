/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {
	namespace net {

		/**
		 * Class for use of the MAC in RMII mode on the F107. The only available pin package is MacDefaultPinPackage.
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
				void outputClockOnMCO() const;
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

			// configure in RMII mode and enable remap if defined by Mac_Peripheral

			ClockControl<static_cast<PeripheralName>(TPinPackage::Mac_Peripheral)>::On();

			GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);

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
		 * Enable output of a 25Mhz clock on the MCO pin for clocking the PHY. This requires that the
		 * STM32 is being clocked from a 25Mhz HSE
		 */

		template<class TPhysicalLayer,class TPinPackage>
		void RmiiInterface<TPhysicalLayer,TPinPackage>::outputClockOnMCO() const {

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

		template<class TPhysicalLayer,class TPinPackage>
		inline void RmiiInterface<TPhysicalLayer,TPinPackage>::initPins() const {

			// RMII pins

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_TXD0,TPinPackage::Pin_RMII_TXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_TXD1,TPinPackage::Pin_RMII_TXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_TXEN,TPinPackage::Pin_RMII_TXEN,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_RXD0,TPinPackage::Pin_RMII_RXD0,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_RXD1,TPinPackage::Pin_RMII_RXD1,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_CRSDV,TPinPackage::Pin_RMII_CRSDV,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_REFCLK,TPinPackage::Pin_RMII_REFCLK,Gpio::INPUT,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,0);

			// PHY pins

			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MDC,TPinPackage::Pin_MDC,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
			GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MDIO,TPinPackage::Pin_MDIO,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,0);
		}
	}
}
