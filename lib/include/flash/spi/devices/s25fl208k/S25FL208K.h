/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Spansion S25FL208K 8Mbit Serial Flash Memory with Dual SPI
     *
     */

    template<class TSpi>
    struct S25FL208K : SpiFlashDevice<S25FL208K<TSpi>>,

                      WriteStatusRegister <TSpi>,   // standard commands
                      WriteDisable        <TSpi>,
                      ReadStatusRegister  <TSpi>,
                      WriteEnable         <TSpi>,
                      PageProgram         <TSpi>,
                      Read                <TSpi>,
                      FastRead            <TSpi>,
                      SectorErase         <TSpi>,
                      BlockErase          <TSpi,s25fl208k::CMD_BLOCK_ERASE>,
                      ChipErase           <TSpi>,
                      DeepPowerDown       <TSpi>,
                      ReleaseDeepPowerDown<TSpi>,
                      ReadJedecId         <TSpi> {

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

      S25FL208K(TSpi& spi) {
        this->_spi=&spi;
      }


      /**
       * Get the size of the device in bytes (1 megabyte)
       * @return 1048576
       */

      constexpr uint32_t getSize() const {
        return 1024*1024*1;
      }
    };
  }
}
