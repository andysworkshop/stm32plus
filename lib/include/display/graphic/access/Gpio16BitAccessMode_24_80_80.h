/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Specialisation for 24Mhz HCLK, 64K colour mode, 80ns low and 80ns high write cycle periods.
     * The 41.6ns HCLK means that we actually achieve 83.2ns (+/- clock accuracy).
     *
     * This driver is intended for use with the Medium Density Value Line 24MHz products,
     * for example the STM32 VL Discovery board.
     *
     * This is for 18, 24 bit colour depths. The 64K specialisation should be preferred for 16-bit
     * colour depths as it contains more optimisations for increased speed. The compiler should
     * automatically pick the correct specialisation for you.
     */

    template<class TPinPackage,ColourDepth TColourDepth>
    class Gpio16BitAccessMode<TPinPackage,TColourDepth,24,80,80> {

      protected:
        uint32_t _controlSetAddress;
        uint32_t _controlResetAddress;
        uint32_t _portOutputRegister;
        uint16_t _wr;
        uint16_t _rs;
        uint32_t _jump;

      public:
        Gpio16BitAccessMode();

        void reset() const;

        void writeCommand(uint16_t command) const;
        void writeCommand(uint16_t command,uint16_t parameter) const;
        void writeData(uint16_t value) const;
        void writeDataAgain(uint16_t value) const;
        void writeMultiData(uint32_t howMuch,uint16_t value) const;

        void rawTransfer(const void *buffer,uint32_t numWords) const;
    };


    /**
     * Constructor
     */

    template<class TPinPackage,ColourDepth TColourDepth>
    inline Gpio16BitAccessMode<TPinPackage,TColourDepth,24,80,80>::Gpio16BitAccessMode() {

      // the assembly code needs these

      _rs=TPinPackage::Pin_RS;
      _wr=TPinPackage::Pin_WR;

      // these are the addresses of the reset/set registers in the normal peripheral region.

#if defined(STM32PLUS_F1) || defined(STM32PLUS_F0)
      _controlResetAddress=TPinPackage::Port_CONTROL+offsetof(GPIO_TypeDef,BRR);
      _controlSetAddress=TPinPackage::Port_CONTROL+offsetof(GPIO_TypeDef,BSRR);
#elif defined(STM32PLUS_F4)
      _controlResetAddress=TPinPackage::Port_CONTROL+offsetof(GPIO_TypeDef,BSRRH);
      _controlSetAddress=TPinPackage::Port_CONTROL+offsetof(GPIO_TypeDef,BSRRL);
#else
#error Unsupported MCU
#endif

      // this is the address of the data output ODR register in the normal peripheral region.

      _portOutputRegister=TPinPackage::Port_DATA+offsetof(GPIO_TypeDef,ODR);

      // all 16 port pins to output, 50MHz slew rate

      GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_DATA,
                                     0xffff,
                                     Gpio::OUTPUT);

      // control pins to output

      GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_CONTROL,
                                     TPinPackage::Pin_RS | TPinPackage::Pin_WR | TPinPackage::Pin_RESET,
                                     Gpio::OUTPUT);

      // WR must start as HIGH

      GPIO_SetBits((GPIO_TypeDef *)TPinPackage::Port_CONTROL,TPinPackage::Pin_WR);
    }


    /**
     * Hard-reset the panel
     */

    template<class TPinPackage,ColourDepth TColourDepth>
    inline void Gpio16BitAccessMode<TPinPackage,TColourDepth,24,80,80>::reset() const {

      GPIO_TypeDef *port;

      // let the power stabilise

      MillisecondTimer::delay(10);

      // reset sequence

      port=(GPIO_TypeDef *)TPinPackage::Port_CONTROL;

      GPIO_SetBits(port,TPinPackage::Pin_RESET);
      MillisecondTimer::delay(5);
      GPIO_ResetBits(port,TPinPackage::Pin_RESET);
      MillisecondTimer::delay(50);
      GPIO_SetBits(port,TPinPackage::Pin_RESET);
      MillisecondTimer::delay(50);
    }


    /**
     * Write a command
     * @param command The command register
     */

    template<class TPinPackage,ColourDepth TColourDepth>
    inline void Gpio16BitAccessMode<TPinPackage,TColourDepth,24,80,80>::writeCommand(uint16_t command) const {

      __asm volatile(
        " str  %[value], [%[data]]          \n\t"     // port <= value
        " str  %[rs],    [%[creset], #0]    \n\t"     // [rs] = 0
        " str  %[wr],    [%[creset], #0]    \n\t"     // [wr] = 0
        " str  %[wr],    [%[cset], #0]      \n\t"     // [wr] = 1
        :: [creset]   "r" (_controlResetAddress),     // the control reset address
           [cset]     "r" (_controlSetAddress),       // the control set address
           [data]     "r" (_portOutputRegister),      // the data port
           [wr]       "r" (_wr),                      // WR pin bit
           [rs]       "r" (_rs),                      // RS pin bit
           [value]    "r" (command)                   // input value
      );
    }


    /**
     * Write a command and its parameter (convenience function)
     * @param command The command register
     * @param parameter The register parameter
     */

    template<class TPinPackage,ColourDepth TColourDepth>
    inline void Gpio16BitAccessMode<TPinPackage,TColourDepth,24,80,80>::writeCommand(uint16_t command,uint16_t parameter) const {
      writeCommand(command);
      writeData(parameter);
    }


    /**
     * Write a data value
     * @param value The data value to write
     */

    template<class TPinPackage,ColourDepth TColourDepth>
    inline void Gpio16BitAccessMode<TPinPackage,TColourDepth,24,80,80>::writeData(uint16_t value) const {

      __asm volatile(
        " str  %[value], [%[data]]          \n\t"     // port <= value
        " str  %[rs],    [%[cset], #0]      \n\t"     // [rs] = 1
        " str  %[wr],    [%[creset], #0]    \n\t"     // [wr] = 0
        " str  %[wr],    [%[cset], #0]      \n\t"     // [wr] = 1
        :: [creset]   "r" (_controlResetAddress),     // the control reset address
           [cset]     "r" (_controlSetAddress),       // the control set address
           [data]     "r" (_portOutputRegister),      // the data port
           [wr]       "r" (TPinPackage::Pin_WR),      // WR pin bit
           [rs]       "r" (TPinPackage::Pin_RS),      // RS pin bit
           [value]    "r" (value)                     // input value
      );
    }


    /**
     * Write the same data value that we have recently written out. We cannot optimise by toggling
     * WR because the bus has multiple writes per pixel.
     */

    template<class TPinPackage,ColourDepth TColourDepth>
    inline void Gpio16BitAccessMode<TPinPackage,TColourDepth,24,80,80>::writeDataAgain(uint16_t value) const {
      writeData(value);
    }


  /**
   * Write a batch of the same data values to the XMEM interface using GPIO. We cannot optimise here
   * because the bus has multiple writes per pixel.
   * @param howMuch The number of 16-bit values to write
   * @param value The pixel-part to write out
   */

  template<class TPinPackage,ColourDepth TColourDepth>
  inline void Gpio16BitAccessMode<TPinPackage,TColourDepth,24,80,80>::writeMultiData(uint32_t howMuch,uint16_t value) const {
    while(howMuch--)
      writeData(value);
  }


    /**
     * Write out a raw block of data from memory
     * @param buffer Where to read from
     * @param numWords The number of 16-bit words
     */

    template<class TPinPackage,ColourDepth TColourDepth>
    inline void Gpio16BitAccessMode<TPinPackage,TColourDepth,24,80,80>::rawTransfer(const void *buffer,uint32_t numWords) const {

      const uint16_t *ptr=static_cast<const uint16_t *>(buffer);

      while(numWords--)
        writeData(*ptr++);
    }
  }
}
