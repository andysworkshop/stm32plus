/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * Fat filesystem formatter base class
     */

    class FatFileSystemFormatter {

      protected:
        char _volumeLabel[11];
        BlockDevice& _blockDevice;
        uint32_t _numSectors;
        uint32_t _firstSectorIndex;
        BootSector _bootSector;

      protected:
        FatFileSystemFormatter(BlockDevice& blockDevice,uint32_t firstSectorIndex,uint32_t numSectors,const char *volumeLabel);
        virtual ~FatFileSystemFormatter() {}

        virtual bool createNewBootSector();

        void setReservedSectorSignature(uint8_t *sector) const;
        void initVolumeLabelDirent(uint8_t *rootDirSector) const;
        bool writeBootSector(uint32_t bootSectorIndex);
        bool writeFats(uint32_t firstFatFirstSector,uint32_t sectorsPerFat) const;
        bool writeRootDirectoryEntries(uint32_t sectorIndex) const;

        virtual bool writeReservedSectors()=0;
        virtual void initReservedClusters(uint8_t *sector) const=0;

      protected:
        static const uint8_t MEDIA_TYPE=0xf8;         // fixed media, which this may not be.

      public:

        enum {
          /// tried to format a disk whose size is out of range for the format.
          E_INVALID_DISK_SIZE=1
        };

    };
  }
}


