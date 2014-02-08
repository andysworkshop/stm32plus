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
     * Constructor.
     * @param[in] fs_ The filesystem reference for this iterator.
     * @param[in] options_ Options that control how to iterator over this directory.
     */

    Fat16RootDirectoryEntryIterator::Fat16RootDirectoryEntryIterator(FatFileSystem& fs_,Options options_) :
      DirectoryEntryIterator(fs_,options_),
      _currentSector(fs_.getBootSector().BPB_BytsPerSec) {

      _entriesPerSector=_fs.getBootSector().BPB_BytsPerSec/sizeof(DirectoryEntry);
      _rootDirMaxEntries=_fs.getBootSector().BPB_RootEntCnt;

      reset();
    }

    /*
     * reset the iteration
     */

    void Fat16RootDirectoryEntryIterator::reset() {
      _currentIndex=-1;
      _firstEntryInCurrentSector=-1;
    }

    /*
     * Get the byte offset into the sector of this entry
     */

    uint32_t Fat16RootDirectoryEntryIterator::entryOffsetInSector(uint32_t entryIndex_) const {
      return (entryIndex_%_entriesPerSector)*sizeof(DirectoryEntry);
    }

    /*
     * Return true if the current entry is in the current sector.
     * Assumes _currentIndex!=-1 and _firstEntryInCurrentSector!=-1
     */

    bool Fat16RootDirectoryEntryIterator::isEntryInCurrentSector() const {
      return _currentIndex-_firstEntryInCurrentSector<_entriesPerSector;
    }

    /*
     * Get the sector index for the given entry
     */

    uint32_t Fat16RootDirectoryEntryIterator::sectorIndexForEntry(uint32_t entryIndex_) const {
      return _fs.getRootDirectoryFirstSector()+(entryIndex_/_entriesPerSector);
    }

    /*
     * Move to next entry
     */

    bool Fat16RootDirectoryEntryIterator::internalNext() {

      uint32_t offset;

      // update to next entry, or zero if this is the first move

      _currentIndex++;

      // check for the end of this fixed length structure

      if(_currentIndex==_rootDirMaxEntries)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_ITERATOR,E_END_OF_ENTRIES);

      // first time?

      if(_currentIndex==0||!isEntryInCurrentSector()) {

        // current index is not in the cached sector, read new sector

        if(!readSectorForEntry(_currentIndex))
          return false;
      }

      // get the offset into the sector and copy

      offset=entryOffsetInSector(_currentIndex);

      memcpy(&_currentEntry.Dirent,static_cast<uint8_t *> (_currentSector)+offset,sizeof(DirectoryEntry));
      _currentEntry.SectorNumber=sectorIndexForEntry(_currentIndex);
      _currentEntry.IndexWithinSector=_currentIndex%_entriesPerSector;

      // check for end-of-entries

      if((_currentEntry.Dirent.sdir.DIR_Attr&DirectoryEntry::ATTR_LONG_NAME_MASK)!=DirectoryEntry::ATTR_LONG_NAME&&_currentEntry.Dirent.sdir.DIR_Name[0]=='\0')
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_ITERATOR,E_END_OF_ENTRIES);

      return true;
    }

    /*
     * Read a sector for the given entry index
     */

    bool Fat16RootDirectoryEntryIterator::readSectorForEntry(uint32_t entryIndex_) {

      uint32_t sectorIndex;

      // check if reached the end

      if(entryIndex_/_entriesPerSector>=((Fat16FileSystem&)_fs).getRootDirSectors())
        return false;

      sectorIndex=sectorIndexForEntry(entryIndex_);
      return _fs.readSector(sectorIndex,_currentSector);
    }

    /*
     * 'extend' the directory to include space for new entries. The directory has a fixed number sectors
     *  so we cannot allocate more if the end is reached
     */

    bool Fat16RootDirectoryEntryIterator::extendDirectory(DirectoryEntry *dirents_,uint32_t direntCount_) {

      DirectoryEntryWithLocation dloc;
      uint32_t i;

      // go to the end

      reset();
      while(next())
        ;
      if(!errorProvider.isLastError(ErrorProvider::ERROR_PROVIDER_ITERATOR,Iterator<DirectoryEntryWithLocation>::E_END_OF_ENTRIES))
        return false;

      // have we got space?

      if(_currentIndex>_rootDirMaxEntries-direntCount_)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_ITERATOR,E_ROOT_DIRECTORY_FULL);

      for(i=0;i<=direntCount_;i++) {

        if(_currentIndex+i<_rootDirMaxEntries) { // may not be space for the end marker

          dloc.SectorNumber=sectorIndexForEntry(_currentIndex+i);
          dloc.IndexWithinSector=(_currentIndex+i)%_entriesPerSector;

          if(i==direntCount_)
            memset(&dloc.Dirent,0,sizeof(dloc.Dirent)); // end of entries marker
          else
            dloc.Dirent=dirents_[i]; // struct copy

          if(!_fs.writeDirectoryEntry(dloc))
            return false;
        }
      }

      return true;
    }
  }
}
