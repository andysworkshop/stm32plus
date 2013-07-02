/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * FSMC_Bank1_NORSRAM1 implementation
   */

  template<>
  class FsmcPeripheral<FSMC_Bank1_NORSRAM1> {

    public:
      static void initialise() {

        ClockControl<PERIPHERAL_FSMC>::On();

        // initialise NE

        GpioPinInitialiser::initialise(
          GPIOD,
          GPIO_Pin_7,         // NE
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GPIO_AF_FSMC);
      }

      /**
       * Static access to the base address
       */

      template<typename T>
      static T *getBaseAddress() {
        return reinterpret_cast<T *>(0x60000000);
      }


      /**
       * Static access to the bank number
       */

      static uint32_t getBank() {
        return FSMC_Bank1_NORSRAM1;
      }
  };


  /**
   * FSMC_Bank1_NORSRAM2 implementation
   */

  template<>
  class FsmcPeripheral<FSMC_Bank1_NORSRAM2> {

    public:
      static void initialise() {

        ClockControl<PERIPHERAL_FSMC>::On();

        // initialise NE

        GpioPinInitialiser::initialise(
          GPIOG,
          GPIO_Pin_9,         // NE
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GPIO_AF_FSMC);
      }

      /**
       * Static access to the base address
       */

      template<typename T>
      static T *getBaseAddress() {
        return reinterpret_cast<T *>(0x64000000);
      }


      /**
       * Static access to the bank number
       */

      static uint32_t getBank() {
        return FSMC_Bank1_NORSRAM2;
      }
  };


  /**
   * FSMC_Bank1_NORSRAM3 implementation
   */

  template<>
  class FsmcPeripheral<FSMC_Bank1_NORSRAM3> {

    public:
      static void initialise() {

        ClockControl<PERIPHERAL_FSMC>::On();

        // initialise NE

        GpioPinInitialiser::initialise(
          GPIOG,
          GPIO_Pin_10,          // NE
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GPIO_AF_FSMC);
      }

      /**
       * Static access to the base address
       */

      template<typename T>
      static T* getBaseAddress() {
        return reinterpret_cast<T *>(0x68000000);
      }


      /**
       * Static access to the bank number
       */

      static uint32_t getBank() {
        return FSMC_Bank1_NORSRAM3;
      }
  };


  /**
   * FSMC_Bank1_NORSRAM4 implementation
   */

  template<>
  class FsmcPeripheral<FSMC_Bank1_NORSRAM4> {

    public:
      static void initialise() {

        ClockControl<PERIPHERAL_FSMC>::On();

        // initialise NE

        GpioPinInitialiser::initialise(
          GPIOG,
          GPIO_Pin_12,          // NE
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GPIO_AF_FSMC);
      }

      /**
       * Static access to the base address
       */

      template<typename T>
      static T *getBaseAddress() {
        return reinterpret_cast<T *>(0x6C000000);
      }


      /**
       * Static access to the bank number
       */

      static uint32_t getBank() {
        return FSMC_Bank1_NORSRAM4;
      }
  };


  /**
   * FSMC_Bank2_NAND implementation
   */

  template<>
  class FsmcPeripheral<FSMC_Bank2_NAND> {

    public:
      static void initialise() {

        ClockControl<PERIPHERAL_FSMC>::On();

        // initialise NE

        GpioPinInitialiser::initialise(
          GPIOD,
          GPIO_Pin_7,         // NE
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GPIO_AF_FSMC);
      }

      /**
       * Static access to the base address
       */

      template<typename T>
      static T *getBaseAddress() {
        return reinterpret_cast<T *>(0x70000000);
      }


      /**
       * Static access to the bank number
       */

      static uint32_t getBank() {
        return FSMC_Bank2_NAND;
      }
  };


  /**
   * FSMC_Bank3_NAND implementation
   */

  template<>
  class FsmcPeripheral<FSMC_Bank3_NAND> {

    public:
      static void initialise() {

        ClockControl<PERIPHERAL_FSMC>::On();

        // initialise NE

        GpioPinInitialiser::initialise(
          GPIOG,
          GPIO_Pin_9,         // NE
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GPIO_AF_FSMC);
      }

      /**
       * Static access to the base address
       */

      template<typename T>
      static T *getBaseAddress() {
        return reinterpret_cast<T *>(0x80000000);
      }


      /**
       * Static access to the bank number
       */

      static uint32_t getBank() {
        return FSMC_Bank3_NAND;
      }
  };


  /**
   * FSMC_Bank4_PCCARD implementation
   */

  template<>
  class FsmcPeripheral<FSMC_Bank4_PCCARD> {

    public:
      static void initialise() {

        ClockControl<PERIPHERAL_FSMC>::On();

        // initialise NE

        GpioPinInitialiser::initialise(
          GPIOG,
          GPIO_Pin_10,          // NE
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)FsmcPeripheralTraits::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GPIO_AF_FSMC);
      }

      /**
       * Static access to the base address
       */

      template<typename T>
      static T *getBaseAddress() {
        return reinterpret_cast<T *>(0x90000000);
      }


      /**
       * Static access to the bank number
       */

      static uint32_t getBank() {
        return FSMC_Bank4_PCCARD;
      }
  };


  /**
   * Typedefs for easy use
   */

  typedef FsmcPeripheral<FSMC_Bank1_NORSRAM1> FsmcBank1NorSram1;
  typedef FsmcPeripheral<FSMC_Bank1_NORSRAM2> FsmcBank1NorSram2;
  typedef FsmcPeripheral<FSMC_Bank1_NORSRAM3> FsmcBank1NorSram3;
  typedef FsmcPeripheral<FSMC_Bank1_NORSRAM4> FsmcBank1NorSram4;
  typedef FsmcPeripheral<FSMC_Bank2_NAND> FsmcBank2Nand;
  typedef FsmcPeripheral<FSMC_Bank3_NAND> FsmcBank3Nand;
  typedef FsmcPeripheral<FSMC_Bank4_PCCARD> FsmcBank4PcCard;
}
