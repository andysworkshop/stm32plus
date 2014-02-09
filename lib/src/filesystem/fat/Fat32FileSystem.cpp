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
     * Constructor. This is normally called by the factory method in FileSystem.
     *
     * @param[in] blockDevice The block device that holds this filesystem.
     * @param[in] timeProvider A provider of time information for write access to the file system.
     * @param[in] bootSector The boot sector (BPB) for this file system.
     * @param[in] firstSectorIndex The index of the first sector on the block device of this file system.
     * @param[in] countOfClusters The total clusters allocated to this file system.
     *
     * @see FileSystem::getInstance
     */

    Fat32FileSystem::Fat32FileSystem(BlockDevice& blockDevice,const TimeProvider& timeProvider,const fat::BootSector& bootSector,uint32_t firstSectorIndex,uint32_t countOfClusters) :
      FatFileSystem(blockDevice,timeProvider,bootSector,firstSectorIndex,countOfClusters) {

      _rootDirFirstSector=clusterToSector(_bootSector.fat32.BPB_RootClus);
      _firstDataSector=_bootSector.BPB_RsvdSecCnt+(_bootSector.BPB_NumFATs*_bootSector.fat32.BPB_FATSz32);
    }

    /**
     * Destructor
     */

    Fat32FileSystem::~Fat32FileSystem() {
    }

    /**
     * @copydoc FatFileSystem::getFileSystemType
     * Returns FileSystemType::Fat32
     */

    FileSystem::FileSystemType Fat32FileSystem::getFileSystemType() const {
      return Fat32;
    }

    /**
     * @copydoc FatFileSystem::getFatEntrySizeInBytes
     * Returns 4.
     */

    uint32_t Fat32FileSystem::getFatEntrySizeInBytes() const {
      return 4;
    }

    /**
     * @copydoc FatFileSystem::getSectorsPerFat
     */

    uint32_t Fat32FileSystem::getSectorsPerFat() const {
      return _bootSector.fat32.BPB_FATSz32;
    }

    /**
     * @copydoc FatFileSystem::getBadClusterMarker
     * Returns 0x0FFFFFF7.
     */

    uint32_t Fat32FileSystem::getBadClusterMarker() const {
      return 0x0FFFFFF7;
    }

    /**
     * @copydoc FatFileSystem::isEndOfClusterChainMarker
     * Returns 0x0FFFFFF8.
     */

    bool Fat32FileSystem::isEndOfClusterChainMarker(uint32_t clusterNumber) const {
      return clusterNumber>=0x0FFFFFF8;
    }

    /**
     * @copydoc FatFileSystem::getEndOfClusterChainMarker
     * Returns 0xFFFFFFF8.
     */

    uint32_t Fat32FileSystem::getEndOfClusterChainMarker() const {
      return 0xFFFFFFF8;
    }

    /**
     * @copydoc FatFileSystem::getFatEntryFromMemory
     * Returns the entry and'd with 0x0FFFFFFF;
     */

    uint32_t Fat32FileSystem::getFatEntryFromMemory(void *addr) const {
      return *static_cast<uint32_t *> (addr)&0x0FFFFFFF;
    }

    /**
     * @copydoc FatFileSystem::setFatEntryToMemory
     */

    void Fat32FileSystem::setFatEntryToMemory(void *addr,uint32_t entry) const {
      *static_cast<uint32_t *> (addr)=entry;
    }

    /**
     * @copydoc FatFileSystem::getRootDirectoryIterator
     */

    DirectoryEntryIterator *Fat32FileSystem::getRootDirectoryIterator(DirectoryEntryIterator::Options options) {
      return new NormalDirectoryEntryIterator(*this,_bootSector.fat32.BPB_RootClus,options);
    }
  }
}
