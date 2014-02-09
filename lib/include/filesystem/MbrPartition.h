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

  /**
   * MBR partition definition
   */

  struct MbrPartition {

      /// status (0x80 = bootable (active), 0x00 = non-bootable, other = invalid )
      uint8_t status;

      /// CHS address of first absolute sector in partition. The format is described by 3 bytes
      uint8_t chsFirstSector[3];

      /// Partition type (see documentation on web)
      uint8_t partitionType;

      /// CHS address of last sector in partition.
      uint8_t chsLastSector[3];

      /// LBA of first absolute sector in the partition.
      uint32_t lbaFirstSector;

      /// Number of sectors in partition, in little-endian format
      uint32_t numSectors;
#ifdef WIN32
    };
#else
  }__attribute__ ((packed));
#endif
}

#ifdef WIN32
#pragma pack(pop)
#endif
