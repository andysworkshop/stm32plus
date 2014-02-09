/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

  class FatFileSystem;


  /**
   * @brief An iterator over the directory entry structures in a FAT filesystem.
   *
   * A directory entry iterator iterates over the 32 byte DirectoryEntry structures in
   * a FAT directory. A base class is defined here so that the special case of iterating
   * over the FAT16 root directory is abstracted.
   */

    class DirectoryEntryIterator : public ResetableIterator<DirectoryEntryWithLocation&> {

      public:
        /**
         * Options bitmask for the iteration
         */

        enum Options {
          /// No options.
          OPT_NONE              = 0x0,

          /// Entries that are marked as deleted will be skipped over.

          OPT_SKIP_DELETED      = 0x1,

          /// Long names are spread over multiple directory entries. Set this option to parse them as one entry.
          OPT_PARSE_LONG_NAMES  = 0x2,

          /// Default options are to skip deleted and parse long names
          OPT_DEFAULT_REAL_ENTRIES = OPT_SKIP_DELETED | OPT_PARSE_LONG_NAMES
        };

      protected:
        FatFileSystem& _fs;
        DirectoryEntryWithLocation _currentEntry;
        Options _options;
        FilenameHandler _filename;
        time_t _lastAccessDateTime;
        time_t _lastWriteDateTime;
        time_t _creationDateTime;

      protected:
        DirectoryEntryIterator(FatFileSystem& fs_,Options options_);

        virtual bool internalNext()=0;
        virtual bool extendDirectory(DirectoryEntry *dirents_,uint32_t direntCount_)=0;

      public:

        /**
         * Virtual destructor, does nothing
         */

        virtual ~DirectoryEntryIterator() {}

        const char *getFilename();
        FilenameHandler& getFilenameHandler();

        time_t getLastAccessDateTime();
        time_t getLastWriteDateTime();
        time_t getCreationDateTime();

        bool writeDirents(DirectoryEntry *dirents_,int direntCount_);

      // helpers for converting dates and times for directory entries

        static time_t calculateUnixTime(uint16_t date_,uint16_t time_,time_t& result_);
        static void calculateFatDateTime(time_t unixTime_,uint16_t& date_,uint16_t& time);

      // overrides from Iterator<DirectoryEntry>

        virtual DirectoryEntryWithLocation& current() override;
        virtual bool next() override;
    };

  }
}
