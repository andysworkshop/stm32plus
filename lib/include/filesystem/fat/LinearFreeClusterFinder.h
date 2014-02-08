/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Free cluster finder suitable for rotating disks.
     *
     * Find a free cluster by linearly looking in the FAT from cluster zero until a free entry is found.
     * Devices that do not have an MTBF for each block should probably use this as it will tend to keep
     * clusters together.
     */

    class LinearFreeClusterFinder : public IteratingFreeClusterFinder {
      public:
        LinearFreeClusterFinder(FatFileSystem& fs);

        /**
         * Virtual destructor. Does nothing.
         */

        virtual ~LinearFreeClusterFinder() {
        }


        /**
         * Find a sequence of at least the given number of clusters that are all free
         */

        bool findMultipleSequential(uint32_t clustersRequired,uint32_t& firstCluster);
    };
  }
}
