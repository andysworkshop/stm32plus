/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {

  /**
   * Default Constructor, set the current position to zero.
   */

  File::File() {
    _offset=0;
  }

  /**
   * Get the current offset
   * @return The current file offset.
   */

  uint32_t File::getOffset() const {
    return _offset;
  }
}
