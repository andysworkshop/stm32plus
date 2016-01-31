/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
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

#if defined(STM32PLUS_F4_HAS_GPIOF_G_I)

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

    struct PG0 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PG1 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PG2 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PG3 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PG4 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PG5 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PG6 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PG7 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PG8 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PG9 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PG10 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PG11 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PG12 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PG13 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PG14 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PG15 {
      enum {
        Port=GPIOG_BASE,
        Pin=GPIO_Pin_15
      };
    };

#endif

    struct PH0 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PH1 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PH2 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PH3 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PH4 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PH5 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PH6 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PH7 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PH8 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PH9 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PH10 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PH11 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PH12 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PH13 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PH14 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PH15 {
      enum {
        Port=GPIOH_BASE,
        Pin=GPIO_Pin_15
      };
    };

#if defined(STM32PLUS_F4_HAS_GPIOF_G_I)

    struct PI0 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PI1 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PI2 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PI3 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PI4 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PI5 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PI6 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PI7 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PI8 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PI9 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PI10 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PI11 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PI12 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PI13 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PI14 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PI15 {
      enum {
        Port=GPIOI_BASE,
        Pin=GPIO_Pin_15
      };
    };

#endif
#if defined(STM32PLUS_F4_HAS_GPIOJ_K)

    struct PJ0 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PJ1 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PJ2 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PJ3 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PJ4 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PJ5 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PJ6 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PJ7 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PJ8 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PJ9 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PJ10 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PJ11 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PJ12 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PJ13 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PJ14 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PJ15 {
      enum {
        Port=GPIOJ_BASE,
        Pin=GPIO_Pin_15
      };
    };

    struct PK0 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_0
      };
    };

    struct PK1 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_1
      };
    };

    struct PK2 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_2
      };
    };

    struct PK3 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_3
      };
    };

    struct PK4 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_4
      };
    };

    struct PK5 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_5
      };
    };

    struct PK6 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_6
      };
    };

    struct PK7 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_7
      };
    };

    struct PK8 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_8
      };
    };

    struct PK9 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_9
      };
    };

    struct PK10 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_10
      };
    };

    struct PK11 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_11
      };
    };

    struct PK12 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_12
      };
    };

    struct PK13 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_13
      };
    };

    struct PK14 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_14
      };
    };

    struct PK15 {
      enum {
        Port=GPIOK_BASE,
        Pin=GPIO_Pin_15
      };
    };

#endif

  }
}
