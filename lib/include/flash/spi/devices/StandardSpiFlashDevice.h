/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * A standard SPI flash device implements only the commands that are known to
     * be common across (nearly) all SPI flash devices.
     */

    template<class TSpi>
    struct StandardSpiFlashDevice : SpiFlashDevice<StandardSpiFlashDevice<TSpi>>,

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
       */

      StandardSpiFlashDevice(TSpi& spi) {
        this->_spi=&spi;
      }


      /**
       * This is a generic implementation, the size of the device is unknown
       * @return 0
       */

      constexpr uint32_t getSize() const {
        return 0;
      }
    };
  }
}
