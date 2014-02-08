/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {
  namespace fat {

    /**
     * Constructor: generate a random starting index between zero and the number of FAT entries.
     *
     * @param[in] fs_ A reference to the fat file system class. Must stay in scope.
     */

    WearResistFreeClusterFinder::WearResistFreeClusterFinder(FatFileSystem& fs_) :
      IteratingFreeClusterFinder(fs_,rand()%(fs_.getSectorsPerFat()*fs_.getSectorSizeInBytes())/fs_.getFatEntrySizeInBytes()) {
    }
  }
}
