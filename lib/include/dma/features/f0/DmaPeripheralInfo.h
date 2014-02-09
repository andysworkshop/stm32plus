/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * DMA peripheral info types serve to hold basic information about a peripheral that could be used
   * as a source or target for DMA operations. e.g. the register address and memory transfer size
   */

  struct TimerDmaPeripheralInfo {
      enum {
        MEMORY_WORD_SIZE = DMA_MemoryDataSize_HalfWord,
        PERIPHERAL_WORD_SIZE = DMA_PeripheralDataSize_HalfWord
    };
  };

  struct Timer1CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM1_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer1ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM1_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer1RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM1_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer1Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM1_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer1Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM1_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer1Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM1_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer1Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM1_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer2CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM2_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer2ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM2_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer2RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM2_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer2Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM2_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer2Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM2_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer2Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM2_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer2Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM2_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer3CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM3_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer3ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM3_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer3RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM3_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer3Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM3_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer3Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM3_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer3Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM3_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer3Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM3_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer6CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer6ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer6RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer6Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer6Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer6Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer6Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer15CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM15_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer15ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM15_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer15RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM15_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer15Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM15_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer15Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM15_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer15Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM15_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer15Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM15_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer16CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM16_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer16ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM16_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer16RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM16_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer16Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM16_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer16Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM16_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer16Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM16_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer16Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM16_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer17CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM17_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer17ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM17_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer17RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM17_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer17Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM17_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer17Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM17_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer17Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM17_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer17Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM17_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct UsartDmaPeripheralInfo {
      enum {
        MEMORY_WORD_SIZE = DMA_MemoryDataSize_Byte,
        PERIPHERAL_WORD_SIZE = DMA_PeripheralDataSize_Byte
    };
  };

  struct Usart1TxDmaPeripheralInfo : UsartDmaPeripheralInfo { enum { REGISTER_ADDRESS = USART1_BASE + __builtin_offsetof(USART_TypeDef,TDR) }; };
  struct Usart2TxDmaPeripheralInfo : UsartDmaPeripheralInfo { enum { REGISTER_ADDRESS = USART2_BASE + __builtin_offsetof(USART_TypeDef,TDR) }; };
  struct Usart1RxDmaPeripheralInfo : UsartDmaPeripheralInfo { enum { REGISTER_ADDRESS = USART1_BASE + __builtin_offsetof(USART_TypeDef,RDR) }; };
  struct Usart2RxDmaPeripheralInfo : UsartDmaPeripheralInfo { enum { REGISTER_ADDRESS = USART2_BASE + __builtin_offsetof(USART_TypeDef,RDR) }; };

  struct SpiDmaPeripheralInfo {
      enum {
        MEMORY_WORD_SIZE = DMA_MemoryDataSize_Byte,
        PERIPHERAL_WORD_SIZE = DMA_PeripheralDataSize_Byte
    };
  };

  struct Spi1TxDmaPeripheralInfo : SpiDmaPeripheralInfo { enum { REGISTER_ADDRESS = SPI1_BASE + __builtin_offsetof(SPI_TypeDef,DR) }; };
  struct Spi2TxDmaPeripheralInfo : SpiDmaPeripheralInfo { enum { REGISTER_ADDRESS = SPI2_BASE + __builtin_offsetof(SPI_TypeDef,DR) }; };
  struct Spi1RxDmaPeripheralInfo : SpiDmaPeripheralInfo { enum { REGISTER_ADDRESS = SPI1_BASE + __builtin_offsetof(SPI_TypeDef,DR) }; };
  struct Spi2RxDmaPeripheralInfo : SpiDmaPeripheralInfo { enum { REGISTER_ADDRESS = SPI2_BASE + __builtin_offsetof(SPI_TypeDef,DR) }; };
}
