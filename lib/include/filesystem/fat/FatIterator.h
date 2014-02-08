/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Iterate over the entries in the FAT sequentially.
     */

    class FatIterator : public Iterator<uint32_t> {

      private:
        FatFileSystem& _fs;
        uint32_t _firstIndex;
        uint32_t _currentIndex;
        uint32_t _entriesPerFat;
        uint32_t _lastSectorIndex;
        bool _wrap;
        bool _first;
        ByteMemblock _sectorBuffer;

      public:
        FatIterator(FatFileSystem& fs_,uint32_t firstIndex_,bool wrap_);

        uint32_t currentContent(); // get the content of the current index

        // overrides from Iterator

        virtual bool next() override;
        virtual uint32_t current() override; // get the current index
    };

  }
}
