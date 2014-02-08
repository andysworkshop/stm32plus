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
     * Constructor
     */

    FatFileSystemFormatter::FatFileSystemFormatter(BlockDevice& blockDevice_,uint32_t firstSectorIndex_,uint32_t numSectors_,const char *volumeLabel_)
      : _blockDevice(blockDevice_) {

      int i;

      errorProvider.clear();

      // copy up to 11 characters from the volume label

      memset(_volumeLabel,' ',sizeof(_volumeLabel));
      for(i=0;i<11 && volumeLabel_[i];i++)
        _volumeLabel[i]=volumeLabel_[i];

      _numSectors=numSectors_;
      _firstSectorIndex=firstSectorIndex_;
    }


    /*
     * Prepare the parts of the bootsector that are FAT independent
     */

    bool FatFileSystemFormatter::createNewBootSector() {

      _bootSector.BS_jmpBoot[0]=0xeb;
      _bootSector.BS_jmpBoot[1]=0x58;
      _bootSector.BS_jmpBoot[2]=0x90;

      memcpy(_bootSector.BS_OEMName,"MSDOS5.0",8);

      _bootSector.BPB_BytsPerSec=512;

      _bootSector.BPB_NumFATs=2;
      _bootSector.BPB_Media=MEDIA_TYPE; // media type
      _bootSector.BPB_SecPerTrk=63;
      _bootSector.BPB_NumHeads=255;
      _bootSector.BPB_HiddSec=_firstSectorIndex;

      return true;
    }

    /*
     * Format a boot sector and write it to disk
     */

    bool FatFileSystemFormatter::writeBootSector(uint32_t bootSectorIndex_) {

      ByteMemblock sector(512);

    // create a new boot sector at the start of the block

      if(!createNewBootSector())
        return false;

      memcpy(sector.getData(),&_bootSector,sizeof(_bootSector));

    // write signature to sector

      setReservedSectorSignature(sector);

    // write to the device

      return _blockDevice.writeBlock(sector,bootSectorIndex_);
    }


    /*
     * Add the signature 0x55,0xAA
     */

    void FatFileSystemFormatter::setReservedSectorSignature(uint8_t *sector_) const {

      sector_[0x1fe]=0x55;
      sector_[0x1ff]=0xaa;
    }


    /*
     * Format the FAT structures
     */

    bool FatFileSystemFormatter::writeFats(uint32_t firstFatFirstSector_,uint32_t sectorsPerFat_) const {

      ByteMemblock sector(512);
      uint32_t i,j,sectorIndex;

      // two copies of the FAT, back to back

      sectorIndex=firstFatFirstSector_;

      for(i=0;i<2;i++) {

        // initialise the two reserved clusters

        memset(sector,0,512);
        initReservedClusters(sector);

        // write this sector
        if(!_blockDevice.writeBlock(sector,sectorIndex++))
          return false;

        // write the remaining sectors

        memset(sector,0,512);

        for(j=0;j<sectorsPerFat_;j++)
          if(!_blockDevice.writeBlock(sector,sectorIndex++))
            return false;
      }

      // done

      return true;
    }


    /*
     * Initialise the volume label
     */

    void FatFileSystemFormatter::initVolumeLabelDirent(uint8_t *rootDirSector_) const {

      ShortDirectoryEntry *dirent;

      dirent=reinterpret_cast<ShortDirectoryEntry *>(rootDirSector_);

      memset(dirent,0,32);
      memcpy(dirent->DIR_Name,_volumeLabel,11);
      dirent->DIR_Attr=DirectoryEntry::ATTR_VOLUME_ID;
    }


    /*
     * Initialise and write the root directory entries
     */

    bool FatFileSystemFormatter::writeRootDirectoryEntries(uint32_t sectorIndex_) const {

      ByteMemblock sector(512);

      // zero the sector and then write the volume label into it

      memset(sector.getData(),0,512);
      initVolumeLabelDirent(sector);

      // write to the device

      return _blockDevice.writeBlock(sector,sectorIndex_);
    }
  }
}
