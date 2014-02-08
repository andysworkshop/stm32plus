/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Dirent with sector location on device
     */

    struct DirectoryEntryWithLocation {

        /// The directory entry structure
        DirectoryEntry Dirent;

        /// The sector number on the FS where this entry came from
        uint32_t SectorNumber;

        /// The index within the sector where this entry came from. This is a byte offset.
        uint32_t IndexWithinSector;

        bool isFile();
        bool isDirectory();
        void copyTimesTo(DirectoryEntry& dest_);
    };
  }
}
