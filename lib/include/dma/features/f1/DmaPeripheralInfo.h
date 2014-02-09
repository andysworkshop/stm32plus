/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
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

  struct Timer4CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM4_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer4ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM4_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer4RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM4_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer4Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM4_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer4Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM4_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer4Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM4_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer4Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM4_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer5CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM5_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer5ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM5_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer5RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM5_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer5Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM5_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer5Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM5_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer5Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM5_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer5Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM5_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer6CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer6ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer6RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer6Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer6Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer6Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer6Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM6_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer7CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM7_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer7ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM7_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer7RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM7_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer7Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM7_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer7Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM7_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer7Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM7_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer7Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM7_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct Timer8CntDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM8_BASE + __builtin_offsetof(TIM_TypeDef,CNT) }; };
  struct Timer8ArrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM8_BASE + __builtin_offsetof(TIM_TypeDef,ARR) }; };
  struct Timer8RcrDmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM8_BASE + __builtin_offsetof(TIM_TypeDef,RCR) }; };
  struct Timer8Ccr1DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM8_BASE + __builtin_offsetof(TIM_TypeDef,CCR1) }; };
  struct Timer8Ccr2DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM8_BASE + __builtin_offsetof(TIM_TypeDef,CCR2) }; };
  struct Timer8Ccr3DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM8_BASE + __builtin_offsetof(TIM_TypeDef,CCR3) }; };
  struct Timer8Ccr4DmaPeripheralInfo : TimerDmaPeripheralInfo { enum { REGISTER_ADDRESS = TIM8_BASE + __builtin_offsetof(TIM_TypeDef,CCR4) }; };

  struct UsartDmaPeripheralInfo {
      enum {
        MEMORY_WORD_SIZE = DMA_MemoryDataSize_Byte,
        PERIPHERAL_WORD_SIZE = DMA_PeripheralDataSize_Byte
    };
  };

  struct Usart1TxDmaPeripheralInfo : UsartDmaPeripheralInfo { enum { REGISTER_ADDRESS = USART1_BASE + __builtin_offsetof(USART_TypeDef,DR) }; };
  struct Usart2TxDmaPeripheralInfo : UsartDmaPeripheralInfo { enum { REGISTER_ADDRESS = USART2_BASE + __builtin_offsetof(USART_TypeDef,DR) }; };
  struct Usart3TxDmaPeripheralInfo : UsartDmaPeripheralInfo { enum { REGISTER_ADDRESS = USART3_BASE + __builtin_offsetof(USART_TypeDef,DR) }; };
  struct Uart4TxDmaPeripheralInfo : UsartDmaPeripheralInfo { enum { REGISTER_ADDRESS = UART4_BASE + __builtin_offsetof(USART_TypeDef,DR) }; };
  struct Uart5TxDmaPeripheralInfo : UsartDmaPeripheralInfo { enum { REGISTER_ADDRESS = UART5_BASE + __builtin_offsetof(USART_TypeDef,DR) }; };

  struct SpiDmaPeripheralInfo {
      enum {
        MEMORY_WORD_SIZE = DMA_MemoryDataSize_Byte,
        PERIPHERAL_WORD_SIZE = DMA_PeripheralDataSize_Byte
    };
  };

  struct Spi1TxDmaPeripheralInfo : SpiDmaPeripheralInfo { enum { REGISTER_ADDRESS = SPI1_BASE + __builtin_offsetof(SPI_TypeDef,DR) }; };
  struct Spi2TxDmaPeripheralInfo : SpiDmaPeripheralInfo { enum { REGISTER_ADDRESS = SPI2_BASE + __builtin_offsetof(SPI_TypeDef,DR) }; };
  struct Spi3TxDmaPeripheralInfo : SpiDmaPeripheralInfo { enum { REGISTER_ADDRESS = SPI3_BASE + __builtin_offsetof(SPI_TypeDef,DR) }; };
}
