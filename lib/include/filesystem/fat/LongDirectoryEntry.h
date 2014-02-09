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
   * @brief Long name FAT directory entry.
   *
   * This packed structure holds part of a long name FAT structure. Search the net for more details.
   */

    struct LongDirectoryEntry {

      /// ordinal number of this entry
      uint8_t  LDIR_Ord;

      /// first part of the name
      uint16_t LDIR_Name1[5];

      /// attributes
      uint8_t  LDIR_Attr;

      /// type
      uint8_t  LDIR_Type;

      /// checksum
      uint8_t  LDIR_Chksum;

      /// second part of the name
      uint16_t LDIR_Name2[6];

      /// first cluster low 16-bits
      uint16_t LDIR_FstClusLO;

      /// third part of the name
      uint16_t LDIR_Name3[2];
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
