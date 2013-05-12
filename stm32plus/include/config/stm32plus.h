/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
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
 *  STM32PLUS_F1_CL (not yet fully tested)
 *  STM32PLUS_F4
 *
 *  Also ensure that HSE_VALUE has been set to the speed of your external oscillator in hertz. The scons
 *  build procedure takes care of all these definitions.
 */

/*
 * Verify that an MCU has been selected
 */

#if defined (STM32PLUS_F1_HD)
	#define STM32PLUS_F1
	#define STM32F10X_HD
#elif defined (STM32PLUS_F1_CL)
	#define STM32PLUS_F1
	#define STM32F10X_CL
#elif defined(STM32PLUS_F4)
	// no additional defines
#else
	#error "You must define an MCU type. See config/stm32plus.h"
#endif


/*
 * Verify that HSE_VALUE is defined
 */

#if !defined(HSE_VALUE)
#error "Please define HSE_VALUE to the frequency of your external oscillator in hertz"
#endif


/*
 * Flag that we are an STM32PLUS project
 */

#undef STM32PLUS_BUILD
#define STM32PLUS_BUILD 0x020000


/**
 * @namespace stm32plus 'stm32plus' is the primary namespace for everything inside the library. There are
 * also some sub-namespaces such as 'display' for the graphics code.
 */

namespace stm32plus {


	/**
	 * MCU Series names
	 */

	enum McuSeries {
		SERIES_F1,	//!< SERIES_F1
		SERIES_F2,	//!< SERIES_F2
		SERIES_F4 	//!< SERIES_F4
	};


	/**
	 * Peripheral names
	 */

	enum PeripheralName {
		PERIPHERAL_BACKUP,							//!< PERIPHERAL_BACKUP
		PERIPHERAL_CRC,									//!< PERIPHERAL_CRC
		PERIPHERAL_DAC1,								//!< PERIPHERAL_DAC1
		PERIPHERAL_DAC2,								//!< PERIPHERAL_DAC2
		PERIPHERAL_DMA1,								//!< PERIPHERAL_DMA1
		PERIPHERAL_DMA2,								//!< PERIPHERAL_DMA2
		PERIPHERAL_GPIO,   							//!< PERIPHERAL_GPIO
		PERIPHERAL_FSMC,   							//!< PERIPHERAL_FSMC
		PERIPHERAL_I2C1,								//!< PERIPHERAL_I2C1
		PERIPHERAL_I2C2,								//!< PERIPHERAL_I2C2
		PERIPHERAL_I2C3,								//!< PERIPHERAL_I2C3
		PERIPHERAL_I2S2,								//!< PERIPHERAL_I2S2
		PERIPHERAL_I2S3,								//!< PERIPHERAL_I2S3
		PERIPHERAL_MAC,									//!< PERIPHERAL_MAC
		PERIPHERAL_POWER,								//!< PERIPHERAL_POWER
		PERIPHERAL_SDIO,   							//!< PERIPHERAL_SDIO
		PERIPHERAL_SPI1,   							//!< PERIPHERAL_SPI1
		PERIPHERAL_SPI1_REMAP,   				//!< PERIPHERAL_SPI1_REMAP
		PERIPHERAL_SPI2,   							//!< PERIPHERAL_SPI2
		PERIPHERAL_SPI3,   							//!< PERIPHERAL_SPI3
		PERIPHERAL_SPI3_REMAP,   				//!< PERIPHERAL_SPI3_REMAP
		PERIPHERAL_SYSCFG,							//!< PERIPHERAL_SYSCFG
		PERIPHERAL_TIMER1, 							//!< PERIPHERAL_TIMER1
		PERIPHERAL_TIMER2, 							//!< PERIPHERAL_TIMER2
		PERIPHERAL_TIMER3, 							//!< PERIPHERAL_TIMER3
		PERIPHERAL_TIMER4, 							//!< PERIPHERAL_TIMER4
		PERIPHERAL_TIMER5, 							//!< PERIPHERAL_TIMER5
		PERIPHERAL_TIMER6, 							//!< PERIPHERAL_TIMER6
		PERIPHERAL_TIMER7, 							//!< PERIPHERAL_TIMER7
		PERIPHERAL_TIMER8,							//!< PERIPHERAL_TIMER8
		PERIPHERAL_TIMER9, 							//!< PERIPHERAL_TIMER9
		PERIPHERAL_TIMER10,							//!< PERIPHERAL_TIMER10
		PERIPHERAL_TIMER11,							//!< PERIPHERAL_TIMER11
		PERIPHERAL_TIMER12,							//!< PERIPHERAL_TIMER12
		PERIPHERAL_TIMER13,							//!< PERIPHERAL_TIMER13
		PERIPHERAL_TIMER14,							//!< PERIPHERAL_TIMER14
		PERIPHERAL_USART1, 							//!< PERIPHERAL_USART1
		PERIPHERAL_USART1_REMAP,				//!< PERIPHERAL_USART1_REMAP
		PERIPHERAL_USART2, 							//!< PERIPHERAL_USART2
		PERIPHERAL_USART2_REMAP,				//!< PERIPHERAL_USART2_REMAP
		PERIPHERAL_USART3, 							//!< PERIPHERAL_USART3
		PERIPHERAL_USART3_PARTIAL_REMAP,	//!< PERIPHERAL_USART3_PARTIAL_REMAP
		PERIPHERAL_USART3_FULL_REMAP,		//!< PERIPHERAL_USART3_FULL_REMAP
		PERIPHERAL_UART4,  							//!< PERIPHERAL_UART4
		PERIPHERAL_UART5,   						//!< PERIPHERAL_UART5
		PERIPHERAL_USART6   						//!< PERIPHERAL_USART6
	};


	/**
	 * Endian-ness values for when we need it
	 */
	enum Endian {
		LITTLE_ENDIAN,	//!< LITTLE_ENDIAN
		BIG_ENDIAN    	//!< BIG_ENDIAN
	};
}


// include standard libraries

#include <stdint.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cctype>

// include the ST library

#include "config/stdperiph.h"

// error provider is common to everyone

#include "error/ErrorProvider.h"

// include the customisable peripheral traits

#include "config/traits.h"


/**
@mainpage stm32plus documentation
<H1>Introduction</H1>

  Firstly, welcome to STM32PLUS, the C++ library that eases the burden
  of programming the STM32F103 and STM32F4 devices.

  The main introduction and getting started guide can be found at my
  website, http://www.andybrown.me.uk.

<H1>How to compile the library</H1>

  Before you can use the library you need to build it because,
  although much of the library is provided 'header-only' there is a
  substantial amount of compiled code that you must link to.

  Please see the INSTALL.txt file for detailed compilation
  instructions.

<H1>Where are the examples?</H1>

  In the 'examples' subdirectory you will find dozens of examples
  nearly all of which will work without modification on the F1 and F4
  devices. The examples are heavily commented to help you understand
  what's going on.

<H1>Documentation</H1>

  HTML documentation can be found in the doc/html subdirectory. This
  documentation is auto-generated by the 'doxygen' tool from the
  comments in the source code.

  I freely admit that the documentation lags in both quantity and
  quality behind the code itself and it's a future task for me to
  improve it. In the meantime I hope that the heavily commented
  examples are enough to get you started.

<H1>Can i download pre-compiled binaries?</H1>

  Yes you can. Visit the 'downloads' page at my website for a binary
  package that covers all possible MCU combinations. Note that this
  binary package is compiled for an 8MHz external oscillator (HSE).

<H1>A short walk around the directories</H1>

  /CHANGELOG.txt: The change log. In here I'll detail the changes made
  in each release. 2.0.0 is an exception because so much has changed
  that it gets a write-up on my website instead.

  /INSTALL.txt: The installation guide. This file explains how to
  build the library. If you read nothing else, read this!

  /SConstruct: The top level scons build file, broadly equivalent to a
  Makefile for those that have not used scons before.

  /VERSION.txt: Contains the date and time of the build that made this
  release.

  stm32plus/: The root directory containing the library source code.

  stm32plus/include: The include files for the library. This directory
  and the parent stm32plus directory must be on the include path of
  any programs that you write. As of 2.0.0 the only include files that
  you need to know about are those in the 'config' subdirectory. It
  should only ever be necessary to include 'config/stm32plus.h' and
  one each for the peripherals that you want to use, for example
  'config/usart.h' or 'config/spi.h'. These high level files take care
  of including everything else that they need.

  stm32plus/doc: The auto-generated HTML documentation. As noted above
  this documentation is an area slated for improvement in the future.

  stm32plus/src: The C++ source files that make up the
  library. Everything in here is considered internal.

  stm32plus/fwlib: Source code to the ST Microelectronics standard
  peripheral libraries for the F1 and F4 processors.

  examples/: The examples that demonstrate the features of the
  library. There is one subdirectory for each example. All the
  examples follow the same general format. There is the main example
  source code and a 'system' subdirectory. The 'system' subdirectory
  is the same for every example and contains the startup and
  initialisation code required for the F1 and F4 MCU. The SConscript
  file takes care of selecting the appropriate code for your target
  MCU. To build modified example, run scons again from the root
  directory. scons is smart enough to only build changed files and
  their dependents. When I develop the examples in Eclipse I have a
  build configuration for each MCU, e.g. "Debug_F1" and "Debug_F4" and
  I use the "Resource Configurations -> Exclude from build" option to
  exclude the system files for the other MCU.

  utils/bm2rgbi: This PC utility is for converting graphics files
  (jpeg, png, gif etc.) into an internal format suitable for efficient
  transfer to a TFT. It also supports compression using the LZG format
  that results in files roughly the same size as a PNG. You'll need
  this utility if you decide to use the bitmap functions in the
  graphics library.

  utils/FontConv: This PC utility is for converting TrueType bitmap
  fonts such as those you can download for free from www.dafont.com
  into font files suitable for compiling and using with the stm32plus
  text output graphics library functions.

  utils/LzgFontConv: This PC utility is for converting TrueType vector
  anti-aliased fonts into compressed graphical representations
  suitable for compiling and using with the stm32plus bitmap text
  output graphics library functions.
 */
