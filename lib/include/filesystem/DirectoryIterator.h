/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Class for iterating over the contents of a directory.
   *
   * Inherits from Iterator<const FileInformation&> and adds members for manipulating the directory
   */

  class DirectoryIterator : public Iterator<const FileInformation&> {

    public:

    /**
     * Error codes
     */
      enum {

        /// Entry is not a directory.
        E_NOT_A_DIRECTORY=1,

        /// Directory not found.
        E_DIRECTORY_NOT_FOUND,

        /// Directory entry not found
        E_ENTRY_NOT_FOUND
      };

      /**
       * Virtual destructor, does nothing.
       */

      virtual ~DirectoryIterator() {
      }

      /**
       * Get a new iterator on to the subdirectory pointed to by this iterator.
       * @param[out] newIterator The new iterator created by this call. Caller must delete when finished.
       */

      virtual bool getSubdirectoryIterator(DirectoryIterator *& newIterator)=0;


      /**
       * Check if we are pointing at the special "." entry that represents the current directory.
       *
       * @return true if this is the current directory.
       */

      virtual bool isCurrentDirectory()=0;

      /**
       * Check if we are pointing at the special ".." entry that represents the parent directory.
       *
       * @return true if this is the parent directory.
       */

      virtual bool isParentDirectory()=0;


      /**
       * Advance the iterator to the given filename (directory search)
       *
       * @param filename The filename to search for.
       * @return false if it fails.
       */

      virtual bool moveTo(const char *filename)=0;

      /**
       * Open the file pointed to by this iterator and return a high level File object for manipulating it.
       *
       * @param[out] newFile The new File object. Caller must deleted when finished.
       * @return false if it fails.
       */

      virtual bool openFile(File*& newFile)=0;
  };
}
