/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This file provides access to all DMA functionality. Normally you don't have to include this file because it's included
 * as a dependency of any peripheral that has DMA access. An exception is if you're planning to use memory-to-memory
 * fill or copy. In those cases you do need to include this file.
 */

// dma depends on rcc, nvic, event, smart pointers, Timer

#include "config/rcc.h"
#include "config/nvic.h"
#include "config/event.h"
#include "config/timer.h"

#include "memory/scoped_array.h"

// generic includes

#include "dma/DmaEventSource.h"
#include "dma/features/DmaLcdWriter.h"

// device-specific peripheral includes

#if defined(STM32PLUS_F1)

  #define USE_DMA1_1_INTERRUPT
  #define USE_DMA1_2_INTERRUPT
  #define USE_DMA1_3_INTERRUPT
  #define USE_DMA1_4_INTERRUPT
  #define USE_DMA1_5_INTERRUPT
  #define USE_DMA1_6_INTERRUPT
  #define USE_DMA1_7_INTERRUPT
  #define USE_DMA2_1_INTERRUPT
  #define USE_DMA2_2_INTERRUPT
  #define USE_DMA2_3_INTERRUPT
  #define USE_DMA2_4_INTERRUPT        // CL only
  #define USE_DMA2_5_INTERRUPT        // CL only
  #define USE_DMA2_4_5_INTERRUPT      // non-CL only

  #include "dma/f1/Dma.h"
  #include "dma/features/DmaFeatureBase.h"

  #include "dma/f1/Dma1Channel1.h"
  #include "dma/f1/Dma1Channel2.h"
  #include "dma/f1/Dma1Channel3.h"
  #include "dma/f1/Dma1Channel4.h"
  #include "dma/f1/Dma1Channel5.h"
  #include "dma/f1/Dma1Channel6.h"
  #include "dma/f1/Dma1Channel7.h"
  #include "dma/f1/Dma2Channel1.h"
  #include "dma/f1/Dma2Channel2.h"
  #include "dma/f1/Dma2Channel3.h"
  #include "dma/f1/Dma2Channel4.h"
  #include "dma/f1/Dma2Channel5.h"

  #if defined(STM32PLUS_F1_HD) || defined(STM32PLUS_FL_XL)
    #include "dma/features/f1/xl_hd/DmaInterruptFeature.h"
  #elif defined(STM32PLUS_F1_CL)
    #include "dma/features/f1/cl/DmaInterruptFeature.h"
#elif defined(STM32PLUS_F1_MD)
#include "dma/features/f1/md/DmaInterruptFeature.h"
  #elif defined(STM32PLUS_F1_MD_VL)
    #include "dma/features/f1/mdvl/DmaInterruptFeature.h"
  #endif

  #include "dma/features/f1/Dma1Channel1InterruptFeature.h"
  #include "dma/features/f1/Dma1Channel2InterruptFeature.h"
  #include "dma/features/f1/Dma1Channel3InterruptFeature.h"
  #include "dma/features/f1/Dma1Channel4InterruptFeature.h"
  #include "dma/features/f1/Dma1Channel5InterruptFeature.h"
  #include "dma/features/f1/Dma1Channel6InterruptFeature.h"
  #include "dma/features/f1/Dma1Channel7InterruptFeature.h"
  #include "dma/features/f1/Dma2Channel1InterruptFeature.h"
  #include "dma/features/f1/Dma2Channel2InterruptFeature.h"
  #include "dma/features/f1/Dma2Channel3InterruptFeature.h"
  #include "dma/features/f1/Dma2Channel4InterruptFeature.h"
  #include "dma/features/f1/Dma2Channel5InterruptFeature.h"

  #include "dma/features/f1/DmaMemoryCopyBaseFeature.h"
  #include "dma/features/f1/TimerDmaFeature.h"
  #include "dma/features/f1/UsartDmaReaderFeature.h"
  #include "dma/features/f1/UsartDmaWriterFeature.h"
  #include "dma/features/f1/SpiDmaReaderFeature.h"
  #include "dma/features/f1/SpiDmaWriterFeature.h"
  #include "dma/features/f1/I2CDmaReaderFeature.h"
  #include "dma/features/f1/I2CDmaWriterFeature.h"
  #include "dma/features/f1/I2SDmaReaderFeature.h"
  #include "dma/features/f1/I2SDmaWriterFeature.h"
  #include "dma/features/f1/SdioDmaReaderFeature.h"
  #include "dma/features/f1/SdioDmaWriterFeature.h"
  #include "dma/features/f1/DmaFsmcLcdMemoryCopyFeature.h"
  #include "dma/features/f1/AdcDmaFeature.h"
  #include "dma/features/f1/AdcMultiDmaFeature.h"

  #include "dma/features/f1/DmaPeripheralInfo.h"

#elif defined(STM32PLUS_F4)

  #define USE_DMA1_0_INTERRUPT
  #define USE_DMA1_1_INTERRUPT
  #define USE_DMA1_2_INTERRUPT
  #define USE_DMA1_3_INTERRUPT
  #define USE_DMA1_4_INTERRUPT
  #define USE_DMA1_5_INTERRUPT
  #define USE_DMA1_6_INTERRUPT
  #define USE_DMA1_7_INTERRUPT
  #define USE_DMA2_0_INTERRUPT
  #define USE_DMA2_1_INTERRUPT
  #define USE_DMA2_2_INTERRUPT
  #define USE_DMA2_3_INTERRUPT
  #define USE_DMA2_4_INTERRUPT
  #define USE_DMA2_5_INTERRUPT
  #define USE_DMA2_6_INTERRUPT
  #define USE_DMA2_7_INTERRUPT

  #include "dma/f4/Dma.h"
  #include "dma/features/DmaFeatureBase.h"

  #include "dma/f4/DmaIncludes.h"

  #include "dma/features/f4/DmaInterruptFeature.h"
  #include "dma/features/f4/Dma1Stream0InterruptFeature.h"
  #include "dma/features/f4/Dma1Stream1InterruptFeature.h"
  #include "dma/features/f4/Dma1Stream2InterruptFeature.h"
  #include "dma/features/f4/Dma1Stream3InterruptFeature.h"
  #include "dma/features/f4/Dma1Stream4InterruptFeature.h"
  #include "dma/features/f4/Dma1Stream5InterruptFeature.h"
  #include "dma/features/f4/Dma1Stream6InterruptFeature.h"
  #include "dma/features/f4/Dma1Stream7InterruptFeature.h"
  #include "dma/features/f4/Dma2Stream0InterruptFeature.h"
  #include "dma/features/f4/Dma2Stream1InterruptFeature.h"
  #include "dma/features/f4/Dma2Stream2InterruptFeature.h"
  #include "dma/features/f4/Dma2Stream3InterruptFeature.h"
  #include "dma/features/f4/Dma2Stream4InterruptFeature.h"
  #include "dma/features/f4/Dma2Stream5InterruptFeature.h"
  #include "dma/features/f4/Dma2Stream6InterruptFeature.h"
  #include "dma/features/f4/Dma2Stream7InterruptFeature.h"

  #include "dma/features/f4/DmaMemoryCopyBaseFeature.h"
  #include "dma/features/f4/TimerDmaFeature.h"
  #include "dma/features/f4/UsartDmaReaderFeature.h"
  #include "dma/features/f4/UsartDmaWriterFeature.h"
  #include "dma/features/f4/SpiDmaReaderFeature.h"
  #include "dma/features/f4/SpiDmaWriterFeature.h"
  #include "dma/features/f4/I2CDmaReaderFeature.h"
  #include "dma/features/f4/I2CDmaWriterFeature.h"
  #include "dma/features/f4/I2SDmaReaderFeature.h"
  #include "dma/features/f4/I2SDmaWriterFeature.h"
  #include "dma/features/f4/SdioDmaReaderFeature.h"
  #include "dma/features/f4/SdioDmaWriterFeature.h"
  #include "dma/features/f4/DmaFsmcLcdMemoryCopyFeature.h"
  #include "dma/features/f4/AdcDmaFeature.h"
  #include "dma/features/f4/AdcMultiDmaFeature.h"

#if defined(STM32PLUS_F4_HAS_DAC)
  #include "dma/features/f4/DacDmaWriterFeature.h"
#endif

  #include "dma/features/f4/DmaPeripheralInfo.h"

#elif defined(STM32PLUS_F0)

  #define USE_DMA1_1_INTERRUPT
  #define USE_DMA1_2_3_INTERRUPT
  #define USE_DMA1_4_5_INTERRUPT

  #include "dma/f0/Dma.h"
  #include "dma/features/DmaFeatureBase.h"

  #include "dma/f0/Dma1Channel1.h"
  #include "dma/f0/Dma1Channel2.h"
  #include "dma/f0/Dma1Channel3.h"
  #include "dma/f0/Dma1Channel4.h"
  #include "dma/f0/Dma1Channel5.h"

   #include "dma/features/f0/DmaInterruptFeature.h"

  #include "dma/features/f0/Dma1Channel1InterruptFeature.h"
  #include "dma/features/f0/Dma1Channel2InterruptFeature.h"
  #include "dma/features/f0/Dma1Channel3InterruptFeature.h"
  #include "dma/features/f0/Dma1Channel4InterruptFeature.h"
  #include "dma/features/f0/Dma1Channel5InterruptFeature.h"

  #include "dma/features/f0/DmaMemoryCopyBaseFeature.h"
  #include "dma/features/f0/TimerDmaFeature.h"
  #include "dma/features/f0/UsartDmaReaderFeature.h"
  #include "dma/features/f0/UsartDmaWriterFeature.h"
  #include "dma/features/f0/SpiDmaReaderFeature.h"
  #include "dma/features/f0/SpiDmaWriterFeature.h"
  #include "dma/features/f0/I2CDmaReaderFeature.h"
  #include "dma/features/f0/I2CDmaWriterFeature.h"
  #include "dma/features/f0/AdcDmaFeature.h"

#if !(defined(STM32PLUS_F0_30) || defined(STM32PLUS_F0_42))
  #include "dma/features/f0/DacDmaWriterFeature.h"
#endif

  #include "dma/features/f0/DmaPeripheralInfo.h"

#endif

// includes for the features

#include "dma/features/DmaMemoryCopyFeature.h"
#include "dma/features/DmaMemoryFillFeature.h"
#include "dma/features/PwmFadeTimerDmaFeature.h"
