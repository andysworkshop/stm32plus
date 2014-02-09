/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief File information base class
   *
   * Defines the valid file attributes and the methods that a derived class must
   * supply.
   */

  class FileInformation {

    public:

      virtual ~FileInformation() {}

      /**
       * Abstract file attributes. Full set may not be supported by all file systems.
       * These attributes are intended to match FAT attributes 1:1 and will always do so.
       */

      enum FileAttributes {
        /// File is read only
        ATTR_READ_ONLY=0x1,

        /// File is hidden
        ATTR_HIDDEN=0x2,

        /// File is a system file
        ATTR_SYSTEM=0x4,

        /// File is actually the volume label
        ATTR_VOLUME_ID=0x8,

        /// File is a directory
        ATTR_DIRECTORY=0x10,

        /// File has been archived
        ATTR_ARCHIVE=0x20
      };


      /**
       * Get the file attributes as a bitmask of FileAttributes enumeration.
       * @return The file attributes.
       */

      virtual uint32_t getAttributes() const=0;

      /**
       * Get an internal pointer to the file or directory name.
       * @return The filename.
       */

      virtual const char *getFilename() const=0;


      /**
       * Get the the creation date time as a Unix time_t
       * @return The creation date and time.
       */

      virtual time_t getCreationDateTime() const=0;


      /**
       * Get the last write date and time as a Unix time_t.
       * @return The last write date and time.
       */

      virtual time_t getLastWriteDateTime() const=0;


      /**
       * Get the last access date and time as a Unix time_t.
       * @return The last access date and time.
       */

      virtual time_t getLastAccessDateTime() const=0;


      /**
       * Get the file length up to a maximum of 4Gb.
       * @return THe file length.
       */

      virtual uint32_t getLength() const=0;
  };
}

