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
     * Return true if this a file entry
     *
     * @return true if it's a file
     */

    bool DirectoryEntryWithLocation::isFile() {
      return (Dirent.sdir.DIR_Attr & (DirectoryEntry::ATTR_VOLUME_ID | DirectoryEntry::ATTR_DIRECTORY))==0;
    }

    /**
     * Return true if this a directory entry.
     *
     * @return true if it's a directory.
     */

    bool DirectoryEntryWithLocation::isDirectory() {
      return (Dirent.sdir.DIR_Attr & DirectoryEntry::ATTR_DIRECTORY)!=0;
    }

    /**
     * Copy times across from here to the destination.
     * @param[in] dest_ The target for the copy of the date/time members.
     */

    void DirectoryEntryWithLocation::copyTimesTo(DirectoryEntry& dest_) {

      dest_.sdir.DIR_CrtDate=Dirent.sdir.DIR_CrtDate;
      dest_.sdir.DIR_CrtTime=Dirent.sdir.DIR_CrtTime;
      dest_.sdir.DIR_CrtTimeTenth=Dirent.sdir.DIR_CrtTimeTenth;
      dest_.sdir.DIR_LstAccDate=Dirent.sdir.DIR_LstAccDate;
      dest_.sdir.DIR_WrtDate=Dirent.sdir.DIR_WrtDate;
      dest_.sdir.DIR_WrtTime=Dirent.sdir.DIR_WrtTime;
    }
  }
}
