/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

  /**
   * FAT16 implementation of the filesystem
   */

    class Fat16FileSystem : public FatFileSystem {

      protected:
        uint32_t _rootDirSectors;             // number of sectors reserved for the root directory

      public:
        Fat16FileSystem(
          BlockDevice& blockDevice,
          const TimeProvider& timeProvider,
          const fat::BootSector& bootSector,
          uint32_t firstSectorIndex,
          uint32_t countOfClusters);
        
        virtual ~Fat16FileSystem();

        uint32_t getRootDirSectors() const;

        // overrides from FatFileSystem

        virtual FileSystemType getFileSystemType() const override;
        virtual uint32_t getFatEntryFromMemory(void *addr) const override;
        virtual void setFatEntryToMemory(void *addr,uint32_t entry) const override;
        virtual uint32_t getFatEntrySizeInBytes() const override;
        virtual uint32_t getBadClusterMarker() const override;
        virtual uint32_t getEndOfClusterChainMarker() const override;
        virtual uint32_t getSectorsPerFat() const override;
        virtual bool isEndOfClusterChainMarker(uint32_t clusterNumber) const override;
        virtual DirectoryEntryIterator *getRootDirectoryIterator(DirectoryEntryIterator::Options options) override;
    };
  }
}
