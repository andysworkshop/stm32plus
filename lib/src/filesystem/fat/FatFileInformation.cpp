/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {
  namespace fat {

    /**
     * Constructor. Extract attributes from dirent.
     *
     * @param[in] filename_ The name of the file to get information for.
     * @param[in] dirent_ The corresponding dirent for this file.
     */

    FatFileInformation::FatFileInformation(const char *filename_,const DirectoryEntryWithLocation& dirent_) {

      _attributes=dirent_.Dirent.sdir.DIR_Attr;
      _filename=filename_;
      _length=dirent_.Dirent.sdir.DIR_FileSize;

      DirectoryEntryIterator::calculateUnixTime(dirent_.Dirent.sdir.DIR_CrtDate,dirent_.Dirent.sdir.DIR_CrtTime,_creationDate);
      DirectoryEntryIterator::calculateUnixTime(dirent_.Dirent.sdir.DIR_WrtDate,dirent_.Dirent.sdir.DIR_WrtTime,_lastWriteDateTime);
      DirectoryEntryIterator::calculateUnixTime(dirent_.Dirent.sdir.DIR_LstAccDate,0,_lastAccessDateTime);
    }

    /**
     * @copydoc FileInformation::getAttributes
     */

    uint32_t FatFileInformation::getAttributes() const {
      return _attributes;
    }

    /**
     * @copydoc FileInformation::getFilename
     */

    const char *FatFileInformation::getFilename() const {
      return _filename;
    }

    /**
     * @copydoc FileInformation::getCreationDateTime
     */

    time_t FatFileInformation::getCreationDateTime() const {
      return _creationDate;
    }

    /**
     * @copydoc FileInformation::getLastWriteDateTime
     */

    time_t FatFileInformation::getLastWriteDateTime() const {
      return _lastWriteDateTime;
    }

    /**
     * @copydoc FileInformation::getLastAccessDateTime
     */

    time_t FatFileInformation::getLastAccessDateTime() const {
      return _lastAccessDateTime;
    }

    /**
     * @copydoc FileInformation::getLength
     */

    uint32_t FatFileInformation::getLength() const {
      return _length;
    }

  }
}
