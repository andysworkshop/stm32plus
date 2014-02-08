/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Handle constructing a filename from one or more directory entries.
     *
     * This class is used for extracting the true filename from one or more directory
     * entries in a FAT directory. Long filenames are the tricky ones because they
     * occupy more than one entry and have a strange encoding driven by the requirement
     * to be compatible with, and ignored by, earlier systems that don't understand
     * the new entries.
     */

    class FilenameHandler {

      protected:
        enum State {
          STATE_NONE, STATE_READY, STATE_LONG_FILENAME_IN_PROGRESS
        };

        State _state;
        char *_longFilename;             // filenames longer than 29 characters get allocated on the heap
        char _shortFilename[30];         // filenames shorter than 30 characters - quick access
        uint16_t _length;                // length not including nul
        uint8_t _currentIndex;
        uint32_t _direntCount;
        uint32_t *_sectorIndices;         // two arrays to collect sector locations of each dirent that makes
        uint32_t *_sectorOffsetIndices;   // up the filename. Used if the caller decides to delete the file

        bool handleLongName(DirectoryEntryWithLocation& entry);
        bool handleShortName(DirectoryEntryWithLocation& entry);

        void setName(const void *name,uint16_t length);
        void setSectorDetails(DirectoryEntryWithLocation& entry,uint32_t sectorIndex,uint32_t newSize=0);

        void initialise();
        void cleanup();

      public:

        /**
         * Error codes
         */

        enum {
          /// The directory is corrupt. The sequence of entries does not make sense.
          E_INVALID_STATE=1,

          /// The long filename is corrupted
          E_CORRUPT_LONG_FILENAME=2
        };

      public:
        FilenameHandler();
        ~FilenameHandler();

        void reinitialise();
        bool handleEntry(DirectoryEntryWithLocation& entry);
        const char *getFilename();

        uint32_t *getSectorIndices();
        uint32_t *getSectorOffsetIndices();
        uint32_t getDirentCount();
    };
  }
}
