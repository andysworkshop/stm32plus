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


  /**
   * Template access to a set of functions that map the GPIO address to the clock
   */

  namespace gpioHelper {

    template<uint32_t> uint32_t getPeripheral();          ///< generic template - never used
    template<> inline uint32_t getPeripheral<GPIOA_BASE>() { return RCC_AHB1Periph_GPIOA; }
    template<> inline uint32_t getPeripheral<GPIOB_BASE>() { return RCC_AHB1Periph_GPIOB; }
    template<> inline uint32_t getPeripheral<GPIOC_BASE>() { return RCC_AHB1Periph_GPIOC; }
    template<> inline uint32_t getPeripheral<GPIOD_BASE>() { return RCC_AHB1Periph_GPIOD; }
    template<> inline uint32_t getPeripheral<GPIOE_BASE>() { return RCC_AHB1Periph_GPIOE; }

    template<> inline uint32_t getPeripheral<GPIOH_BASE>() { return RCC_AHB1Periph_GPIOH; }

#if defined(STM32PLUS_F4_HAS_GPIOF_G_I)
    template<> inline uint32_t getPeripheral<GPIOF_BASE>() { return RCC_AHB1Periph_GPIOF; }
    template<> inline uint32_t getPeripheral<GPIOG_BASE>() { return RCC_AHB1Periph_GPIOG; }
    template<> inline uint32_t getPeripheral<GPIOI_BASE>() { return RCC_AHB1Periph_GPIOI; }
#endif

#if defined(STM32PLUS_F4_HAS_GPIOJ_K)
    template<> inline uint32_t getPeripheral<GPIOJ_BASE>() { return RCC_AHB1Periph_GPIOJ; }
    template<> inline uint32_t getPeripheral<GPIOK_BASE>() { return RCC_AHB1Periph_GPIOK; }
#endif
  }


  /**
   * Template for a GPIO implementation that manages a set of pins
   */

  template<uint32_t TPortAddress,class... Features>
  class GpioPort : public GpioPortBase,
                   public Features... {

    protected:

      /**
       * Init clock and return port address
       * @return the peripheral port address
       */

      GPIO_TypeDef *initClock() {
        RCC_AHB1PeriphClockCmd(gpioHelper::getPeripheral<TPortAddress>(),ENABLE);
        return (GPIO_TypeDef *)TPortAddress;
      }


    public:

      /**
       * Constructor
       * @param addr The peripheral address (GPIOA, GPIOB, ...)
       */

      GpioPort() :
        GpioPortBase(initClock()),
        Features((GpioPortBase&)*this)... {
      }


      /**
       * Array operator. Return a reference to the selected pin. It is not generally safe to store the
       * reference returned by this function between calls to this index operator because subsequent
       * calls can change the pin used by that reference.
       * @param selectedPin The pin to select
       * @return A reference to the pin that you can call operations such as set(), reset() on.
       */

      Gpio& operator[](uint8_t selectedPin) {
        _pinHandlers[selectedPin]->setSelectedPin(selectedPin);
        return *(_pinHandlers[selectedPin]);
      }


      /**
       * Set some pins directly
       * @param pinIds The pin bitmap in peripheral library format
       */

      void set(uint16_t pinIds) const {
        GPIO_SetBits((GPIO_TypeDef *)TPortAddress,pinIds);
      }


      /**
       * Reset some pins directly
       * @param pinIds The pin bitmap in peripheral library format
       */

      void reset(uint16_t pinIds) const {
        GPIO_ResetBits((GPIO_TypeDef *)TPortAddress,pinIds);
      }


      /**
       * Write a value direct to the port
       * @param value The bitmask to write
       */

      void write(uint16_t value) const {
        GPIO_Write((GPIO_TypeDef *)TPortAddress,value);
      }


      /**
       * Get an STL compatible forward iterator pointing to the first pin
       * @return an iterator
       */

      GpioIterator begin() {
        return GpioIterator(_pinHandlers,_peripheralAddress,_low);
      }


      /**
       * Get an STL compatible forward iterator pointing just beyond the last pin
       * @return a forward iterator
       */

      GpioIterator end() {
        return GpioIterator(_pinHandlers,_peripheralAddress,16);
      }
  };
}
