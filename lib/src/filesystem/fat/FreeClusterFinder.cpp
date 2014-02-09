/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {
  namespace fat {

    /*
     * Constructor
     */

    FreeClusterFinder::FreeClusterFinder(FatFileSystem& fs_) :
      _fs(fs_) {
    }
  }
}
