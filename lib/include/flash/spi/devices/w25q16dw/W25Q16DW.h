/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Winbond W25Q16DW 1.8V 16Mbit Serial Flash Memory with Dual/Quad SPI & QPI
     *
     */

    template<class TSpi>
    struct W25Q16DW : SpiFlashDevice<W25Q16DW<TSpi>>,

                      WriteStatusRegister <TSpi>,   // standard commands
                      WriteDisable        <TSpi>,
                      ReadStatusRegister  <TSpi>,
                      WriteEnable         <TSpi>,
                      PageProgram         <TSpi>,
                      Read                <TSpi>,
                      FastRead            <TSpi>,
                      SectorErase         <TSpi>,
                      BlockErase          <TSpi>,
                      ChipErase           <TSpi>,
                      DeepPowerDown       <TSpi>,
                      ReleaseDeepPowerDown<TSpi>,
                      w25q16dw::EraseResume<TSpi>,        // custom commands
                      w25q16dw::EraseSuspend<TSpi> {

      /**
       * Various constants required by the base class
       */

      enum {
        STATUS_BUSY_BIT_MASK = 0x1,   // bitmask to get the BUSY bit from SR
        PAGE_SIZE = 256               // 256 byte pages
      };


      /**
       * Constructor
       * @param spi Reference to the SPI object
       */

      W25Q16DW(TSpi& spi) {
        this->_spi=&spi;
      }


      /**
       * Get the size of the device in bytes (2 megabytes)
       * @return 2097152
       */

      constexpr uint32_t getSize() const {
        return 1024*1024*2;
      }
    };
  }
}
