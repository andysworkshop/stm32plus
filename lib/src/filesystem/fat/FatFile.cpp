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
     * @param[in] fs_ A reference to the FAT filesystem class.
     * @param[in] dirent_ The directory entry that points to this file.
     */

    FatFile::FatFile(FatFileSystem& fs_,DirectoryEntryWithLocation& dirent_) :
      _fs(fs_),
      _sectorBuffer(_fs.getSectorSizeInBytes()),
      _iterator(fs_,
                (static_cast<uint32_t> (dirent_.Dirent.sdir.DIR_FstClusHI) << 16) | dirent_.Dirent.sdir.DIR_FstClusLO,
                ClusterChainIterator::extensionExtend) {

      _dirent=dirent_; // struct copy
    }


    /*
     * @copydoc File::read
     */

    bool FatFile::read(void *ptr_,uint32_t size_,uint32_t& actuallyRead_) {

      uint32_t sectorSize=_fs.getSectorSizeInBytes();
      uint32_t fileLength,sectorOffset,copySize,available,remainingInFile;
      uint8_t *current;

      fileLength=getLength();

      // early fail for zero length file

      if(fileLength == 0)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILE,E_END_OF_FILE);

      // offset into first sector is probably non-zero

      sectorOffset=_offset % sectorSize;

      // continue while we need data

      current=static_cast<uint8_t *> (ptr_);
      actuallyRead_=0;

      while(size_ > 0) {

        // check for EOF, return true: actuallyRead will confirm the data actually retrieved

        if(_offset == fileLength)
          return true;

        // are we on a new sector?

        if(_offset % sectorSize == 0 && !_iterator.next())
          return false;

        // read a sector

        if(!_iterator.readSector(_sectorBuffer))
          return false;

        // calculate the copy size

        remainingInFile=fileLength - _offset;
        available=remainingInFile < sectorSize - sectorOffset ? remainingInFile : sectorSize - sectorOffset;
        copySize=size_ < available ? size_ : available;

        // copy out

        memcpy(current,_sectorBuffer + sectorOffset,copySize);

        size_-=copySize;
        current+=copySize;

        // update the position

        _offset+=copySize;
        actuallyRead_+=copySize;

        // we are now definitely on a sector boundary

        sectorOffset=0;
      }

      return true;
    }

    /**
     * @copydoc File::write
     */

    bool FatFile::write(const void *ptr_,uint32_t size_) {

      uint16_t d,t;
      const uint8_t *current=static_cast<const uint8_t *> (ptr_);
      DirectoryEntry& dirent=_dirent.Dirent;
      uint32_t sectorOffset,amountToCopy,sectorSize=_fs.getSectorSizeInBytes();

      // need to get the file pointer on to a sector boundary

      if(_offset % sectorSize > 0) {

        // there has to be a valid sector in the iterator
        // calculate the offset for the new data and the amount to copy in

        sectorOffset=_offset % sectorSize;
        amountToCopy=sectorSize - sectorOffset;

        if(amountToCopy > size_)
          amountToCopy=size_;

        // read the partially filled sector

        if(!_iterator.readSector(_sectorBuffer))
          return false;

        // copy in our data

        memcpy(_sectorBuffer + sectorOffset,current,amountToCopy);

        // write it back

        if(!_iterator.writeSector(_sectorBuffer))
          return false;

        // update pointers

        current+=amountToCopy;
        size_-=amountToCopy;
        _offset+=amountToCopy;

        if(_offset > dirent.sdir.DIR_FileSize)
          dirent.sdir.DIR_FileSize=_offset;
      }

      // we're on a sector boundary, loop until out of data

      while(size_ > 0) {

        // move to the next sector if we're on the end

        if(_offset % sectorSize == 0 && !_iterator.next())
          return false;

        // if this was a zero length file getting data for the first time then
        // update the dirent for it

        if(getLength() == 0) {
          dirent.sdir.DIR_FstClusLO=_iterator.getClusterNumber() & 0xFFFF;
          dirent.sdir.DIR_FstClusHI=_iterator.getClusterNumber() >> 16;
        }

        if(size_ < sectorSize && getLength() != _offset) {

          // must be the last part to write, and we are not at the end of the file
          // therefore we need to merge what's left to write with the existing content

          if(!_iterator.readSector(_sectorBuffer))
            return false;

          amountToCopy=size_;
        } else {
          amountToCopy=sectorSize; // we will overwrite the whole sector...

          if(amountToCopy > size_) // ...except when we don't have enough to do that
            amountToCopy=size_;
        }

        memcpy(_sectorBuffer,current,amountToCopy);

        // write the sector full of data

        if(!_iterator.writeSector(_sectorBuffer))
          return false;

        // update pointers

        current+=amountToCopy;
        size_-=amountToCopy;
        _offset+=amountToCopy;

        if(_offset > dirent.sdir.DIR_FileSize)
          dirent.sdir.DIR_FileSize=_offset;
      }

      // set the last modified time
      // gcc will not allow us to make a reference to a field in a packed struct :-;

      DirectoryEntryIterator::calculateFatDateTime(_fs.getTimeProvider().getTime(),d,t);
      dirent.sdir.DIR_WrtDate=d;
      dirent.sdir.DIR_WrtTime=t;

      // write back the modified dirent

      return _fs.writeDirectoryEntry(_dirent);
    }

    /**
     * @copydoc File::seek
     */

    bool FatFile::seek(int32_t offset_,SeekFrom origin_) {

      uint32_t newOffset;
      uint32_t sectorCount;
      DirectoryEntry& dirent=_dirent.Dirent;

      // get the new origin

      switch(origin_) {

        case SeekCurrent:
          newOffset=_offset + offset_;
          break;

        case SeekEnd:
          newOffset=dirent.sdir.DIR_FileSize - 1 + offset_;
          break;

        case SeekStart:
        default:
          newOffset=offset_;
          break;
      }

      // validate (one past the end is a valid "position")

      if(newOffset > dirent.sdir.DIR_FileSize)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILE,E_INVALID_FILE_POSITION);

      // translate to sectors

      sectorCount=(newOffset / _fs.getSectorSizeInBytes());
      if(newOffset % _fs.getSectorSizeInBytes() > 0)
        sectorCount++;

      _iterator.reset((static_cast<uint32_t> (dirent.sdir.DIR_FstClusHI) << 16) | dirent.sdir.DIR_FstClusLO);

      while(sectorCount--)
        if(!_iterator.next())
          return false;

      _offset=newOffset;
      return true;
    }

    /**
     * @copydoc File::getLength
     */

    uint32_t FatFile::getLength() {
      return _dirent.Dirent.sdir.DIR_FileSize;
    }

    /**
     * Get the dirent.
     * @return The directory entry of this file.
     */

    const DirectoryEntryWithLocation& FatFile::getDirectoryEntryWithLocation() {
      return _dirent;
    }
  }
}
