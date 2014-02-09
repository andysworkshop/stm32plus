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
     * Constructor.
     * @param[in] fs_ A reference to the FAT filesystem class. Must stay in scope.
     * @param[in] firstIndex_ The index of the first FAT entry to start iterating at.
     * @param[in] wrap_ true if the iterator should wrap back to zero if
     * it hits the last entry in the FAT. When wrap_ is true iteration will still
     * stop when a wrapped iteration hits firstIndex_.
     */

    FatIterator::FatIterator(FatFileSystem& fs_,uint32_t firstIndex_,bool wrap_) :
      _fs(fs_), _sectorBuffer(_fs.getSectorSizeInBytes()) {

      _firstIndex=firstIndex_;
      _currentIndex=firstIndex_;
      _lastSectorIndex=UINT32_MAX;
      _wrap=wrap_;
      _first=true;
      _entriesPerFat=_fs.getCountOfClusters();
    }

    /**
     * Move to the next position.
     * @return false if it fails.
     */

    bool FatIterator::next() {

      uint32_t sectorIndex;

      if(!_first) {

        // advance the current index until hits the end

        if(++_currentIndex==_entriesPerFat) {

          // if wrapping, go back to zero else end here

          if(_wrap)
            _currentIndex=0;
          else
            return errorProvider.set(ErrorProvider::ERROR_PROVIDER_ITERATOR,E_END_OF_ENTRIES);
        }

        // if we're back at the first index then that's the end

        if(_currentIndex==_firstIndex)
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_ITERATOR,E_END_OF_ENTRIES);
      } else
        _first=false;

      // get the sector index

      sectorIndex=_fs.getBootSector().BPB_RsvdSecCnt+((_currentIndex*_fs.getFatEntrySizeInBytes())/_fs.getSectorSizeInBytes());

      // read the sector if it's new

      if(sectorIndex!=_lastSectorIndex)
        if(!_fs.readSector(sectorIndex,_sectorBuffer))
          return false;

      // done

      return true;
    }

    /**
     * Return the current index.
     * @return The current index into the FAT table (not the FAT table entry content).
     */

    uint32_t FatIterator::current() {
      return _currentIndex;

    }

    /**
     * Return the contents of the FAT entry at the current index.
     * @return The contents of the FAT table at the current position.
     */

    uint32_t FatIterator::currentContent() {
      uint32_t offset;

      offset=(_currentIndex*_fs.getFatEntrySizeInBytes())%_fs.getSectorSizeInBytes();
      return _fs.getFatEntryFromMemory(_sectorBuffer+offset);
    }
  }
}

