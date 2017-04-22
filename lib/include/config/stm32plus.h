/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This is the primary include file for stm32plus. You should always include it before you include any
 * other config files.
 *
 * Configuration for the library is here. Make sure you have defined one of the MCU selection defines:
 *
 *  STM32PLUS_F1_HD
 *  STM32PLUS_F1_CL
 *  STM32PLUS_F1_MDVL
 *  STM32PLUS_F0_51
 *  STM32PLUS_F401
 *  ... and all the others (see SConstruct for all)
 *
 *  Also ensure that HSE_VALUE has been set to the speed of your external oscillator in hertz. The scons
 *  build procedure takes care of all these definitions.
 */


// include the standard peripheral library

#include "config/stdperiph.h"


/*
 * Verify that HSE_VALUE is defined
 */

#if !defined(HSE_VALUE) && !defined(HSI_VALUE)
#error "Please define HSE_VALUE or HSI_VALUE to the frequency of your external/internal oscillator in hertz"
#endif


/*
 * Flag that we are an STM32PLUS project with the current release version
 */

#undef STM32PLUS_BUILD
#define STM32PLUS_BUILD 0x040100


/**
 * @namespace stm32plus 'stm32plus' is the primary namespace for everything inside the library. There are
 * also some sub-namespaces such as 'display' for the graphics code.
 */

namespace stm32plus {


  /**
   * MCU Series names
   */

  enum McuSeries {
    SERIES_F0,  //!< STM32 F0
    SERIES_F1,  //!< STM32 F1
    SERIES_F4   //!< STM32 F4
  };


  /**
   * Peripheral names
   */

  enum PeripheralName {
    PERIPHERAL_ADC1,                //!< PERIPHERAL_ADC1
    PERIPHERAL_ADC2,                //!< PERIPHERAL_ADC1
    PERIPHERAL_ADC3,                //!< PERIPHERAL_ADC1
    PERIPHERAL_BACKUP,              //!< PERIPHERAL_BACKUP
    PERIPHERAL_CRC,                 //!< PERIPHERAL_CRC
    PERIPHERAL_DAC1,                //!< PERIPHERAL_DAC1
    PERIPHERAL_DAC2,                //!< PERIPHERAL_DAC2
    PERIPHERAL_DMA1,                //!< PERIPHERAL_DMA1
    PERIPHERAL_DMA2,                //!< PERIPHERAL_DMA2
    PERIPHERAL_GPIO,                //!< PERIPHERAL_GPIO
    PERIPHERAL_FSMC,                //!< PERIPHERAL_FSMC
    PERIPHERAL_FMC,                 //!< PERIPHERAL_FMC
    PERIPHERAL_HASH,                //!< PERIPHERAL_HASH
    PERIPHERAL_I2C1,                //!< PERIPHERAL_I2C1
    PERIPHERAL_I2C2,                //!< PERIPHERAL_I2C2
    PERIPHERAL_I2C3,                //!< PERIPHERAL_I2C3
    PERIPHERAL_I2S2,                //!< PERIPHERAL_I2S2
    PERIPHERAL_I2S3,                //!< PERIPHERAL_I2S3
    PERIPHERAL_MAC,                 //!< PERIPHERAL_MAC
    PERIPHERAL_MAC_REMAP,           //!< PERIPHERAL_MAC_REMAP
    PERIPHERAL_OTG_FS,              //!< internal OTG FS PHY
    PERIPHERAL_OTG_HS,              //!< internal OTG HS PHY
    PERIPHERAL_POWER,               //!< PERIPHERAL_POWER
    PERIPHERAL_RNG,                 //!< PERIPHERAL_RNG
    PERIPHERAL_SDIO,                //!< PERIPHERAL_SDIO
    PERIPHERAL_SPI1,                //!< PERIPHERAL_SPI1
    PERIPHERAL_SPI1_REMAP,          //!< PERIPHERAL_SPI1_REMAP
    PERIPHERAL_SPI2,                //!< PERIPHERAL_SPI2
    PERIPHERAL_SPI3,                //!< PERIPHERAL_SPI3
    PERIPHERAL_SPI3_REMAP,          //!< PERIPHERAL_SPI3_REMAP
    PERIPHERAL_SYSCFG,              //!< PERIPHERAL_SYSCFG
    PERIPHERAL_TIMER1,              //!< PERIPHERAL_TIMER1
    PERIPHERAL_TIMER2,              //!< PERIPHERAL_TIMER2
    PERIPHERAL_TIMER3,              //!< PERIPHERAL_TIMER3
    PERIPHERAL_TIMER4,              //!< PERIPHERAL_TIMER4
    PERIPHERAL_TIMER5,              //!< PERIPHERAL_TIMER5
    PERIPHERAL_TIMER6,              //!< PERIPHERAL_TIMER6
    PERIPHERAL_TIMER7,              //!< PERIPHERAL_TIMER7
    PERIPHERAL_TIMER8,              //!< PERIPHERAL_TIMER8
    PERIPHERAL_TIMER9,              //!< PERIPHERAL_TIMER9
    PERIPHERAL_TIMER10,             //!< PERIPHERAL_TIMER10
    PERIPHERAL_TIMER11,             //!< PERIPHERAL_TIMER11
    PERIPHERAL_TIMER12,             //!< PERIPHERAL_TIMER12
    PERIPHERAL_TIMER13,             //!< PERIPHERAL_TIMER13
    PERIPHERAL_TIMER14,             //!< PERIPHERAL_TIMER14
    PERIPHERAL_TIMER15,             //!< PERIPHERAL_TIMER15
    PERIPHERAL_TIMER16,             //!< PERIPHERAL_TIMER16
    PERIPHERAL_TIMER17,             //!< PERIPHERAL_TIMER17
    PERIPHERAL_USART1,              //!< PERIPHERAL_USART1
    PERIPHERAL_USART1_REMAP,        //!< PERIPHERAL_USART1_REMAP
    PERIPHERAL_USART2,              //!< PERIPHERAL_USART2
    PERIPHERAL_USART2_REMAP,        //!< PERIPHERAL_USART2_REMAP
    PERIPHERAL_USART3,              //!< PERIPHERAL_USART3
    PERIPHERAL_USART3_PARTIAL_REMAP,  //!< PERIPHERAL_USART3_PARTIAL_REMAP
    PERIPHERAL_USART3_FULL_REMAP,   //!< PERIPHERAL_USART3_FULL_REMAP
    PERIPHERAL_UART4,               //!< PERIPHERAL_UART4
    PERIPHERAL_UART5,               //!< PERIPHERAL_UART5
    PERIPHERAL_USART6,              //!< PERIPHERAL_USART6
    PERIPHERAL_CAN1,				        //!< PERIPHERAL_CAN1
    PERIPHERAL_CAN2					        //!< PERIPHERAL_CAN2
  };


  /**
   * Endian-ness values for when we need it. Careful: BIG_ENDIAN and LITTLE_ENDIAN
   * are defined as macros.
   */

  enum class Endian {
    LITTLE_ENDIAN_MCU,  //!< little endian
    BIG_ENDIAN_MCU      //!< big endian
  };
}


// include standard libraries

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cctype>

// error provider is common to everyone

#include "error/ErrorProvider.h"


// include the customisable peripheral traits

#include "config/traits.h"


/**
@mainpage stm32plus documentation
<H1>README and INSTALL are on Github</H1>

  Please browse to https://github.com/andysworkshop/stm32plus
 */
