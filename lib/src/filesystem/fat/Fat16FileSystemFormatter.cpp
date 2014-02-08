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

    Fat16FileSystemFormatter::Fat16FileSystemFormatter(BlockDevice& blockDevice,uint32_t firstSectorIndex,uint32_t numSectors,const char *volumeLabel)
      : FatFileSystemFormatter(blockDevice,firstSectorIndex,numSectors,volumeLabel) {

      // write the reserved sectors

      if(!writeReservedSectors())
        return;

      // the two FAT tables start at sector #1, write them

      if(!writeFats(_firstSectorIndex+_bootSector.BPB_RsvdSecCnt,_bootSector.BPB_FATSz16))
        return;

      // write the root directory entry, which comes right after the two FATs

      writeRootDirectoryEntries(_firstSectorIndex+_bootSector.BPB_RsvdSecCnt+(_bootSector.BPB_FATSz16*2));
    }


    /*
     * Prepare a new bootsector for FAT16
     */

    bool Fat16FileSystemFormatter::createNewBootSector() {

      uint32_t tmpVal1,tmpVal2,fatSize;

      // call the base class

      if(!FatFileSystemFormatter::createNewBootSector())
        return false;

      // generate sectors per cluster from the MS specification document

      if(_numSectors <= 8400 || _numSectors > 4194304) // 4.1Mb lower limit, 2Gb upper limit
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FAT16_FILESYSTEM_FORMATTER,E_INVALID_DISK_SIZE);

      if(_numSectors <= 32680)            // 16Mb
        _bootSector.BPB_SecPerClus=2;
      else if(_numSectors <= 262144)      // 128Mb
        _bootSector.BPB_SecPerClus=4;
      else if(_numSectors <= 524288)      // 256Mb
        _bootSector.BPB_SecPerClus=8;
      else if(_numSectors <= 1048576)     // 512Mb
        _bootSector.BPB_SecPerClus=16;
      else if(_numSectors <= 2097152)     // 1Gb
        _bootSector.BPB_SecPerClus=32;
      else
        _bootSector.BPB_SecPerClus=64;    // 2Gb

      _bootSector.BPB_RsvdSecCnt=1;
      _bootSector.BPB_RootEntCnt=512;

      if(_numSectors < 0x10000) {
        _bootSector.BPB_TotSec16=_numSectors;
        _bootSector.BPB_TotSec32=0;
      } else {
        _bootSector.BPB_TotSec16=0;
        _bootSector.BPB_TotSec32=_numSectors;
      }

      // this part lifted from the MS document

      tmpVal1=_numSectors - 33;                           // DskSize – (BPB_ResvdSecCnt + RootDirSectors);
      tmpVal2=(256 * _bootSector.BPB_SecPerClus) + 2;     // (256 * BPB_SecPerClus) + BPB_NumFATs

      fatSize=(tmpVal1 + (tmpVal2 - 1)) / tmpVal2;
      _bootSector.BPB_FATSz16=fatSize & 0xFFFF;

      _bootSector.fat16.BS_DrvNum=0x80;
      _bootSector.fat16.BS_Reserved1=0;
      _bootSector.fat16.BS_BootSig=0x29;
      _bootSector.fat16.BS_VolID=0xBADF00D;
      memcpy(_bootSector.fat16.BS_VolLab,_volumeLabel,11);
      memcpy(_bootSector.fat16.BS_FilSysType,"FAT16   ",8);

      return true;
    }

    /*
     * format the reserved sectors on the device
     */

    bool Fat16FileSystemFormatter::writeReservedSectors() {

      // FAT16 only has the boot sector and nothing else

      return writeBootSector(_firstSectorIndex);
    }


    /*
     * initialise the two reserved clusters at the start of the FAT
     */

    void Fat16FileSystemFormatter::initReservedClusters(uint8_t *sector_) const {

      uint16_t cluster,*fat;

      fat=reinterpret_cast<uint16_t *>(sector_);
      cluster=0xff00 | MEDIA_TYPE;

      fat[0]=cluster;
      fat[1]=0xFFFF;        // EOC
    }
  }
}
