/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @namespace fat 'fat' is a sub-namespace of stm32plus that contains everything to do with the FAT
   * filesystem. Generally you won't have to go here because the FileSystem API is exposed through the
   * stm32plus::FileSystem class.
   */

  namespace fat {

    class FatDirectoryIterator;

    /**
     * @brief Base class for FAT filesystems.
     *
     * Exposes the common functionality of FAT16 and FAT32 filesystems.
     */

    class FatFileSystem : public FileSystem {
      protected:
        BootSector _bootSector; // the BPB

        uint32_t _firstDataSector; // index of the first data sector
        uint32_t _fatFirstSector; // first sector of the FAT
        uint32_t _rootDirFirstSector; // first sector of the root directory
        uint32_t _countOfClusters; // total # of clusters

      protected:
        FatFileSystem(BlockDevice& blockDevice,const TimeProvider& timeProvider,const fat::BootSector& bootSector,uint32_t firstSectorIndex,uint32_t countOfClusters);

        bool getDirectoryEntry(TokenisedPathname& pathTokens,DirectoryEntryWithLocation& direntCopy);
        bool getDirectoryIteratorPointingToFile(TokenisedPathname& pathTokens,FatDirectoryIterator *& parent);
        bool getParentDirectoryFirstCluster(TokenisedPathname& pathTokens,uint16_t* lo,uint16_t* hi);
        bool fullyDelete(FatDirectoryIterator& it);
        bool deleteDirents(FatDirectoryIterator& fdi);

      public:

        /**
         * Error codes
         */

        enum {
          /// Cluster contains the bad marker
          E_BAD_CLUSTER=1,

          /// Expected a file, got something else.
          E_NOT_A_FILE,

          /// Expected a directory, got something else.
          E_NOT_A_DIRECTORY,

          /// Expected an empty directory but it has content.
          E_DIRECTORY_NOT_EMPTY
        };

        // factory constructor/destructor
        static bool getInstance(BlockDevice& blockDevice,const TimeProvider& timeProvider,FatFileSystem*& newFileSystem);

        virtual ~FatFileSystem();

        // overrides from FileSystem

        virtual bool getFileInformation(const char *filename,FileInformation*& finfo) override;
        virtual bool getDirectoryIterator(const char *pathname,DirectoryIterator*& newIterator) override;
        virtual bool createFile(const char *filename) override;
        virtual bool openFile(const char *filename,File*& newFile) override;
        virtual bool deleteFile(const char *filename) override;
        virtual bool deleteDirectory(const char *dirname) override;
        virtual bool createDirectory(const char *dirname) override;
        virtual uint32_t getSectorSizeInBytes() const override;
        virtual bool getFreeSpace(uint32_t& freeUnits,uint32_t& unitsMultiplier) override;

        const fat::BootSector& getBootSector() const;
        uint32_t getCountOfClusters() const;
        uint32_t clusterToSector(uint32_t clusterNumber) const;
        uint32_t getRootDirectoryFirstSector() const;
        bool readSectorFromCluster(uint32_t clusterIndex,uint32_t sectorIndexInCluster,void *buffer);
        bool writeSectorToCluster(uint32_t clusterIndex,uint32_t sectorIndexInCluster,void *buffer);
        bool readFatEntry(uint32_t clusterNumber,uint32_t& fatEntryForCluster);
        bool allocateNewCluster(uint32_t anyClusterInChain,uint32_t& newCluster);
        bool findFreeCluster(uint32_t& freeCluster);
        bool writeFatEntry(uint32_t fatEntryIndex,uint32_t fatEntryContent);
        bool writeDirectoryEntry(DirectoryEntryWithLocation& dirent);
        bool deAllocateClusterChain(uint32_t firstCluster);
        bool directoryHasContent(const char *dirName,bool& hasContent);

        /**
         * Get a FAT entry from memory. 16-bit entries are up-cast to fill 32 bits.
         * @param[in] addr the address to extract the entry from.
         * @return The extracted FAT entry.
         */

        virtual uint32_t getFatEntryFromMemory(void *addr) const=0;

        /**
         * Write a FAT entry into memory.
         * @param[in] addr The address to write to.
         * @param[in] entry The entry to write, which may be down-cast to 16-bits by the implementation.
         */

        virtual void setFatEntryToMemory(void *addr,uint32_t entry) const=0;


        /**
         * Get the size of a FAT entry in bytes.
         * @return 2 or 4, depending on whether this is FAT16 or FAT32.
         */

        virtual uint32_t getFatEntrySizeInBytes() const=0;

        /**
         * Get the bad cluster marker for this file system.
         * @return The bad cluster marker.
         */

        virtual uint32_t getBadClusterMarker() const=0;

        /**
         * Get the number of sectors per FAT.
         * @return The number of sectors in each FAT.
         */

        virtual uint32_t getSectorsPerFat() const=0;


        /**
         * Test if this cluster number is the end of cluster (EOC) marker.
         * @param[in] clusterNumber The value to test.
         * @return true if it is the EOC marker, false if not.
         */

        virtual bool isEndOfClusterChainMarker(uint32_t clusterNumber) const=0;

        /**
         * Get the EOC marker.
         * @return The EOC marker.
         */

        virtual uint32_t getEndOfClusterChainMarker() const=0;

        /**
         * Get an iterator on to the root directory of this filesystem.
         * @param[in] options The options to use while iterating over this directory.
         * @return A directory entry pointer. The caller owns this pointer and must delete it
         * when finished. nullptr is returned on failure and the error provider is updated.
         */

        virtual DirectoryEntryIterator *getRootDirectoryIterator(DirectoryEntryIterator::Options options)=0;
    };
  }
}
