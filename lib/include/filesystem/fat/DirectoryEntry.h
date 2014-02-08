/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * FAT directory entry. There are no data members here, just the union of the short and long directory entry
     * implementations.
     */

    struct DirectoryEntry {

    /**
     * Attributes in a directory entry
     */

      enum {
        /// Entry is read only
        ATTR_READ_ONLY = 0x1,

        /// Entry is hidden
        ATTR_HIDDEN    = 0x2,

        /// Entry is a system file
        ATTR_SYSTEM    = 0x4,

        /// Entry is the volume label
        ATTR_VOLUME_ID = 0x8,

        /// Entry is a directory
        ATTR_DIRECTORY = 0x10,

        /// Entry has been archived
        ATTR_ARCHIVE   = 0x20,

        /// The special combination that indicates a long name entry
        ATTR_LONG_NAME = ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID,

        /// The long name mask
        ATTR_LONG_NAME_MASK = ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID | ATTR_DIRECTORY | ATTR_ARCHIVE,

        /// The special bit that is set to indicate the last ordinal in a long name sequence
        LAST_LONG_ENTRY = 0x40
      };

      /**
       * The 32 byte structure itself
       */

      union {
        ShortDirectoryEntry sdir;
        LongDirectoryEntry ldir;
      };
    };
  }
}
