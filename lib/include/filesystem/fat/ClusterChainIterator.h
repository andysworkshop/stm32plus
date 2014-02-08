/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

  /**
   * @brief Iterate over the clusters in a chain
   */

    class ClusterChainIterator : public Iterator<uint32_t> {

      public:
        /**
         * Whether to extend the chain when the end is hit
         */

        enum ExtensionMode {
          /// Extend the chain.
          extensionExtend,

          /// Don't extend the chain.
          extensionDontExtend };

      protected:
        bool _first;
        ExtensionMode _extend;
        uint32_t _currentClusterNumber;
        uint32_t _firstClusterNumber;
        FatFileSystem& _fs;

      public:
        ClusterChainIterator(FatFileSystem& fs_,uint32_t firstClusterNumber_,ExtensionMode extend_);

        /**
         * Virtual destructor, do nothing.
         */

        virtual ~ClusterChainIterator() {}

        uint32_t currentSectorNumber();
        void reset(uint32_t firstClusterNumber_);

        // overrides from Iterator

        virtual bool next() override;
        virtual uint32_t current() override;
    };

  }
}


