/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief File system base class.
   *
   * All file systems that derive from this use the same Unix style pathnames for addressing files. That is,
   * forward slashes as separators with / as the root. e.g. "/FileInRootDirectory.txt" or "/subdir/My Documents".
   * None of the filesystems support multibyte characters as filenames.
   */

  class FileSystem {

    protected:
      uint32_t _firstSectorIndex;
      uint32_t _sectorsPerBlock;
      BlockDevice& _blockDevice;
      const TimeProvider& _timeProvider;

    protected:
      FileSystem(BlockDevice& blockDevice,const TimeProvider& timeProvider,uint32_t firstSectorIndex);

      uint32_t sectorIndexToBlockIndex(uint32_t sectorIndex) const; // Convert a sector index to a block index
      uint32_t sectorOffsetIntoBlock(uint32_t sectorIndex) const; // Get the byte offset into a block of the sector

    public:

      /**
       * Error codes
       */
      enum {
        /// Filesystem type is not known
        E_UNKNOWN_FILESYSTEM=1,

        /// Pathname is invalid
        E_INVALID_PATHNAME,

        /// Device block size and filesystem sector size are not the same
        E_UNEQUAL_BLOCK_SECTOR_SIZES,

        /// File already exists
        E_FILE_EXISTS,

        /// invalid block size
        E_INVALID_BLOCK_SIZE
      };


      /**
       * Possible file system types
       */

      enum FileSystemType {
        /// FAT16 filesystem
        Fat16,

        /// FAT32 filesystem
        Fat32
      };

    public:
      static bool getInstance(BlockDevice& blockDevice,const TimeProvider& timeProvider,FileSystem*& newFileSystem);

      virtual ~FileSystem();

      const TimeProvider& getTimeProvider() const;

      virtual bool readSector(uint32_t sectorIndex,void *buffer);
      virtual bool writeSector(uint32_t sectorIndex,void *buffer);

      /**
       * Get the first sector index
       * @return the index of the first sector on the device
       */

      uint32_t getFirstSector() const {
        return _firstSectorIndex;
      }

      /**
       * Get the type of the file system from the enumeration.
       * @return The file system type.
       */

      virtual FileSystemType getFileSystemType() const=0;


      /**
       * Get file information for the given pathname.
       *
       * @param[in] filename The full pathname of the file to get information for.
       * @param[out] finfo The information for this file. Caller must delete when finished.
       * @return false if it fails.
       */

      virtual bool getFileInformation(const char *filename,FileInformation*& finfo)=0;


      /**
       * Create a file at the given pathname. The file must not already exist. The newly created
       * file is available for opening afterwards and will have zero length.
       *
       * @param[in] filename The filename to create.
       * @return false if it fails.
       */

      virtual bool createFile(const char *filename)=0;


      /**
       * Create a new directory at the given pathname.
       * @param[in] dirname The new directory name.
       * @return false if it fails.
       */

      virtual bool createDirectory(const char *dirname)=0;


      /**
       * Open the file at the given pathname.
       * @param[in] filename The full pathname to an existing file.
       * @param[out] newFile The new File object that the caller can use. Caller must delete when finished.
       * @return false if it fails.
       */

      virtual bool openFile(const char *filename,File*& newFile)=0;


      /**
       * Delete the file at the given pathname.
       * @param[in] filename The filename to delete.
       * @return false if it fails.
       */

      virtual bool deleteFile(const char *filename)=0;


      /**
       * Delete the directory with the given name. The directory must be empty.
       * @param[in] dirname The directory to delete.
       * @return false if it fails.
       */

      virtual bool deleteDirectory(const char *dirname)=0;

      /**
       * Get the size of a sector on this file system. Usually 512.
       * @return The sector size.
       */

      virtual uint32_t getSectorSizeInBytes() const=0;

      /**
       * Get a directory iterator on to the given directory.
       * @param[in] pathname The pathname to the directory to open.
       * @param[out] newIterator A new iterator for the caller to use. Caller must delete when finished.
       * @return false if it fails.
       */

      virtual bool getDirectoryIterator(const char *pathname,DirectoryIterator*& newIterator)=0;


      /**
       * Get the free space on the device as a two part result. The free space in bytes
       * Can be found by multiplying the two results together.
       * @param[out] freeUnits The number of free units.
       * @param[out] unitsMultiplier What to multiply freeUnits by to get a byte size.
       * @return false if it fails.
       */

      virtual bool getFreeSpace(uint32_t& freeUnits,uint32_t& unitsMultiplier)=0;
  };
}
