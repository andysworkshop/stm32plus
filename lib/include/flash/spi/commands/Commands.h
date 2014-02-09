/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * "Standard" command list
     */

    enum {
      CMD_WRITE_REGISTERS         = 0x01,         ///< write status or write status & configuration
      CMD_PAGE_PROGRAM            = 0x02,         ///< program a page
      CMD_READ                    = 0x03,         ///< read, 1-bit mode
      CMD_WRITE_DISABLE           = 0x04,         ///< disable writing
      CMD_READ_STATUS_REGISTER    = 0x05,         ///< read the status register
      CMD_WRITE_ENABLE            = 0x06,         ///< enable writing
      CMD_FAST_READ               = 0x0b,         ///< fast frequency read out of data
      CMD_SECTOR_ERASE            = 0x20,         ///< erase a sector
      CMD_BLOCK_ERASE             = 0x52,         ///< erase a block of sectors
      CMD_CHIP_ERASE              = 0xc7,         ///< erase the whole thing
      CMD_READ_JEDEC_ID           = 0x9f,         ///< read the manufacturer and device information
      CMD_DEEP_POWER_DOWN         = 0xb9,         ///< power down mode
      CMD_RELEASE_DEEP_POWER_DOWN = 0xab          ///< release from deep power down
    };
  }
}
