/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Generic definition of the clock control class. There is a specialisation
   * for each supported peripheral
   */

  template<PeripheralName TPeripheral>
  struct ClockControl;

  /*
   * DAC1
   */

  template<>
  struct ClockControl<PERIPHERAL_DAC1> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,DISABLE);
    }
  };


  /*
   * DAC2
   */

  template<>
  struct ClockControl<PERIPHERAL_DAC2> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,DISABLE);
    }
  };


  /*
   * I2C1
   */

  template<>
  struct ClockControl<PERIPHERAL_I2C1> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,DISABLE);
    }
  };

  /*
   * I2C2
   */

  template<>
  struct ClockControl<PERIPHERAL_I2C2> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,DISABLE);
    }
  };

  /**
   * SPI2
   */

  template<>
  struct ClockControl<PERIPHERAL_SPI2> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,DISABLE);
    }
  };

  /**
   * SPI3
   */

  template<>
  struct ClockControl<PERIPHERAL_SPI3> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,DISABLE);
    }
  };

  /*
   * TIM1
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER1> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,DISABLE);
    }
  };

  /*
   * TIM2
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER2> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);
    }
  };

  /*
   * TIM3
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER3> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,DISABLE);
    }
  };

  /*
   * TIM4
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER4> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,DISABLE);
    }
  };

  /*
   * TIM5
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER5> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,DISABLE);
    }
  };

  /*
   * TIM6
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER6> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,DISABLE);
    }
  };

  /*
   * TIM7
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER7> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,DISABLE);
    }
  };

  /*
   * TIM8
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER8> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,DISABLE);
    }
  };

  /*
   * TIM9
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER9> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,DISABLE);
    }
  };

  /*
   * TIM10
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER10> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,DISABLE);
    }
  };

  /*
   * TIM11
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER11> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,DISABLE);
    }
  };

  /*
   * TIM12
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER12> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,DISABLE);
    }
  };

  /*
   * TIM13
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER13> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,DISABLE);
    }
  };

  /*
   * TIM14
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER14> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,DISABLE);
    }
  };
}

