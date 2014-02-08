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
   * @param[in] fs_ A reference to the FAT file system. Must not go out of scope.
   * @param[in] firstClusterIndex_ The number of the first cluster in this file.
   * @param[in] extend_ Whether or not to extend the file by allocating new clusters when the end is reached. File writers
   * typically want to extend a file when the end is reached, file readers do not.
   */

    FileSectorIterator::FileSectorIterator(FatFileSystem& fs_,uint32_t firstClusterIndex_,ClusterChainIterator::ExtensionMode extend_)
      : _iterator(fs_,firstClusterIndex_,extend_),_fs(fs_) {

      _sectorIndexInCluster=0x10000;        // force initial move
      _sectorsPerCluster=fs_.getBootSector().BPB_SecPerClus;
    }


  /**
   * Reset the iterator to the first cluster.
   * @param firstClusterNumber_ The first cluster in the file.
   */

    void FileSectorIterator::reset(uint32_t firstClusterNumber_) {
      _sectorIndexInCluster=0x10000;
      _iterator.reset(firstClusterNumber_);
    }


  /**
   * Move to next sector in the file.
   * @see Iterator::next
   * @return false if the move fails due to error or end of sectors.
   */

    bool FileSectorIterator::next() {

    // check if move required

      if(++_sectorIndexInCluster>=_sectorsPerCluster) {
        if(!_iterator.next())
          return false;

        _sectorIndexInCluster=0;
      }

      return true;
    }


  /**
   * Get the current cluster number
   * @return The current cluster number.
   */

    uint32_t FileSectorIterator::getClusterNumber() {
      return _iterator.current();
    }


  /**
   * Read sector from the cluster.
   * @param buffer_ A caller-supplied buffer that will receive the sector data.
   * @return false if the read fails.
   */

    bool FileSectorIterator::readSector(void *buffer_) {
      return _fs.readSectorFromCluster(_iterator.current(),_sectorIndexInCluster,buffer_);
    }


  /**
   * Write sector to the cluster.
   * @param[in] buffer_ A caller supplied buffer that holds the sector data to write.
   * @return false if the write fails.
   */

    bool FileSectorIterator::writeSector(void *buffer_) {
      return _fs.writeSectorToCluster(_iterator.current(),_sectorIndexInCluster,buffer_);
    }


  /**
   * Return the current sector number.
   * @return The number of the current sector (a linear sequence from the start of the device).
   */

    uint32_t FileSectorIterator::current() {
      return _iterator.currentSectorNumber()+_sectorIndexInCluster;
    }
  }
}
