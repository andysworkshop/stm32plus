/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


/**
 * Use the selected MCU family to set up other definitions used to include and exclude
 * features. Some are used by stm32plus and some are used by the ST peripheral and CMSIS
 * libraries.
 *
 * The _HAS_ macros are considered private to stm32plus and may change without warning
 * with new library releases.
 */


#if defined(STM32PLUS_F1_HD)
  #define STM32PLUS_F1                // enable features common to all F1
  #define STM32F10X_HD                // for the std peripheral library

#elif defined(STM32PLUS_F1_CL_E)
  #define STM32PLUS_F1                // enable features common to all F1
  #define STM32PLUS_F1_CL             // enable features common to F105 and F107
  #define STM32F10X_CL                // for the std peripheral library

#elif defined(STM32PLUS_F1_MD_VL)
  #define STM32PLUS_F1                // enable features common to all F1
  #define STM32PLUS_F1_VL             // enable features common to all VL devices
  #define STM32F10X_MD_VL             // for the std peripheral library

#elif defined(STM32PLUS_F1_MD)
  #define STM32PLUS_F1                // enable features common to all F1
  #define STM32F10X_MD                // for the std peripheral library

#elif defined(STM32PLUS_F0_51) || defined(STM32PLUS_F0_30)
  #define STM32PLUS_F0                // enable features common to all F0
  #define STM32F0XX_MD                // these are medium density devices
#elif defined(STM32PLUS_F0_42)
  #define STM32PLUS_F0
  #define STM32F0XX
#elif defined(STM32PLUS_F405)
  #define STM32PLUS_F4
  #define STM32F405xx
  #define STM32F40_41xxx
  #define STM32PLUS_F4_HAS_CAN
  #define STM32PLUS_F4_HAS_ADC2_3
  #define STM32PLUS_F4_HAS_USART3_4_5
  #define STM32PLUS_F4_HAS_GPIOF_G_I
  #define STM32PLUS_F4_HAS_TIM6_7_8_12_13_14
  #define STM32PLUS_F4_HAS_OTG_HS
  #define STM32PLUS_F4_HAS_DAC
  #define STM32PLUS_F4_HAS_FSMC

#elif defined(STM32PLUS_F407)
  #define STM32PLUS_F4
  #define STM32F407xx
  #define STM32F40_41xxx
  #define STM32PLUS_F4_HAS_MAC
  #define STM32PLUS_F4_HAS_DCMI
  #define STM32PLUS_F4_HAS_CAN
  #define STM32PLUS_F4_HAS_ADC2_3
  #define STM32PLUS_F4_HAS_USART3_4_5
  #define STM32PLUS_F4_HAS_GPIOF_G_I
  #define STM32PLUS_F4_HAS_TIM6_7_8_12_13_14
  #define STM32PLUS_F4_HAS_OTG_HS
  #define STM32PLUS_F4_HAS_DAC
  #define STM32PLUS_F4_HAS_FSMC

#elif defined(STM32PLUS_F415)
  #define STM32PLUS_F4
  #define STM32F415xx
  #define STM32F40_41xxx
  #define STM32PLUS_F4_HAS_CRYPTO
  #define STM32PLUS_F4_HAS_CAN
  #define STM32PLUS_F4_HAS_ADC2_3
  #define STM32PLUS_F4_HAS_USART3_4_5
  #define STM32PLUS_F4_HAS_GPIOF_G_I
  #define STM32PLUS_F4_HAS_TIM6_7_8_12_13_14
  #define STM32PLUS_F4_HAS_OTG_HS
  #define STM32PLUS_F4_HAS_DAC
  #define STM32PLUS_F4_HAS_FSMC

#elif defined(STM32PLUS_F417)
  #define STM32PLUS_F4
  #define STM32F417xx
  #define STM32F40_41xxx
  #define STM32PLUS_F4_HAS_MAC
  #define STM32PLUS_F4_HAS_DCMI
  #define STM32PLUS_F4_HAS_CRYPTO
  #define STM32PLUS_F4_HAS_CAN
  #define STM32PLUS_F4_HAS_ADC2_3
  #define STM32PLUS_F4_HAS_USART3_4_5
  #define STM32PLUS_F4_HAS_GPIOF_G_I
  #define STM32PLUS_F4_HAS_TIM6_7_8_12_13_14
  #define STM32PLUS_F4_HAS_OTG_HS
  #define STM32PLUS_F4_HAS_DAC
  #define STM32PLUS_F4_HAS_FSMC

#elif defined(STM32PLUS_F427)
  #define STM32PLUS_F4
  #define STM32F427xx
  #define STM32F427_437xx
  #define STM32PLUS_F4_HAS_MAC
  #define STM32PLUS_F4_HAS_DCMI
  #define STM32PLUS_F4_HAS_SAI
  #define STM32PLUS_F4_HAS_DMA2D
  #define STM32PLUS_F4_HAS_CAN
  #define STM32PLUS_F4_HAS_ADC2_3
  #define STM32PLUS_F4_HAS_USART3_4_5
  #define STM32PLUS_F4_HAS_GPIOF_G_I
  #define STM32PLUS_F4_HAS_GPIOJ_K
  #define STM32PLUS_F4_HAS_TIM6_7_8_12_13_14
  #define STM32PLUS_F4_HAS_OTG_HS
  #define STM32PLUS_F4_HAS_DAC
  #define STM32PLUS_F4_HAS_FMC

#elif defined(STM32PLUS_F429)
  #define STM32PLUS_F4
  #define STM32F429xx
  #define STM32F429_439xx
  #define STM32PLUS_F4_HAS_MAC
  #define STM32PLUS_F4_HAS_DCMI
  #define STM32PLUS_F4_HAS_SAI
  #define STM32PLUS_F4_HAS_LTDC
  #define STM32PLUS_F4_HAS_DMA2D
  #define STM32PLUS_F4_HAS_CAN
  #define STM32PLUS_F4_HAS_ADC2_3
  #define STM32PLUS_F4_HAS_USART3_4_5
  #define STM32PLUS_F4_HAS_GPIOF_G_I
  #define STM32PLUS_F4_HAS_GPIOJ_K
  #define STM32PLUS_F4_HAS_TIM6_7_8_12_13_14
  #define STM32PLUS_F4_HAS_OTG_HS
  #define STM32PLUS_F4_HAS_DAC
  #define STM32PLUS_F4_HAS_FMC

#elif defined(STM32PLUS_F437)
  #define STM32PLUS_F4
  #define STM32F437xx
  #define STM32F427_437xx
  #define STM32PLUS_F4_HAS_MAC
  #define STM32PLUS_F4_HAS_DCMI
  #define STM32PLUS_F4_HAS_SAI
  #define STM32PLUS_F4_HAS_CRYPTO
  #define STM32PLUS_F4_HAS_DMA2D
  #define STM32PLUS_F4_HAS_CAN
  #define STM32PLUS_F4_HAS_ADC2_3
  #define STM32PLUS_F4_HAS_USART3_4_5
  #define STM32PLUS_F4_HAS_GPIOF_G_I
  #define STM32PLUS_F4_HAS_GPIOJ_K
  #define STM32PLUS_F4_HAS_TIM6_7_8_12_13_14
  #define STM32PLUS_F4_HAS_OTG_HS
  #define STM32PLUS_F4_HAS_DAC
  #define STM32PLUS_F4_HAS_FMC

#elif defined(STM32PLUS_F439)
  #define STM32PLUS_F4
  #define STM32F439xx
  #define STM32F429_439xx
  #define STM32PLUS_F4_HAS_MAC
  #define STM32PLUS_F4_HAS_DCMI
  #define STM32PLUS_F4_HAS_SAI
  #define STM32PLUS_F4_HAS_CRYPTO
  #define STM32PLUS_F4_HAS_LTDC
  #define STM32PLUS_F4_HAS_DMA2D
  #define STM32PLUS_F4_HAS_CAN
  #define STM32PLUS_F4_HAS_ADC2_3
  #define STM32PLUS_F4_HAS_USART3_4_5
  #define STM32PLUS_F4_HAS_GPIOF_G_I
  #define STM32PLUS_F4_HAS_GPIOJ_K
  #define STM32PLUS_F4_HAS_TIM6_7_8_12_13_14
  #define STM32PLUS_F4_HAS_OTG_HS
  #define STM32PLUS_F4_HAS_DAC
  #define STM32PLUS_F4_HAS_FMC

#else
  #error "You must define an MCU type. See config/stm32plus.h"
#endif
