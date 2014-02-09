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
     *
     * @param[in] fs_ The filesystem reference.
     * @param[in] firstClusterNumber_ The number of the first cluster in the chain.
     * @param[in] extend_ Whether or not to extend when the end is reached.
     */

    ClusterChainIterator::ClusterChainIterator(FatFileSystem& fs_,uint32_t firstClusterNumber_,ExtensionMode extend_) :
      _fs(fs_) {
      _currentClusterNumber=firstClusterNumber_;
      _first=true;
      _firstClusterNumber=firstClusterNumber_;
      _extend=extend_;
    }

    /**
     * Reset the iterator to the beginning.
     * @param[in] firstClusterNumber_ The first cluster in the chain.
     */

    void ClusterChainIterator::reset(uint32_t firstClusterNumber_) {
      _currentClusterNumber=firstClusterNumber_;
      _first=true;
    }

    /*
     * Get the current cluster number
     */

    uint32_t ClusterChainIterator::current() {
      return _currentClusterNumber;
    }

    /**
     * Get the current sector number.
     * @return The current sector number.
     */

    uint32_t ClusterChainIterator::currentSectorNumber() {
      return _fs.clusterToSector(_currentClusterNumber);
    }

    /*
     * Move to the next in the chain
     */

    bool ClusterChainIterator::next() {

      uint32_t nextNumber;

      // clear error

      errorProvider.clear();

      // first call to next() must return the first cluster

      if(_first) {
        nextNumber=_currentClusterNumber;
        _first=false;
      } else {

        // read from the FAT

        if(!_fs.readFatEntry(_currentClusterNumber,nextNumber))
          return false;
      }

      // reached the end?

      if(nextNumber == 0 || _fs.isEndOfClusterChainMarker(nextNumber)) {
        if(_extend == extensionExtend) {

          // try to extend the cluster chain with a new entry

          if(!_fs.allocateNewCluster(_currentClusterNumber,nextNumber))
            return false;
        } else
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_ITERATOR,E_END_OF_ENTRIES);
      }

      // check for bad cluster

      if(nextNumber == _fs.getBadClusterMarker())
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FAT_FILESYSTEM,FatFileSystem::E_BAD_CLUSTER);

      // OK

      _currentClusterNumber=nextNumber;
      return true;
    }
  }
}
