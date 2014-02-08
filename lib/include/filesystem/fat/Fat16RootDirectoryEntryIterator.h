/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

  /**
   * @brief Iterator for directory entries in the Fat16 root directory.
   *
   * The Fat16 root directory is a contiguous block of sectors.
   */

    class Fat16RootDirectoryEntryIterator : public DirectoryEntryIterator {

      protected:
        uint32_t _currentIndex;
        uint32_t _firstEntryInCurrentSector;
        uint32_t _entriesPerSector;
        uint32_t _rootDirMaxEntries;
        Memblock<uint8_t> _currentSector;

      protected:
        uint32_t entryOffsetInSector(uint32_t entryIndex) const;
        bool isEntryInCurrentSector() const;
        uint32_t sectorIndexForEntry(uint32_t entryIndex) const;
        bool readSectorForEntry(uint32_t entryIndex);
        
      // overrides from DirectoryEntryIterator

        virtual bool internalNext() override;
        virtual bool extendDirectory(DirectoryEntry *dirents,uint32_t direntCount) override;

      // overrides from ResetableIterator

        virtual void reset() override;

      public:

        /**
         * Error codes
         */

        enum {

          /// The root directory is full
          E_ROOT_DIRECTORY_FULL = 1
        };

      public:
        Fat16RootDirectoryEntryIterator(FatFileSystem& fs_,Options options_);

        /**
         * Virtual destructor. Does nothing
         */

        virtual ~Fat16RootDirectoryEntryIterator() {}
    };

  }
}

