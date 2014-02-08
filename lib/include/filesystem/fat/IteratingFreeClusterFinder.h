/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Free cluster finder that uses a FAT iterator to find free clusters.
     *
     * This is a base class
     */

    class IteratingFreeClusterFinder : public FreeClusterFinder {

      protected:
        FatIterator _iterator;

      protected:
        IteratingFreeClusterFinder(FatFileSystem& fs,uint32_t firstFatEntry);

      public:

      /**
       * Virtual destructor, does nothing
       */

        virtual ~IteratingFreeClusterFinder() {
        }

        // overrides from FreeClusterFinder

        virtual bool find(uint32_t& freeCluster) override;
    };

  }
}
