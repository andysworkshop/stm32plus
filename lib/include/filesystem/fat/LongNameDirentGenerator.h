/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fat {

    /**
     * @brief Handle the generation of dirents given a long filename.
     *
     * This class does all the work necessary to generate the sequence of directory entries
     * that are required to represent a long file name, i.e. a filename that is more than
     * the 8.3 format supported by old versions of MSDOS.
     *
     * This class does not actually write the new directory entries. The caller is responsible for that.
     */

    class LongNameDirentGenerator {

      protected:
        DirectoryEntryIterator& _targetDir;
        const char *_longName;
        DirectoryEntry *_dirents;
        uint16_t _createDate;
        uint16_t _createTime;
        int _direntCount;
        int _currentIteratorIndex;

      public:

        /**
         * Error codes
         */

        enum {
          /// The filename already exists in the directory.
          E_FILE_EXISTS=1
        };

      public:
        LongNameDirentGenerator(const char *longName_,DirectoryEntryIterator& targetDir_,uint16_t createDate_,uint16_t createTime_);
        ~LongNameDirentGenerator();

        int getDirentCount();
        DirectoryEntry *getDirents();

      protected:
        bool generateDirentsFromLongName();
        void generateShortName(char *shortName_) const;
        void computeLossyShortName(const char *shortName_,char *lossyName_,int tailNumber_) const;
        uint8_t shortNameChecksum(const uint8_t *shortName_) const;
        bool isLongNameValidShortName() const;
        void copyChars(const char *& src_,int& srcLen_,uint16_t *dest_,int destLen_);
        bool findUniqueShortName(char *shortName_);
    };
  }
}
