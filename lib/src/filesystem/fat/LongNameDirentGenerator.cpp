/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {
  namespace fat {

    /**
     * Constructor. Generates the dirents from the supplied parameters. Check the error provider
     * afterwards for errors.
     *
     * @param[in] longName_ The desired long name for the new file.
     * @param[in] targetDir_ A directory iterator pointing to the directory that will hold the new file.
     * @param[in] createDate_ The creation date of the new file.
     * @param[in] createTime_ The creation time of the new file. The tenths field will be set to zero.
     */

    LongNameDirentGenerator::LongNameDirentGenerator(const char *longName_,DirectoryEntryIterator& targetDir_,uint16_t createDate_,uint16_t createTime_)
      : _targetDir(targetDir_),
        _longName(longName_),
        _dirents(nullptr),
        _createDate(createDate_),
        _createTime(createTime_),
        _currentIteratorIndex(-1) {

      if(generateDirentsFromLongName())
        errorProvider.clear();
    }

    /**
     * Virtual destructor, frees memory used by the directory entries.
     */

    LongNameDirentGenerator::~LongNameDirentGenerator() {
      if(_dirents != nullptr)
        delete[] _dirents;
    }

    /**
     * Get the number of dirents.
     * @return The number of dirents required to store the new file.
     */

    int LongNameDirentGenerator::getDirentCount() {
      return _direntCount;
    }

    /**
     * Get the dirents array.
     * @return An internal pointer to the array of dirents that the constructor generated. Owned by
     * this class, do not delete.
     */

    DirectoryEntry *LongNameDirentGenerator::getDirents() {
      return _dirents;
    }

    /*
     * Generate the DirectoryEntry structures from the long name
     */

    bool LongNameDirentGenerator::generateDirentsFromLongName() {

      char shortName[12];
      int namelen=0;
      DirectoryEntry *de;
      const char *src;
      uint8_t checksum,ordinal;

      // if the long name is a valid short name then there will be only
      // 1 dirent for the short name

      if(isLongNameValidShortName()) {
        _direntCount=1;
      } else {

        // 13 characters per dirent

        namelen=strlen(_longName);
        _direntCount=(namelen / 13) + (namelen % 13 == 0 ? 1 : 2);
      }

      // check for unique name. Only long names can have the short name
      // modified up

      if(!findUniqueShortName(shortName))
        return false; // FAIL.

      // need memory for these structures

      _dirents=new DirectoryEntry[_direntCount];

      // create the short entry, the last in the sequence

      de=&_dirents[_direntCount - 1];

      memcpy(de->sdir.DIR_Name,shortName,11);
      de->sdir.DIR_Attr=DirectoryEntry::ATTR_ARCHIVE;
      de->sdir.DIR_CrtDate=de->sdir.DIR_LstAccDate=de->sdir.DIR_WrtDate=_createDate;
      de->sdir.DIR_CrtTime=de->sdir.DIR_WrtTime=_createTime;
      de->sdir.DIR_CrtTimeTenth=0;
      de->sdir.DIR_FileSize=0;
      de->sdir.DIR_FstClusHI=de->sdir.DIR_FstClusLO=0;
      de->sdir.DIR_NTRes=0;

      if(_direntCount > 1) {

        // create the long entries

        src=_longName;
        checksum=shortNameChecksum(_dirents[_direntCount - 1].sdir.DIR_Name);
        ordinal=1;

        // long entries are in reverse order

        for(de=&_dirents[_direntCount - 2];de >= _dirents;de--) {

          // index of this long entry

          de->ldir.LDIR_Ord=ordinal++;

          if(de == _dirents)
            de->ldir.LDIR_Ord|=0x40;

          // basics

          de->ldir.LDIR_Attr=DirectoryEntry::ATTR_LONG_NAME;
          de->ldir.LDIR_Type=0;
          de->ldir.LDIR_Chksum=checksum;
          de->ldir.LDIR_FstClusLO=0;

          // name parts

          copyChars(src,namelen,de->ldir.LDIR_Name1,5);
          copyChars(src,namelen,de->ldir.LDIR_Name2,6);
          copyChars(src,namelen,de->ldir.LDIR_Name3,2);
        }
      }

      return true;
    }

    /*
     * find unique short name for the long name
     */

    bool LongNameDirentGenerator::findUniqueShortName(char *shortName_) {

      char lossyName[11];
      int tailNumber;
      bool found;

      _targetDir.reset();

      // first pass through to ensure that the filename is unique

      while(_targetDir.next()) {
        if(!strcasecmp(_targetDir.getFilename(),_longName))
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_LONG_FILENAME_GENERATOR,E_FILE_EXISTS);
      }

      // generate the short name

      generateShortName(shortName_);

      // if there will only one dirent then that's it

      if(_direntCount == 1)
        return true;

      // now need to ensure that the short name does not clash with another
      // start the process by checking the unadulterated short name

      for(tailNumber=1,found=true;found;tailNumber++) {

        // compute the lossy name

        computeLossyShortName(shortName_,lossyName,tailNumber);

        _targetDir.reset();
        found=false;

        // search directory entries

        while(_targetDir.next()) {

          // check the dirent shortname against the current lossy name (or short name if first around)

          if(!memcmp(_targetDir.current().Dirent.sdir.DIR_Name,lossyName,11)) {
            found=true;
            break;
          }
        }
      }

      // keep the current find

      memcpy(shortName_,lossyName,11);
      return true;
    }

    /*
     * copy in characters
     */

    void LongNameDirentGenerator::copyChars(const char *& src_,int& srcLen_,uint16_t *dest_,int destLen_) {

      char *dest,padding,c;

      // the chars in the name are 16 bit unicode and we only support 8-bit chars encoded as
      // xx00 so we'll need to treat the dest as an 8-bit pointer to ensure that
      // endian-ness of the CPU doesn't come into play

      dest=reinterpret_cast<char *> (dest_);

      while(destLen_--) {

        // when past the nul terminated string pad to end with 0xff

        if(srcLen_ < 0) {
          c='\xff';
          padding='\xff';
        } else {
          c=*src_++; // still in the string, take a character including the ending nul
          padding='\0';
          srcLen_--;
        }

        *dest++=c;
        *dest++=padding;
      }
    }

    /*
     * check if this long name is a valid short name
     */

    bool LongNameDirentGenerator::isLongNameValidShortName() const {

      static const char *permittedSpecialCharacters="$%'-_@~!(){}^#&";

      int seenChars;
      const char *ptr,*dotPos;

      dotPos=nullptr;
      seenChars=0;

      for(ptr=_longName;*ptr;ptr++) {

        // no more than 3 characters after the dot

        if(dotPos != nullptr && ptr - dotPos > 3)
          return false;

        // one period is permitted

        if(*ptr == '.') {

          if(dotPos != nullptr)
            return false;

          dotPos=ptr;
        } else {

          // must be a permitted character (upper case alphanum, >127 or in the special set)

          if(!isupper(*ptr) && !isdigit(*ptr) && *ptr < 128 && strchr(permittedSpecialCharacters,*ptr) == nullptr)
            return false;

          // no more than 11 characters total not including the dot

          if(++seenChars > 11)
            return false;
        }
      }

      // it's OK

      return true;
    }

    /*
     * Generate equivalent short name from long name
     * shortName_ must point to at least 11 chars
     * this function will not null terminate the short name
     */

    void LongNameDirentGenerator::generateShortName(char *shortName_) const {

      char *dest;
      const char *src,*lastDot;
      int copied;

      copied=0;
      lastDot=strrchr(_longName,'.');

      // copy first 8 chars, or up to the last period, or the end

      for(src=_longName,dest=shortName_;copied < 8 && *src && src != lastDot;src++) {

        if(*src != ' ' && *src != '.') {
          *dest++=toupper(*src);
          copied++;
        }
      }

      // fill out remaining base name with spaces

      while(copied < 8) {
        *dest++=' ';
        copied++;
      }

      // copy up to three non-space chars after the last dot

      copied=0;

      if(lastDot) {

        src=lastDot + 1;
        while(copied < 3 && *src) {

          if(*src != ' ') {
            *dest++=toupper(*src);
            src++;
            copied++;
          }
        }
      }

      // fill out with spaces

      while(copied < 3) {
        *dest++=' ';
        copied++;
      }
    }

    /*
     * Generate a lossy name from the short name
     * e.g. FOO.DOC is FOO~1.DOC
     * short name layout must be in the dirent format,
     * e.g. FOO.DOC = "FOO     DOC"
     * tailNumber_ must not be greater than 999999
     */

    void LongNameDirentGenerator::computeLossyShortName(const char *shortName_,char *lossyName_,int tailNumber_) const {

      char tail[8];
      int state,tailSize,i;
      const char *src,*bodyPtr;

      // get the tail size

      tail[0]='~';
      StringUtil::itoa(tailNumber_,tail + 1,10);
      tailSize=strlen(tail);

      // start copying the basename (state=0) until either a space is encountered
      // or we must switch to the tail because of space

      src=bodyPtr=shortName_;
      state=0;

      for(i=0;i < 11;i++) {

        if(state == 0 && (*src == ' ' || i + tailSize == 8)) {

          // switch to copying the tail

          state=1;
          src=tail;
        } else if(state == 1 && *src == '\0') {

          // tail is copied, back to the basename

          state=2;
          src=bodyPtr;
        }

        *lossyName_++=*src++;
        bodyPtr++;
      }
    }

    /*
     * Compute the checksum for the short name. The short name must be exactly 11
     * characters long.
     */

    uint8_t LongNameDirentGenerator::shortNameChecksum(const uint8_t *shortName_) const {

      int i;
      uint8_t sum;

      // NOTE: The operation is an unsigned char rotate right

      sum=0;
      for(i=11;i > 0;i--)
        sum=((sum & 1) ? 0x80 : 0) + (sum >> 1) + *shortName_++;

      return sum;
    }
  }
}
