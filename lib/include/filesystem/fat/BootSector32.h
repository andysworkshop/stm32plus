/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#ifdef WIN32
#pragma pack(push,1)
#endif

namespace stm32plus {
  namespace fat {


  /**
   * @brief Fat32 specific part of the boot sector
   */

    struct BootSector32 {

      /// This field is the FAT32 32-bit count of sectors occupied by ONE FAT. BPB_FATSz16 must be 0.
      uint32_t BPB_FATSz32;

      /// This field is only defined for FAT32 media and does not exist on FAT12 and FAT16 media.
      uint16_t BPB_ExtFlags;

      /// High byte is major revision number. Low byte is minor revision number. This is the version number of the FAT32 volume. This supports the ability to extend the FAT32 media type in the future without worrying about old FAT32 drivers mounting the volume. This document defines the version to 0:0.  If this field is non-zero, back-level Windows versions will not mount the volume.
      uint16_t BPB_FSVer;

      /// This is set to the cluster number of the first cluster of the root directory, usually 2 but not required to be 2.
      uint32_t BPB_RootClus;

      /// Sector number of FSINFO structure in the reserved area of the FAT32 volume. Usually 1.
      uint16_t BPB_FSInfo;

      /// If non-zero, indicates the sector number in the reserved area of the volume of a copy of the boot record. Usually 6. No value other than 6 is recommended.
      uint16_t BPB_BkBootSec;

      /// Reserved for future expansion. Code that formats FAT32 volumes should always set all of the bytes of this field to 0.
      uint8_t  BPB_Reserved[12];

      /// This field has the same definition as it does for FAT12 and FAT16 media. The only difference for FAT32 media is that the field is at a different offset in the boot sector.
      uint8_t  BS_DrvNum;

      /// This field has the same definition as it does for FAT12 and FAT16 media. The only difference for FAT32 media is that the field is at a different offset in the boot sector.
      uint8_t  BS_Reserved1;

      /// This field has the same definition as it does for FAT12 and FAT16 media. The only difference for FAT32 media is that the field is at a different offset in the boot sector.
      uint8_t  BS_BootSig;

      /// This field has the same definition as it does for FAT12 and FAT16 media. The only difference for FAT32 media is that the field is at a different offset in the boot sector.
      uint32_t BS_VolID;

      /// This field has the same definition as it does for FAT12 and FAT16 media. The only difference for FAT32 media is that the field is at a different offset in the boot sector.
      char     BS_VolLab[11];

      /// Always set to the string "FAT32   ".  Please see the note for this field in the FAT12/FAT16 section earlier. This field has nothing to do with FAT type determination.
      char     BS_FilSysType[8];
#ifdef WIN32
    };
#else
  } __attribute__ ((packed));
#endif
  }
}

#ifdef WIN32
#pragma pack(pop)
#endif
