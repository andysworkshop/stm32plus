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
  namespace gpio {

    struct PA0 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PA1 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PA2 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PA3 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PA4 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PA5 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PA6 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PA7 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PA8 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PA9 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PA10 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PA11 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PA12 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PA13 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PA14 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PA15 {
      enum {
        Port=GPIOA_BASE,
        Pin=GPIO_Pin_15
      };
    };

    struct PB0 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PB1 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PB2 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PB3 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PB4 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PB5 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PB6 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PB7 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PB8 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PB9 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PB10 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PB11 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PB12 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PB13 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PB14 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PB15 {
      enum {
        Port=GPIOB_BASE,
        Pin=GPIO_Pin_15
      };
    };

    struct PC0 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PC1 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PC2 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PC3 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PC4 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PC5 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PC6 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PC7 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PC8 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PC9 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PC10 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PC11 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PC12 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PC13 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PC14 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PC15 {
      enum {
        Port=GPIOC_BASE,
        Pin=GPIO_Pin_15
      };
    };

    struct PD0 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PD1 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PD2 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PD3 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PD4 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PD5 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PD6 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PD7 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PD8 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PD9 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PD10 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PD11 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PD12 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PD13 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PD14 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PD15 {
      enum {
        Port=GPIOD_BASE,
        Pin=GPIO_Pin_15
      };
    };

    struct PE0 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PE1 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PE2 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PE3 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PE4 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PE5 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PE6 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PE7 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PE8 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PE9 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PE10 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PE11 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PE12 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PE13 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PE14 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PE15 {
      enum {
        Port=GPIOE_BASE,
        Pin=GPIO_Pin_15
      };
    };

    struct PF0 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PF1 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PF2 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PF3 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PF4 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PF5 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PF6 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PF7 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PF8 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PF9 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PF10 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PF11 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PF12 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PF13 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PF14 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PF15 {
      enum {
        Port=GPIOF_BASE,
        Pin=GPIO_Pin_15
      };
    };
  }
}
