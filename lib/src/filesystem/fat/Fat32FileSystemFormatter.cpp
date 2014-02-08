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
     * @brief Initialise the class and do the format.
     *
     * Check the error provider status to find out whether the format worked. The format operation prepares the FAT data structures. It does not
     * do a low-level format of the device.
     *
     * @param[in] blockDevice The device to format.
     * @param[in] firstSectorIndex The zero-based index of the first sector of the partition, or the device as a whole if partitions are not supported.
     * @param[in] numSectors The total number of sectors to format on this device.
     * @param[in] volumeLabel The new name for the volume, max 11 characters.
     */

    Fat32FileSystemFormatter::Fat32FileSystemFormatter(BlockDevice& blockDevice,uint32_t firstSectorIndex,uint32_t numSectors,const char *volumeLabel)
      : FatFileSystemFormatter(blockDevice,firstSectorIndex,numSectors,volumeLabel) {

      uint32_t rootDirFirstSector,firstDataSector;

      // write the reserved sectors

      if(!writeReservedSectors())
        return;

      // the two FAT tables start at sector #6, write them

      if(!writeFats(_firstSectorIndex+_bootSector.BPB_RsvdSecCnt,_bootSector.fat32.BPB_FATSz32))
        return;

      // root dir sector is a little more troublesome than FAT16 since it's at cluster #2

      firstDataSector=_bootSector.BPB_RsvdSecCnt+(_bootSector.BPB_NumFATs*_bootSector.fat32.BPB_FATSz32);
      rootDirFirstSector=firstDataSector+((_bootSector.fat32.BPB_RootClus-2)*_bootSector.BPB_SecPerClus);

      // write the root directory entry

      writeRootDirectoryEntries(_firstSectorIndex+rootDirFirstSector);
    }


    /*
     * Prepare a new bootsector for FAT32
     */

    bool Fat32FileSystemFormatter::createNewBootSector() {

      uint32_t tmpVal1,tmpVal2;

      // call the base class

      if(!FatFileSystemFormatter::createNewBootSector())
        return false;

      // generate sectors per cluster from the MS specification document

      if(_numSectors <= 66600)            // 32.5Mb lower limit
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FAT32_FILESYSTEM_FORMATTER,E_INVALID_DISK_SIZE);

      if(_numSectors <= 532480)           // 260Mb
        _bootSector.BPB_SecPerClus=1;
      else if(_numSectors <= 16777216)    // 8Gb
        _bootSector.BPB_SecPerClus=8;
      else if(_numSectors <= 33554432)    // 16Gb
        _bootSector.BPB_SecPerClus=16;
      else if(_numSectors <= 67108864)    // 32Gb
        _bootSector.BPB_SecPerClus=32;
      else
        _bootSector.BPB_SecPerClus=64;    // > 32Gb

      _bootSector.BPB_RsvdSecCnt=32;
      _bootSector.BPB_RootEntCnt=0;
      _bootSector.BPB_TotSec16=0;
      _bootSector.BPB_FATSz16=0;
      _bootSector.BPB_TotSec32=_numSectors;

      // this part lifted from the MS specification

      tmpVal1=_numSectors - 32;
      tmpVal2=((256 * _bootSector.BPB_SecPerClus) + 2) / 2;

      _bootSector.fat32.BPB_FATSz32=(tmpVal1 + (tmpVal2 - 1)) / tmpVal2;

      _bootSector.fat32.BPB_ExtFlags=0;
      _bootSector.fat32.BPB_FSVer=0;
      _bootSector.fat32.BPB_RootClus=2;
      _bootSector.fat32.BPB_FSInfo=1;         // fsinfo sector in reserved area
      _bootSector.fat32.BPB_BkBootSec=6;      // backup boot sector in reserved area
      memset(_bootSector.fat32.BPB_Reserved,0,sizeof(_bootSector.fat32.BPB_Reserved));
      _bootSector.fat32.BS_DrvNum=0x80;
      _bootSector.fat32.BS_Reserved1=0;
      _bootSector.fat32.BS_BootSig=0x29;
      _bootSector.fat32.BS_VolID=0xBADF00D;
      memcpy(_bootSector.fat32.BS_VolLab,_volumeLabel,11);
      memcpy(_bootSector.fat32.BS_FilSysType,"FAT32   ",8);

      return true;
    }


    /*
     * Format the reserved sectors
     */

    bool Fat32FileSystemFormatter::writeReservedSectors() {

      ByteMemblock fsinfo(512);

      // boot sector and backup

      if(!writeBootSector(_firstSectorIndex) || !writeBootSector(_firstSectorIndex+6))
        return false;

      // fsinfo structure at reserved #1

      createNewFsInfo(*reinterpret_cast<Fat32FsInfo *>(fsinfo.getData()));
      return _blockDevice.writeBlock(fsinfo,1);
    }


    /*
     * Set up a new FsInfo structure
     */

    void Fat32FileSystemFormatter::createNewFsInfo(Fat32FsInfo& fsinfo) const {

      fsinfo.FSI_LeadSig=0x41615252;
      memset(fsinfo.FSI_Reserved1,0,sizeof(fsinfo.FSI_Reserved1));
      fsinfo.FSI_StrucSig=0x61417272;
      fsinfo.FSI_Free_Count=0xFFFFFFFF;
      fsinfo.FSI_Nxt_Free=0xFFFFFFFF;
      memset(fsinfo.FSI_Reserved2,0,sizeof(fsinfo.FSI_Reserved2));
      fsinfo.FSI_TrailSig=0xAA550000;
    }

    /*
     * initialise the two reserved clusters at the start of the FAT
     */

    void Fat32FileSystemFormatter::initReservedClusters(uint8_t *sector) const {

      uint32_t cluster,*fat;

      fat=reinterpret_cast<uint32_t *>(sector);
      cluster=0x0fffff00 | MEDIA_TYPE;

      fat[0]=cluster;
      fat[1]=0x0FFFFFFF;
      fat[2]=0x0FFFFFFF;        // EOC for the root directory at cluster 2
    }

  }
}
