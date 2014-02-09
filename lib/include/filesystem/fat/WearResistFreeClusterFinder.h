/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Free cluster finder suitable for flash media.
     *
     * Find a free cluster by randomly picking a position in the FAT and then iterating
     * until a free entry is found. Scattering writes is desirable for flash devices that
     * will wear out after a certain number of writes. Since the FAT blocks are heavily
     * hit during write activity the benefit of this approach is limited unless the FAT is
     * cached.
     */

    class WearResistFreeClusterFinder : public IteratingFreeClusterFinder {
      public:
        WearResistFreeClusterFinder(FatFileSystem& fs_);
        virtual ~WearResistFreeClusterFinder() {
        }
    };
  }
}
