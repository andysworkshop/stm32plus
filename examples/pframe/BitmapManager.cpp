/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

BitmapManager::BitmapManager(LcdManager& lcdManager,FileSystemManager& fsManager)
  : Initialiser(lcdManager),
    _fsManager(fsManager) {
}


/*
 * Initialise the class
 */

bool BitmapManager::initialise() {

  // create the bitmap blocks on the sd card

  return createBitmapBlocks();
}


/*
 * Create the contiguous blocks that hold the bitmaps for fast access
 */

bool BitmapManager::createBitmapBlocks() {

  countImages();

  if(_fsManager.imagesAreCached())
    return true;

  return readImages();
}


/*
 * Count the images
 */

bool BitmapManager::countImages() {

  char buffer[30];
  FileInformation *finfo;
  uint32_t length;
  FileSystem& fs=_fsManager.getFileSystem();

  _lcdManager.getLcd().setForeground(ColourNames::WHITE);

  // status

  _term.writeString("Counting images");

  // iterate over sequential *.262 images in /pframe/img
  // e.g. 0.262, 1.262, 2.262 etc.

  _imageCount=0;

  for(;;) {

    // check that the image exists

    strcpy(buffer,"/pframe/img/");
    StringUtil::itoa(_imageCount,buffer+12,10);
    strcat(buffer,".262");

    if(!fs.getFileInformation(buffer,finfo))
      break;

    // verify that this is a bitmap by checking the size

    length=finfo->getLength();
    delete finfo;

    if(length!=IMAGE_BYTE_SIZE)
      return error("Invalid image format");

    _imageCount++;
    _term << '.';
  }

  _term << '\n';

  // check for no images

  if(_imageCount==0)
    return error("There are no images to play");

  _term << "Found " << _imageCount << " images\n";

  return true;
}


/*
 * Read the images
 */

bool BitmapManager::readImages() {

  char buffer[30];
  uint8_t block[512];
  uint32_t blockIndex,i,j,actuallyRead;
  FileSystem& fs=_fsManager.getFileSystem();
  File *file;
  BlockDevice& sdcard=_fsManager.getSdCard();

  // allocate enough blocks from the free space to hold the image data

  if(!_fsManager.allocateBlocks(_imageCount))
    return false;

  // status

  _term << "Caching images.\n";

  blockIndex=_fsManager.getFirstCacheBlock();

  for(i=0;i<_imageCount;i++) {

    // read and cache the image

    strcpy(buffer,"/pframe/img/");
    StringUtil::itoa(i,buffer+12,10);
    strcat(buffer,".262");

    if(!fs.openFile(buffer,file))
      return error("Failed to open file");

    _term.clearLine();
    _term << "Image: " << (i+1) << '/' << _imageCount;

    // 640 blocks/image (2 per row, 320 rows / image)
    // each block contains 1 scan line (240*4 bytes)

    for(j=0;j<640;j++) {

      if(!file->read(block,480,actuallyRead) || actuallyRead!=480)
        return error("IO error reading image");

      if(!sdcard.writeBlock(block,blockIndex++))
        return error("IO error writing to card");
    }

    delete file;
  }

  _term << '\n';
  return true;
}


/*
 * Open an image file
 */

bool BitmapManager::openImage(uint32_t imageIndex,File*& file) {

  char buffer[30];

  strcpy(buffer,"/pframe/img/");
  StringUtil::itoa(imageIndex,buffer+12,10);
  strcat(buffer,".262");

  return _fsManager.getFileSystem().openFile(buffer,file);
}

