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
   * Get the current filename.
   * @return An internal null terminated pointer to the filename. Do not delete.
   */

    const char *FilenameHandler::getFilename() {
      return _longFilename==nullptr ? _shortFilename : _longFilename;
    }


  /**
   * Constructor. Set initial state.
   */

    FilenameHandler::FilenameHandler() {

      initialise();
    }


    /*
     * initialise
     */

    void FilenameHandler::initialise() {

      _longFilename=nullptr;
      _sectorIndices=nullptr;
      _sectorOffsetIndices=nullptr;

      _state=STATE_NONE;
    }


  /**
   * Destructor. Delete internal buffers.
   */

    FilenameHandler::~FilenameHandler() {
      cleanup();
    }


    /**
     * Cleanup and re-initialise for another run
     */

    void FilenameHandler::reinitialise() {
      cleanup();
      initialise();
    }


    /*
     * De-allocate
     */

    void FilenameHandler::cleanup() {
      
      if(_longFilename!=nullptr)
        delete [] _longFilename;

      if(_sectorIndices!=nullptr)
        delete [] _sectorIndices;

      if(_sectorOffsetIndices!=nullptr)
        delete [] _sectorOffsetIndices;
    }


  /**
   * Handle the next directory entry. The caller should repeatedly call this method until
   * a short directory entry is encountered. Since a long name immediately precedes its short equivalent
   * when the short name is hit then the long name will have already been processed here.
   *
   * @param[in] entry The directory entry to process.
   * @return false if there is a processing failure.
   */

    bool FilenameHandler::handleEntry(DirectoryEntryWithLocation& entry) {

      errorProvider.clear();

    // entry passed in here should already have been checked to ensure it's valid
    // and has not been deleted.

      if((entry.Dirent.sdir.DIR_Attr & DirectoryEntry::ATTR_LONG_NAME_MASK)==DirectoryEntry::ATTR_LONG_NAME)
        return handleLongName(entry);
      else
        return handleShortName(entry);
    }


  /*
   * Handle a short name
   */

    bool FilenameHandler::handleShortName(DirectoryEntryWithLocation& entry) {

      int i,j;
      DirectoryEntry& dirent=entry.Dirent;

      if(_state==STATE_LONG_FILENAME_IN_PROGRESS) {

      // we've hit the short version of a long filename, the countdown of long filename
      // entries must have hit 1 at this point

        if(_currentIndex!=1)
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FAT_DIRECTORY,E_CORRUPT_LONG_FILENAME,0);

        setSectorDetails(entry,0);
      }
      else {

      // old-style short name, this will fit in the short name buffer

        if(_longFilename!=nullptr) {
          delete [] _longFilename;
          _longFilename=nullptr;
        }

      // copy the first 8 characters, or up to the first space

        for(i=0;i<8 && dirent.sdir.DIR_Name[i]!=' ';i++)
          _shortFilename[i]=dirent.sdir.DIR_Name[i];

      // is there an extension? if so then take up to 3 characters

        if(dirent.sdir.DIR_Name[8]!=' ') {

          _shortFilename[i++]='.';

          for(j=8;j<11 && dirent.sdir.DIR_Name[j]!=' ';j++,i++)
            _shortFilename[i]=dirent.sdir.DIR_Name[j];
        }

        _shortFilename[i]='\0';

        setSectorDetails(entry,0,1);
      }

      _state=STATE_READY;
      return true;
    }


  /*
   * Handle a long name
   */

    bool FilenameHandler::handleLongName(DirectoryEntryWithLocation& entry) {

      char *dest,subcomponent[13];
      int subcomponentLength;
      const uint16_t *src;

      DirectoryEntry& dirent=entry.Dirent;

    // get the characters from the name, supporting only 8 bit characters

      dest=subcomponent;
      src=dirent.ldir.LDIR_Name1;

      for(subcomponentLength=0;subcomponentLength<13 && *src;subcomponentLength++) {

        *dest++=(char)*src++;

      // move to the next part of the name when we hit the end

        if(subcomponentLength==4)
          src=dirent.ldir.LDIR_Name2;
        else if(subcomponentLength==10)
          src=dirent.ldir.LDIR_Name3;
      }

      if((dirent.ldir.LDIR_Ord & DirectoryEntry::LAST_LONG_ENTRY)!=0) {

      // this is the start of the reverse sequence of name fragments
      // the state cannot be parsing an existing sequence

        if(_state==STATE_LONG_FILENAME_IN_PROGRESS)
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FAT_DIRECTORY,E_INVALID_STATE,0);

        _currentIndex=dirent.ldir.LDIR_Ord & ~DirectoryEntry::LAST_LONG_ENTRY;

      // name length is this block size + 13*the number of following blocks

        _length=subcomponentLength+(_currentIndex-1)*13;

      // free old filename if present

        if(_longFilename!=nullptr)
          delete [] _longFilename;

      // where are we storing?

        if(_length>=sizeof(_shortFilename)) {
          _longFilename=new char[_length+1];
          _longFilename[_length]='\0';
        }
        else {
          _longFilename=nullptr;
          _shortFilename[_length]='\0';
        }

      // create new array to hold sector details and store

        setSectorDetails(entry,_currentIndex,_currentIndex+1);
      }
      else {

      // check that we are in sequence

        if(_currentIndex!=dirent.ldir.LDIR_Ord+1)
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FAT_DIRECTORY,E_CORRUPT_LONG_FILENAME,0);

        _currentIndex=dirent.ldir.LDIR_Ord;
        setSectorDetails(entry,_currentIndex);
      }

    // copy the filename into position

      dest=_longFilename==nullptr ? _shortFilename : _longFilename;
      memcpy(dest+(_currentIndex-1)*13,subcomponent,subcomponentLength);

    // set the current state

      _state=STATE_LONG_FILENAME_IN_PROGRESS;
      return true;
    }


  /*
   * set the sector details that we store here. we store the locations of the dirents that make up this
   * [long]filename so that a caller can use them to delete the file.
   */

    void FilenameHandler::setSectorDetails(DirectoryEntryWithLocation& entry,uint32_t sectorIndex,uint32_t newSize) {
      
      if(newSize!=0) {
        _sectorIndices=new uint32_t[newSize];
        _sectorOffsetIndices=new uint32_t[newSize];
        _direntCount=newSize;
      }

      _sectorIndices[sectorIndex]=entry.SectorNumber;
      _sectorOffsetIndices[sectorIndex]=entry.IndexWithinSector;
    }


  /**
   * Get the sector indices array. This array holds the sector locations of each dirent that makes up the filename.
   * This can be used to efficiently delete the file if the caller needs to.
   *
   * @return An internal pointer to the sector indices array. Do not delete.
   */

    uint32_t *FilenameHandler::getSectorIndices() {
      return _sectorIndices;
    }
    
    
  /**
   * Get the sector offset indices array. Each entry in this array is paired with an entry in the getSectorIndices() array
   * to give the exact location in a sector of the directory entry. The sector index tells you where on disk the entry lies
   * and the sector offset tells you where in the sector the entry lies. The entries in this array are byte offsets into the
   * sectors.
   *
   * @return An internal pointer to the sector offset indices array. Do not delete.
   */

    uint32_t *FilenameHandler::getSectorOffsetIndices() {
      return _sectorOffsetIndices;
    }


  /**
   * Get the number of dirents holding this filename.
   * @return The number of dirents, which corresponds to the number of entries in the getSectorIndices() and getSectorOffsetIndices()
   * arrays.
   */

    uint32_t FilenameHandler::getDirentCount() {
      return _direntCount;
    }

  }
}
