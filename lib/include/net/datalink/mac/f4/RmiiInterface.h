/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


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

			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_TXD0,TPinPackage::Pin_RMII_TXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_TXD1,TPinPackage::Pin_RMII_TXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_TXEN,TPinPackage::Pin_RMII_TXEN,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_RXD0,TPinPackage::Pin_RMII_RXD0,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_RXD1,TPinPackage::Pin_RMII_RXD1,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_CRSDV,TPinPackage::Pin_RMII_CRSDV,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_RMII_REFCLK,TPinPackage::Pin_RMII_REFCLK,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);

			 // PHY pins

			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MDC,TPinPackage::Pin_MDC,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
			 GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MDIO,TPinPackage::Pin_MDIO,Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)MacPeripheralTraits::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GPIO_AF_ETH);
		}
	}
}
