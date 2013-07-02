/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * @file
 * This internal include file is included for you by the top-level stm32plus.h config file. Its purpose is to
 * include everything that's required from the ST Microelectronics Standard Peripheral Library.
 */

#if defined (STM32PLUS_F1_HD) || defined(STM32PLUS_F1_HD_CL)

#include "misc.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_can.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_cec.h"
#include "stm32f10x_wwdg.h"

#elif defined(STM32PLUS_F3)

#include "stm32f30x_misc.h"
#include "stm32f30x_adc.h"
#include "stm32f30x_can.h"
#include "stm32f30x_comp.h"
#include "stm32f30x_crc.h"
#include "stm32f30x_dac.h"
#include "stm32f30x_dbgmcu.h"
#include "stm32f30x_dma.h"
#include "stm32f30x_exti.h"
#include "stm32f30x_flash.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_i2c.h"
#include "stm32f30x_iwdg.h"
#include "stm32f30x_opamp.h"
#include "stm32f30x_pwr.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_rtc.h"
#include "stm32f30x_spi.h"
#include "stm32f30x_syscfg.h"
#include "stm32f30x_tim.h"
#include "stm32f30x_usart.h"
#include "stm32f30x_wwdg.h"

/* Addition to provide some symbolic names as in other cores: */
#include "stm32f30x_gpio_alt.h"

#elif defined(STM32PLUS_F4)

#include "misc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_crc.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_sdio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_fsmc.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_wwdg.h"
#include "stm32f4xx_syscfg.h"

#else

#error STM32PLUS_Fn macro has not been defined: check config/stm32plus.h

#endif

