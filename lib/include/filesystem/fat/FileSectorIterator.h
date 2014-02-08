/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Iterate through the sectors of a file
     */

    class FileSectorIterator : public Iterator<uint32_t> {

      protected:
        ClusterChainIterator _iterator;
        FatFileSystem& _fs;
        uint32_t _sectorIndexInCluster;
        uint32_t _sectorsPerCluster;

      public:
        FileSectorIterator(FatFileSystem& fs,uint32_t firstClusterIndex,ClusterChainIterator::ExtensionMode extend);

        /**
         * Virtual destructor, does nothing.
         */

        virtual ~FileSectorIterator() {
        }

        uint32_t getClusterNumber();

        bool readSector(void *buffer);
        bool writeSector(void *buffer);

        void reset(uint32_t firstClusterNumber);

        // overrides from Iterator

        virtual bool next() override;
        virtual uint32_t current() override;
    };

  }
}
