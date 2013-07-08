/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {


#define FULL_PARAMETER_PACK uint8_t p1=16,uint8_t p2=16,uint8_t p3=16,uint8_t p4=16,uint8_t p5=16,uint8_t p6=16,uint8_t p7=16,uint8_t p8=16,uint8_t p9=16,uint8_t p10=16,uint8_t p11=16,uint8_t p12=16,uint8_t p13=16,uint8_t p14=16,uint8_t p15=16,uint8_t p16=16
#define SLIM_PARAMETER_PACK p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16


	/**
	 * Template feature class to support initialisation of alternate function pins on the F4.
	 * Multiple pins can be initialised in one instantiation
	 */

	template<uint8_t TAfSelection,
					 GPIOSpeed_TypeDef TSpeed=GPIO_Speed_50MHz,
					 Gpio::GpioOutputType TOutputType=Gpio::PUSH_PULL,
					 Gpio::GpioPullUpDownType TPullUpDownType=Gpio::PUPD_NONE,
					 FULL_PARAMETER_PACK>
	class AlternateFunctionFeature : public Gpio {

		public:

		/**
		 * Constructor
		 * @param port the port reference
		 */

			AlternateFunctionFeature(GpioPortBase& port)
				: Gpio(port) {

				uint32_t pins;

				// p1..16 are constants. the compiler will eliminate the impossible branches
				// so the code generated here is minimal. In an optimised build all the 16 conditionals below
				// are eliminated and the compiler generates a single load instruction with a constant.

				pins=0;

				if(p1!=16)
					pins|=1 << p1;

				if(p2!=16)
					pins|=1 << p2;

				if(p3!=16)
					pins|=1 << p3;

				if(p4!=16)
					pins|=1 << p4;

				if(p5!=16)
					pins|=1 << p5;

				if(p6!=16)
					pins|=1 << p6;

				if(p7!=16)
					pins|=1 << p7;

				if(p8!=16)
					pins|=1 << p8;

				if(p9!=16)
					pins|=1 << p9;

				if(p10!=16)
					pins|=1 << p10;

				if(p11!=16)
					pins|=1 << p11;

				if(p12!=16)
					pins|=1 << p12;

				if(p13!=16)
					pins|=1 << p13;

				if(p14!=16)
					pins|=1 << p14;

				if(p15!=16)
					pins|=1 << p15;

				if(p16!=16)
					pins|=1 << p16;

				// do the initialisation in the non-template base

				initialiseAF(pins,TAfSelection,TSpeed,TOutputType,TPullUpDownType);
			}
	};


	/**
	 * Default alternate function helper. This provides default port speed settings and is only
	 * compatible with the F4.
	 */

	template<uint8_t TAfSelection,FULL_PARAMETER_PACK>
	using DefaultAlternateFunctionFeature=AlternateFunctionFeature<TAfSelection,GPIO_Speed_50MHz,Gpio::PUSH_PULL,Gpio::PUPD_NONE,SLIM_PARAMETER_PACK>;


	/**
	 * Types for each of the peripherals supporting AFIO. These are the compatibility types and will work
	 * on any MCU. Some of the examples use these to guarantee compatibility.
	 */

	template<FULL_PARAMETER_PACK> using DefaultTim1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM1,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM2,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim3AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM3,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim4AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM4,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim5AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM5,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim8AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM8,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim9AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM9,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim10AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM10,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim11AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM11,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim12AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM12,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim13AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM13,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultTim14AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM14,SLIM_PARAMETER_PACK>;

	template<FULL_PARAMETER_PACK> using DefaultI2C1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_I2C1,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultI2C2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_I2C2,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultI2C3AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_I2C3,SLIM_PARAMETER_PACK>;

	template<FULL_PARAMETER_PACK> using DefaultSpi1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_SPI1,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultSpi2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_SPI2,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultSpi3AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_SPI3,SLIM_PARAMETER_PACK>;

	template<FULL_PARAMETER_PACK> using DefaultUsart1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_USART1,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultUsart2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_USART1,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultUsart3AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_USART3,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultUart4AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_UART4,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultUart5AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_UART5,SLIM_PARAMETER_PACK>;
	template<FULL_PARAMETER_PACK> using DefaultUsart6AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_USART6,SLIM_PARAMETER_PACK>;

	template<FULL_PARAMETER_PACK> using DefaultFsmcAlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_FSMC,SLIM_PARAMETER_PACK>;

	template<FULL_PARAMETER_PACK> using DefaultSdioAlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_SDIO,SLIM_PARAMETER_PACK>;

#undef FULL_PARAMETER_PACK				// don't let this escape into the global namespace
#undef SLIM_PARAMETER_PACK				// don't let this escape into the global namespace
}
