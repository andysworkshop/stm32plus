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
     * Constructor - open the directory pointed to by dirent
     */

    FatDirectoryIterator::FatDirectoryIterator(FatFileSystem& fs,DirectoryEntryWithLocation& direntWithLocation) :
      _fs(fs) {

      DirectoryEntry& dirent=direntWithLocation.Dirent;

      _entryIterator=new NormalDirectoryEntryIterator(fs,(uint32_t)dirent.sdir.DIR_FstClusLO | ((uint32_t)dirent.sdir.DIR_FstClusHI) << 16,DirectoryEntryIterator::OPT_DEFAULT_REAL_ENTRIES);
    }

    /*
     * Constructor over the root directory
     */

    FatDirectoryIterator::FatDirectoryIterator(FatFileSystem& fs) :
      _fs(fs) {

      _entryIterator=_fs.getRootDirectoryIterator(DirectoryEntryIterator::OPT_DEFAULT_REAL_ENTRIES);
    }

    /**
     * Virtual destructor, clean up internal iterator.
     */

    FatDirectoryIterator::~FatDirectoryIterator() {

      if(_entryIterator != nullptr)
        delete _entryIterator;
    }

    /**
     * Get the directory entry iterator
     * @return The internal directory entry iterator.
     */

    DirectoryEntryIterator& FatDirectoryIterator::getDirectoryEntryIterator() {
      return *_entryIterator;
    }

    /**
     * @copydoc Iterator::current
     */

    const FileInformation& FatDirectoryIterator::current() {
      return *this;
    }

    /**
     * @copydoc FileInformation::getFilename
     */

    const char *FatDirectoryIterator::getFilename() const {
      return _entryIterator->getFilename();
    }

    /**
     * @copydoc FileInformation::getAttributes
     */

    uint32_t FatDirectoryIterator::getAttributes() const {
      return _entryIterator->current().Dirent.sdir.DIR_Attr;
    }

    /**
     *@copydoc FileInformation::getCreationDateTime
     */

    time_t FatDirectoryIterator::getCreationDateTime() const {
      return _entryIterator->getCreationDateTime();
    }

    /**
     * @copydoc FileInformation::getLastWriteDateTime
     */

    time_t FatDirectoryIterator::getLastWriteDateTime() const {
      return _entryIterator->getLastWriteDateTime();
    }

    /**
     * @copydoc FileInformation::getLastAccessDateTime
     */

    time_t FatDirectoryIterator::getLastAccessDateTime() const {
      return _entryIterator->getLastAccessDateTime();
    }

    /**
     * @copydoc DirectoryIterator::isCurrentDirectory
     */

    bool FatDirectoryIterator::isCurrentDirectory() {
      return !strcmp(_entryIterator->getFilename(),"."); // only dirs can have this name
    }

    /**
     * @copydoc DirectoryIterator::isParentDirectory
     */

    bool FatDirectoryIterator::isParentDirectory() {
      return !strcmp(_entryIterator->getFilename(),".."); // only dirs can have this name
    }

    /**
     * @copydoc FileInformation::getLength
     */

    uint32_t FatDirectoryIterator::getLength() const {
      return _entryIterator->current().Dirent.sdir.DIR_FileSize;
    }

    /**
     * @copydoc DirectoryIterator::getSubdirectoryIterator
     */

    bool FatDirectoryIterator::getSubdirectoryIterator(DirectoryIterator*& newIterator) {

      // ensure that this is a directory we are currently looking at

      if((_entryIterator->current().Dirent.sdir.DIR_Attr & DirectoryEntry::ATTR_DIRECTORY) == 0)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_DIRECTORY_ITERATOR,E_NOT_A_DIRECTORY);

      // open it

      newIterator=new FatDirectoryIterator(_fs,_entryIterator->current());
      return true;
    }

    /**
     * @copydoc DirectoryIterator::moveTo
     */

    bool FatDirectoryIterator::moveTo(const char *filename) {

      while(next()) {

        if(!strcasecmp(getFilename(),filename))
          return true;
      }

      // not found

      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_DIRECTORY_ITERATOR,E_ENTRY_NOT_FOUND);
    }

    /**
     * @copydoc DirectoryIterator::openFile
     */

    bool FatDirectoryIterator::openFile(File*& newFile) {

      newFile=new FatFile(_fs,_entryIterator->current());

      return true;
    }

    /**
     * @copydoc Iterator::next
     */

    bool FatDirectoryIterator::next() {

      // move to next

      return _entryIterator->next();
    }

    /**
     * Create an instance of the directory iterator suitable for the given directory.
     *
     * @param[in] fs Reference to the filesystem class.
     * @param[in] tp The tokenised pathname that points to this directory.
     * @param[out] newIterator The directory iterator created by this call if it succeeds. Caller must delete this pointer when finished.
     * @return false if it fails.
     */

    bool FatDirectoryIterator::getInstance(FatFileSystem& fs,const TokenisedPathname& tp,FatDirectoryIterator *& newIterator) {

      int i;
      DirectoryEntryIterator *it,*tempIterator;

      // check for root directory

      if(tp.getNumTokens()==0) {

        // create a new iterator over the root directory

        newIterator=new FatDirectoryIterator(fs);
        return true;
      }

      // iterate for every component of the name

      it=nullptr;

      for(i=0;i<tp.getNumTokens();i++) {

        if(it==nullptr) {

          // create a new iterator over the root directory

          it=fs.getRootDirectoryIterator(DirectoryEntryIterator::OPT_DEFAULT_REAL_ENTRIES);
        } else {

          // create a new iterator over the subdirectory we just found

          DirectoryEntry& dirent=it->current().Dirent;

          tempIterator=new NormalDirectoryEntryIterator(fs,dirent.sdir.DIR_FstClusLO | (((uint32_t)dirent.sdir.DIR_FstClusHI) << 16),DirectoryEntryIterator::OPT_DEFAULT_REAL_ENTRIES);
          delete it;
          it=tempIterator;
        }

        // get next entry

        do {
          if(!it->next())
            return errorProvider.set(ErrorProvider::ERROR_PROVIDER_DIRECTORY_ITERATOR,E_DIRECTORY_NOT_FOUND);

          // test the filename in the entry against the component

        } while(strcasecmp(it->getFilename(),tp[i]) != 0);
      }

      // the final entry must be a directory

      if(!it->current().isDirectory())
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_DIRECTORY_ITERATOR,E_NOT_A_DIRECTORY);

      newIterator=new FatDirectoryIterator(fs,it->current());

      delete it;
      return true;
    }

    /**
     * Get the DirectoryEntryWithLocation structure for this entry.
     * @return The directory entry structure.
     */

    DirectoryEntryWithLocation& FatDirectoryIterator::getDirectoryEntryWithLocation() {
      return _entryIterator->current();
    }

  }
}
