/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Iterate over normal directory entries.
     *
     * A normal directory entry iterator iterates over directory entries stored in a "file" structure,
     * i.e. a sequence of clusters specified by their FAT entries.
     */

    class NormalDirectoryEntryIterator : public DirectoryEntryIterator {
      protected:
        uint32_t _indexWithinSector;
        FileSectorIterator _iterator;
        uint32_t _firstClusterIndex;
        uint32_t _currentDirentIndex;

      protected:
        // overrides from DirectoryEntryIterator

        virtual bool internalNext() override;
        virtual bool extendDirectory(DirectoryEntry *dirents,uint32_t direntCount) override;

        // overrides from ResettableIterator

        virtual void reset() override;

      public:
        NormalDirectoryEntryIterator(FatFileSystem& fs,uint32_t firstClusterIndex,Options options);

        /**
         * Virtual destructor. Does nothing.
         */

        virtual ~NormalDirectoryEntryIterator() {
        }
    };
  }
}
