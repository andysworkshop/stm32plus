/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {


  /**
   * @brief Base class for iterating over the entries in a FAT directory.
   *
   * This class provides the static factory method used for returning an appropriate
   * derived class.
   */

    class FatDirectoryIterator : public DirectoryIterator, public FatFileInformation {

      protected:
        FatFileSystem& _fs;
        DirectoryEntryIterator *_entryIterator;

      protected:
        FatDirectoryIterator(FatFileSystem& fs,DirectoryEntryWithLocation& dirent);
        FatDirectoryIterator(FatFileSystem& fs);

      public:
        static bool getInstance(FatFileSystem& fs,const TokenisedPathname& tp,FatDirectoryIterator *& newIterator);

        virtual ~FatDirectoryIterator();

        DirectoryEntryIterator& getDirectoryEntryIterator();

      // overrides from Iterator<FileInformation>

        virtual bool next() override;
        virtual const FileInformation& current() override;

      // overrides from DirectoryIterator

        virtual bool getSubdirectoryIterator(DirectoryIterator *& newIterator) override;
        virtual bool isCurrentDirectory() override;
        virtual bool isParentDirectory() override;
        virtual bool moveTo(const char *filename) override;
        virtual bool openFile(File*& newFile) override;

      // overrides from FileInformation

        virtual const char *getFilename() const override;
        virtual uint32_t getAttributes() const override;
        virtual time_t getCreationDateTime() const override;
        virtual time_t getLastWriteDateTime() const override;
        virtual time_t getLastAccessDateTime() const override;
        virtual uint32_t getLength() const override;

      // overrides from FatFileInformation

        DirectoryEntryWithLocation& getDirectoryEntryWithLocation();
    };
  }
}
