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

	/**
	 * CRC
	 */

	template<>
	struct ClockControl<PERIPHERAL_CRC> {

		static void On() {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);
		}

		static void Off() {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,DISABLE);
		}
	};

	/**
	 * DMA1
	 */

	template<>
	struct ClockControl<PERIPHERAL_DMA1> {

		static void On() {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
		}

		static void Off() {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,DISABLE);
		}
	};

	/**
	 * DMA2
	 */

	template<>
	struct ClockControl<PERIPHERAL_DMA2> {

		static void On() {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
		}

		static void Off() {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,DISABLE);
		}
	};


	/*
	 * Ethernet MAC
	 */

	template<>
	struct ClockControl<PERIPHERAL_MAC> {

		// configure Tx, Rx. PTP is not enabled here.

		static void On() {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx | RCC_AHB1Periph_ETH_MAC_Rx,ENABLE);
		}

		static void Off() {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx | RCC_AHB1Periph_ETH_MAC_Rx,DISABLE);
		}
	};


	/**
	 * FSMC
	 */

	template<>
	struct ClockControl<PERIPHERAL_FSMC> {

		static void On() {
			RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);
		}

		static void Off() {
			RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,DISABLE);
		}
	};

	/*
	 * I2C3
	 */

	template<>
	struct ClockControl<PERIPHERAL_I2C3> {

		static void On() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3,ENABLE);
		}

		static void Off() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3,DISABLE);
		}
	};

	/**
	 * I2S2
	 */

	template<>
	struct ClockControl<PERIPHERAL_I2S2> {

		static void On() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
			RCC_PLLI2SCmd(ENABLE);
		}

		static void Off() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,DISABLE);
			RCC_PLLI2SCmd(DISABLE);
		}
	};

	/**
	 * I2S3
	 */

	template<>
	struct ClockControl<PERIPHERAL_I2S3> {

		static void On() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
			RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
			RCC_PLLI2SCmd(ENABLE);
			RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY);
		}

		static void Off() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,DISABLE);
			RCC_PLLI2SCmd(DISABLE);
		}
	};

	/**
	 * POWER
	 */

	template<>
	struct ClockControl<PERIPHERAL_POWER> {

		static void On() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		}

		static void Off() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,DISABLE);
		}
	};

	/**
	 * SDIO
	 */

	template<>
	struct ClockControl<PERIPHERAL_SDIO> {

		static void On() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO,ENABLE);
		}

		static void Off() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO,DISABLE);
		}
	};

	/**
	 * SPI1
	 */

	template<>
	struct ClockControl<PERIPHERAL_SPI1> {

		static void On() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
		}

		static void Off() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,DISABLE);
		}
	};

	/**
	 * SPI1_REMAP
	 */

	template<>
	struct ClockControl<PERIPHERAL_SPI1_REMAP> {

		static void On() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
		}

		static void Off() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,DISABLE);
		}
	};

	/**
	 * SPI3_REMAP
	 */

	template<>
	struct ClockControl<PERIPHERAL_SPI3_REMAP> {

		static void On() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
		}

		static void Off() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,DISABLE);
		}
	};

	/**
	 * SYSCFG
	 */

	template<>
	struct ClockControl<PERIPHERAL_SYSCFG> {

		static void On() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
		}

		static void Off() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,DISABLE);
		}
	};

	/**
	 * USART1
	 */

	template<>
	struct ClockControl<PERIPHERAL_USART1> {

		static void On() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		}

		static void Off() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,DISABLE);
		}
	};

	/**
	 * USART2
	 */

	template<>
	struct ClockControl<PERIPHERAL_USART2> {

		static void On() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		}

		static void Off() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,DISABLE);
		}
	};

	/**
	 * USART3
	 */

	template<>
	struct ClockControl<PERIPHERAL_USART3> {

		static void On() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		}

		static void Off() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,DISABLE);
		}
	};

	/**
	 * UART4
	 */

	template<>
	struct ClockControl<PERIPHERAL_UART4> {

		static void On() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
		}

		static void Off() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,DISABLE);
		}
	};

	/**
	 * UART5
	 */

	template<>
	struct ClockControl<PERIPHERAL_UART5> {

		static void On() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
		}

		static void Off() {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,DISABLE);
		}
	};

	/**
	 * USART6
	 */

	template<>
	struct ClockControl<PERIPHERAL_USART6> {

		static void On() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
		}

		static void Off() {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,DISABLE);
		}
	};
}
