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
   * @brief FAT short directory entry
   *
   * This packed structure maps directly on to the 32 byte FAT directory entry. Full documentation
   * for this structure can be found by searching the net.
   */

    struct ShortDirectoryEntry {

      /// 8.3 name
      uint8_t DIR_Name[11];

      /// file/directory attributes
      uint8_t DIR_Attr;

      /// reserved
      uint8_t DIR_NTRes;

      /// creation time tenths of a second
      uint8_t DIR_CrtTimeTenth;

      /// creation time
      uint16_t DIR_CrtTime;

      /// creation date
      uint16_t DIR_CrtDate;

      /// last access date
      uint16_t DIR_LstAccDate;

      /// first cluster high 16 bits
      uint16_t DIR_FstClusHI;

      /// last write time
      uint16_t DIR_WrtTime;

      /// last write date
      uint16_t DIR_WrtDate;

      /// first cluster low 16 bits
      uint16_t DIR_FstClusLO;

      /// if this is a file then this is the byte size up to 4Gb.
      uint32_t DIR_FileSize;
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
