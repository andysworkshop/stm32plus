/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Base class for free cluster finders
     */

    class FreeClusterFinder {
      protected:
        FatFileSystem& _fs;

      protected:
        FreeClusterFinder(FatFileSystem& fs);

      public:

        /**
         * Error codes
         */

        enum {
          /// The disk is full. There are no more free clusters available.
          E_NO_FREE_CLUSTERS=1
        };


        /**
         * Virtual destructor. Does nothing
         */

        virtual ~FreeClusterFinder() {
        }


        /**
         * Derivations of this class must implement this. Finds the next free cluster and
         * returns it to the client in the freeCluster_ reference.
         *
         * @param[in] freeCluster_ The next free cluster.
         * @return false if it fails, check error provider for details.
         */

        virtual bool find(uint32_t& freeCluster)=0;
    };

  }
}
