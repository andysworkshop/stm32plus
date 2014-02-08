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
     * Constructor
     * @param[in] fs A reference to the FAT file system. Must not go out of scope.
     */

    LinearFreeClusterFinder::LinearFreeClusterFinder(FatFileSystem& fs) :
      IteratingFreeClusterFinder(fs,0) {
    }


    /**
     * Find a sequence of at least the given number of clusters that are all free
     */

    bool LinearFreeClusterFinder::findMultipleSequential(uint32_t clustersRequired,uint32_t& firstCluster) {

      uint32_t i;

      // start searching the FAT

      while(_iterator.next()) {

        // found a free entry

        if(_iterator.currentContent() == 0) {

          firstCluster=_iterator.current();

          // search for continuous free entries after this one

          for(i=1;i<clustersRequired;i++) {

            if(!_iterator.next()) {

              // did we hit the end?

              if(errorProvider.isLastError(ErrorProvider::ERROR_PROVIDER_ITERATOR,FatIterator::E_END_OF_ENTRIES))
                return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FREE_CLUSTER_FINDER,E_NO_FREE_CLUSTERS);

              // some other error

              return false;
            }

            // if this is not free, leave now

            if(_iterator.currentContent()!=0)
              break;
          }

          if(i==clustersRequired)
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
