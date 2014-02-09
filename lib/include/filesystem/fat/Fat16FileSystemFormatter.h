/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Implementation of a FAT formatter for FAT16 file systems
     */

    class Fat16FileSystemFormatter : public FatFileSystemFormatter {

      protected:
        virtual bool createNewBootSector() override;
        virtual bool writeReservedSectors() override;
        virtual void initReservedClusters(uint8_t *sector) const override;

      public:
        Fat16FileSystemFormatter(BlockDevice& blockDevice,uint32_t firstSectorIndex,uint32_t numSectors,const char *volumeLabel);
    };
  }
}
