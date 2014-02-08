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
     * @param[in] fs_ A reference to the fat file system class. Must stay in scope.
     * @param[in] firstClusterIndex_ The first cluster number of this directory.
     * @param[in] options_ The options to use while iterating.
     */

    NormalDirectoryEntryIterator::NormalDirectoryEntryIterator(FatFileSystem& fs_,uint32_t firstClusterIndex_,Options options_) :
      DirectoryEntryIterator(fs_,options_), _iterator(fs_,firstClusterIndex_,ClusterChainIterator::extensionDontExtend) {

      // force a move to the first sector in next()

      _firstClusterIndex=firstClusterIndex_;
      _indexWithinSector=0x10000;
      _currentDirentIndex=-1;
    }

    /*
     * reset the iterator
     */

    void NormalDirectoryEntryIterator::reset() {
      _iterator.reset(_firstClusterIndex);
      _indexWithinSector=0x10000;
      _currentDirentIndex=-1;
    }

    /*
     * Internal next()
     */

    bool NormalDirectoryEntryIterator::internalNext() {

      ByteMemblock sector(_fs.getSectorSizeInBytes());

      // check if need to move

      if(++_indexWithinSector >= _fs.getBootSector().BPB_BytsPerSec / sizeof(DirectoryEntry)) {
        if(!_iterator.next())
          return false;

        _indexWithinSector=0;
      }

      // copy the current entry out of the sector

      DirectoryEntry& dirent=_currentEntry.Dirent;

      if(!_iterator.readSector(sector))
        return false;

      memcpy(&dirent,sector + (_indexWithinSector * sizeof(DirectoryEntry)),sizeof(DirectoryEntry));
      _currentEntry.SectorNumber=_iterator.current();
      _currentEntry.IndexWithinSector=_indexWithinSector;

      // check for end-of-entries

      if((dirent.sdir.DIR_Attr & DirectoryEntry::ATTR_LONG_NAME_MASK) != DirectoryEntry::ATTR_LONG_NAME && dirent.sdir.DIR_Name[0] == '\0')
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_ITERATOR,E_END_OF_ENTRIES);

      // done

      _currentDirentIndex++;
      return true;
    }

    /*
     * Extend the directory to hold new entries
     */

    bool NormalDirectoryEntryIterator::extendDirectory(DirectoryEntry *dirents_,uint32_t direntCount_) {

      uint32_t sectors,i,entriesPerSector,indexInSector;
      ByteMemblock sector(_fs.getSectorSizeInBytes());
      void *dest;

      // get the number of entries in this directory

      reset();
      while(next())
        ;
      if(!errorProvider.isLastError(ErrorProvider::ERROR_PROVIDER_ITERATOR,Iterator<DirectoryEntryWithLocation>::E_END_OF_ENTRIES))
        return false;

      // get a file sector iterator that extends and move to the sector

      FileSectorIterator it(_fs,_firstClusterIndex,ClusterChainIterator::extensionExtend);

      entriesPerSector=_fs.getSectorSizeInBytes() / sizeof(DirectoryEntry);
      sectors=(_currentDirentIndex + 1) / entriesPerSector;
      indexInSector=(_currentDirentIndex + 1) % entriesPerSector;

      for(i=0;i <= sectors;i++)
        if(!it.next())
          return false;

      // read the current sector

      if(!it.readSector(sector))
        return false;

      // start writing

      for(i=0;i <= direntCount_;i++) {

        dest=sector.getData() + (sizeof(DirectoryEntry) * indexInSector);

        if(i == direntCount_)
          memset(dest,0,sizeof(DirectoryEntry)); // new end marker
        else
          memcpy(dest,&dirents_[i],sizeof(DirectoryEntry));

        if(++indexInSector == entriesPerSector) {

          // write the completed sector

          if(!it.writeSector(sector.getData()))
            return false;

          // advance to the next and read it

          if(!it.next() || !it.readSector(sector))
            return false;

          indexInSector=0;
        }
      }

      // write back if we've got some remaining

      if(indexInSector > 0 && !it.writeSector(sector.getData()))
        return false;

      // done

      return true;
    }
  }
}
