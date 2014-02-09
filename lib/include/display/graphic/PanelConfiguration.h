/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /*
     * Possible display orientations
     */

    enum Orientation {
      PORTRAIT,               // long side up
      LANDSCAPE               // short side up
    };

    /*
     * Possible colour depths. Not every colour depth is guaranteed to be
     * supported by the driver embedded in the panel.
     */

    enum ColourDepth {
      COLOURS_16BIT,          // 64K colours
      COLOURS_18BIT,          // 262K colours
      COLOURS_24BIT           // 16M colours
    };

    /*
     * Possible memory locations for something
     */

    enum MemoryLocation {
      FLASH_UNCOMPRESSED,     // it's in flash (program) memory
      FLASH_COMPRESSED,       // it's in flash, and it's compressed
      SRAM                    // it's in the SRAM address space
    };

    /*
     * Possible directions
     */

    enum Direction {
      HORIZONTAL,
      VERTICAL
    };
  }
}
