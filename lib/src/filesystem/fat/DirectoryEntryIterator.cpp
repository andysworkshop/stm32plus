/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {
  namespace fat {

    /*
     * Constructor
     */

    DirectoryEntryIterator::DirectoryEntryIterator(FatFileSystem& fs_,Options options_) :
      _fs(fs_) {
      _options=options_;
    }

    /*
     * return the current entry
     */

    DirectoryEntryWithLocation& DirectoryEntryIterator::current() {
      return _currentEntry;
    }

    /**
     * Return the current filename. If a long name is present then it will be returned.
     * @return The current filename.
     */

    const char *DirectoryEntryIterator::getFilename() {
      return _filename.getFilename();
    }

    /**
     * Get the filename handler.
     * @return A reference to the filename handler.
     */

    FilenameHandler& DirectoryEntryIterator::getFilenameHandler() {
      return _filename;
    }

    /**
     * @copydoc Iterator::next
     *
     * The options are taken into account and entries may be skipped if that's what has been specified.
     */

    bool DirectoryEntryIterator::next() {

      bool repeat,deleted;

      errorProvider.clear();

      _filename.reinitialise();

      do {

        if(!internalNext())
          return false;

        // handle filename as long as this entry is not deleted

        DirectoryEntry& dirent=_currentEntry.Dirent;

        deleted=dirent.sdir.DIR_Name[0] == 0xe5;
        if(!deleted && !_filename.handleEntry(_currentEntry))
          return false;

        // repeat if entry is deleted and we are skipping those or if entry
        // is a long name entry and we are parsing them in one step

        if(deleted)
          repeat=(_options & OPT_SKIP_DELETED) != 0;
        else if((dirent.sdir.DIR_Attr & DirectoryEntry::ATTR_LONG_NAME_MASK) == DirectoryEntry::ATTR_LONG_NAME)
          repeat=(_options & OPT_PARSE_LONG_NAMES) != 0;
        else
          repeat=false;

      } while(repeat);

      // reset the dates and times - these are lazily calculated as required

      _lastAccessDateTime=_lastWriteDateTime=_creationDateTime=INT32_MAX;

      return true;
    }

    /**
     * Get the last access date/time as a Unix time_t
     *
     * @return The last access date/time.
     */

    time_t DirectoryEntryIterator::getLastAccessDateTime() {

      if(_lastAccessDateTime != INT32_MAX)
        return _lastAccessDateTime;

      return calculateUnixTime(_currentEntry.Dirent.sdir.DIR_LstAccDate,0,_lastAccessDateTime);
    }

    /**
     * Get the last write date/time as a unix time_t
     *
     * @return The last write date/time.
     */

    time_t DirectoryEntryIterator::getLastWriteDateTime() {

      if(_lastWriteDateTime != INT32_MAX)
        return _lastWriteDateTime;

      DirectoryEntry& dirent=_currentEntry.Dirent;
      return calculateUnixTime(dirent.sdir.DIR_WrtDate,dirent.sdir.DIR_WrtTime,_lastWriteDateTime);
    }

    /**
     * Get the creation date/time as a unix time_t
     *
     * @return The creation date/time.
     */

    time_t DirectoryEntryIterator::getCreationDateTime() {
      if(_creationDateTime != INT32_MAX)
        return _creationDateTime;

      DirectoryEntry& dirent=_currentEntry.Dirent;
      return calculateUnixTime(dirent.sdir.DIR_CrtDate,dirent.sdir.DIR_CrtTime,_creationDateTime);
    }

    /**
     * Static helper to calculate Unix date/time from MSDOS date+time
     * @param[in] date_ MSDOS date.
     * @param[in] time_ MSDOS time.
     * @param[out] result_ The Unix time_t.
     *
     * @return The same as result_.
     */

    time_t DirectoryEntryIterator::calculateUnixTime(uint16_t date_,uint16_t time_,time_t& result_) {

      struct tm timeInfo;

      timeInfo.tm_mday=date_ & 31; // 5 bits (base=1)
      timeInfo.tm_mon=((date_ >> 5) & 15) - 1; // 4 bits (base=1)
      timeInfo.tm_year=((date_ >> 9) & 127) + 80; // 7 bits (base=1980)

      timeInfo.tm_sec=(time_ & 31) * 2; // 5 bits (resolution=2 secs)
      timeInfo.tm_min=(time_ >> 5) & 63; // 6 bits (base=0)
      timeInfo.tm_hour=(time_ >> 11) & 31; // 5 bits (base=0)

      result_=mktime(&timeInfo);
      return result_;
    }

    /**
     * Convert MSDOS date+time from unix time
     *
     * @param[in] unixTime_ The Unix time_t to convert.
     * @param[out] date_ The MSDOS FAT date.
     * @param[out] time_ The MSDOS FAT time.
     */

    void DirectoryEntryIterator::calculateFatDateTime(time_t unixTime_,uint16_t& date_,uint16_t& time_) {

      struct tm *timeInfo;

      // get components

      timeInfo=gmtime(&unixTime_);

      // set the date

      date_=(static_cast<uint32_t> (timeInfo->tm_year - 80) << 9) | (static_cast<uint32_t> (timeInfo->tm_mon + 1) << 5) | timeInfo->tm_mday;

      // set the time

      time_=(static_cast<uint32_t> (timeInfo->tm_hour) << 11) | (static_cast<uint32_t> (timeInfo->tm_min) << 5) | (static_cast<uint32_t> (timeInfo->tm_sec) >> 1);
    }

    /**
     * Write some new dirents to this directory. If the directory contains a contiguous sequence of deleted
     * large enough to hold the new sequence then these entries are used. If not then new entries are allocated
     * on the end of the directory. Whichever happens, the new dirents are guaranteed not to be broken up.
     *
     * @param[in] dirents_ The dirents to write to this directory.
     * @param[in] direntCount_ The number of dirents to write.
     * @return false if it fails.
     */

    bool DirectoryEntryIterator::writeDirents(DirectoryEntry *dirents_,int direntCount_) {

      DirectoryEntryWithLocation *entries;
      Options oldOptions;
      int foundCount,i;

      // save old options and make sure we see deleted entries now

      oldOptions=_options;
      _options=OPT_PARSE_LONG_NAMES;

      // reset the iterator

      reset();

      // start searching for deleted entries

      foundCount=0;
      entries=new DirectoryEntryWithLocation[direntCount_];

      while(next()) {

        if(current().Dirent.sdir.DIR_Name[0] == 0xE5) {

          // if first in sequence, remember it

          entries[foundCount++]=current(); // struct copy

          // if found enough, remember the last

          if(foundCount == direntCount_) {

            // finished with this

            _options=oldOptions;

            // now write the dirents

            for(i=0;i < foundCount;i++) {

              entries[i].Dirent=dirents_[i]; // struct copy

              if(!_fs.writeDirectoryEntry(entries[i])) {
                delete[] entries;
                return false;
              }
            }

            delete[] entries;
            return true;
          }
        } else
          foundCount=0; // reset the counter
      }

      // we're done with these

      delete[] entries;
      _options=oldOptions;

      // not enough contiguous deleted entries available
      // extend the directory to make space for them

      return extendDirectory(dirents_,direntCount_);
    }
  }
}
