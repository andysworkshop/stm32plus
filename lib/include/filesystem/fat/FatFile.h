/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    class FatFileSystem;

    /**
     * @brief Fat file extends the basic File class.
     *
     * FAT implementation of the File base class.
     */

    class FatFile : public File {
      protected:
        FatFileSystem& _fs;
        DirectoryEntryWithLocation _dirent;
        ByteMemblock _sectorBuffer;
        FileSectorIterator _iterator;

      protected:
        void calcIndexes();

      public:
        FatFile(FatFileSystem& fs_,DirectoryEntryWithLocation& dirent_);

        /**
         * Virtual destructor, does nothing.
         */

        virtual ~FatFile() {}

      // get the dirent

        const DirectoryEntryWithLocation& getDirectoryEntryWithLocation();

      // overrides from File

        virtual bool read(void *ptr_,uint32_t size_,uint32_t& actuallyRead) override;
        virtual bool write(const void *ptr,uint32_t size) override;
        virtual bool seek(int32_t offset,SeekFrom origin) override;
        virtual uint32_t getLength() override;
    };
  }
}
