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

    IteratingFreeClusterFinder::IteratingFreeClusterFinder(FatFileSystem& fs_,uint32_t firstFatEntry_) :
      FreeClusterFinder(fs_), _iterator(fs_,firstFatEntry_,true) {

    }

    /**
     * Find the next free entry by using a FatIterator initialised with the freeCluster_ parameter
     * to find the next free cluster. A cluster that contains zero is free.
     *
     * @param freeCluster_ A reference to a caller supplied uint32_t that will be filled in with the next
     * free cluster that the iterator finds.
     * @return false if there is an error, check error provider for error details.
     *
     * @see FreeClusterFinder::find
     */

    bool IteratingFreeClusterFinder::find(uint32_t& freeCluster_) {

      while(_iterator.next()) {

        if(_iterator.currentContent() == 0) {
          // a zero entry is free

          freeCluster_=_iterator.current();
          return true;
        }
      }

      // if we reached the end then return the true error

      if(errorProvider.isLastError(ErrorProvider::ERROR_PROVIDER_ITERATOR,FatIterator::E_END_OF_ENTRIES))
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FREE_CLUSTER_FINDER,E_NO_FREE_CLUSTERS);

      // something else went wrong, propagate it

      return false;
    }

  }
}
