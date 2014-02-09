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
   * MBR definition
   */

  struct Mbr {

      /// Code area for boot loader.
      uint8_t code[440];

      /// Disk signature (optional).
      uint32_t diskSignature;

      /// reserved word
      uint16_t reserved1;

      /// table of primary partitions
      MbrPartition partitions[4];

      /// 0x55AA alternating bit pattern signature
      uint16_t signature;
#ifdef WIN32
    };
#else
  }__attribute__ ((packed));
#endif

  // MBR signature value

  static const uint16_t MBR_SIGNATURE=0xAA55;
}

#ifdef WIN32
#pragma pack(pop)
#endif
