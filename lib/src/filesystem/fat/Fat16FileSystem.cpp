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

    Fat16FileSystem::Fat16FileSystem(BlockDevice& blockDevice,const TimeProvider& timeProvider,const fat::BootSector& bootSector,uint32_t firstSectorIndex,uint32_t countOfClusters) :
      FatFileSystem(blockDevice,timeProvider,bootSector,firstSectorIndex,countOfClusters) {

      // up-front calculation of important constants

      _rootDirFirstSector=_bootSector.BPB_RsvdSecCnt+(_bootSector.BPB_NumFATs*_bootSector.BPB_FATSz16);
      _rootDirSectors=((_bootSector.BPB_RootEntCnt*32)+(_bootSector.BPB_BytsPerSec-1))/_bootSector.BPB_BytsPerSec;
      _firstDataSector=_bootSector.BPB_RsvdSecCnt+(_bootSector.BPB_NumFATs*_bootSector.BPB_FATSz16)+_rootDirSectors;
    }

    /**
     * Destructor
     */

    Fat16FileSystem::~Fat16FileSystem() {
    }

    /**
     * @copydoc FatFileSystem::getFileSystemType
     * Returns FileSystemType::Fat16
     */

    FileSystem::FileSystemType Fat16FileSystem::getFileSystemType() const {
      return Fat16;
    }

    /*
     * @copydoc FatFileSystem::getFatEntryFromMemory
     */

    uint32_t Fat16FileSystem::getFatEntryFromMemory(void *addr) const {
      return *static_cast<uint16_t *> (addr);
    }

    /*
     * @copydoc FatFileSystem::setFatEntryToMemory
     */

    void Fat16FileSystem::setFatEntryToMemory(void *addr,uint32_t entry) const {
      *static_cast<uint16_t *> (addr)=static_cast<uint16_t> (entry);
    }

    /**
     * @copydoc FatFileSystem::getFatEntrySizeInBytes
     * Returns 2.
     */

    uint32_t Fat16FileSystem::getFatEntrySizeInBytes() const {
      return 2;
    }

    /**
     * @copydoc FatFileSystem::getBadClusterMarker
     * Returns 0xFFF7.
     */

    uint32_t Fat16FileSystem::getBadClusterMarker() const {
      return 0xFFF7;
    }

    /**
     * @copydoc FatFileSystem::getSectorsPerFat
     */

    uint32_t Fat16FileSystem::getSectorsPerFat() const {
      return _bootSector.BPB_FATSz16;
    }

    /**
     * @copydoc FatFileSystem::isEndOfClusterChainMarker
     * Returns true if >= 0xFFF8.
     */

    bool Fat16FileSystem::isEndOfClusterChainMarker(uint32_t clusterNumber) const {
      return clusterNumber>=0xFFF8;
    }

    /**
     * @copydoc FatFileSystem::getEndOfClusterChainMarker
     * Returns 0xFFF8.
     */

    uint32_t Fat16FileSystem::getEndOfClusterChainMarker() const {
      return 0xFFF8;
    }

    /**
     * Return the number of sectors in the root directory.
     * @return The number of sectors in this root directory.
     */

    uint32_t Fat16FileSystem::getRootDirSectors() const {
      return _rootDirSectors;
    }

    /**
     * @copydoc FatFileSystem::getRootDirectoryIterator
     */

    DirectoryEntryIterator *Fat16FileSystem::getRootDirectoryIterator(DirectoryEntryIterator::Options options) {
      return new Fat16RootDirectoryEntryIterator(*this,options);
    }
  }
}
