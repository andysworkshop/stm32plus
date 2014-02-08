/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

FileSystemManager::FileSystemManager(LcdManager& lcdManager)
  : Initialiser(lcdManager) {
}


/*
 * initialise
 */

bool FileSystemManager::initialise() {

  _term.writeString("Initialising SD card.\n");

  // create the SDIO block device - the constructor will attempt to deduce the optimum
  // clock dividers for initialisation and data transfer

  _sdcard=new SdioDmaSdCard;
  if(errorProvider.getLast()!=0)
    return error("Failed to init SD card");

  // attach the block device to the file system

  _term.writeString("Initialising file system.\n");

  _cachedBlockDevice=new CachedBlockDevice(*_sdcard,4);

  if(!FileSystem::getInstance(*_cachedBlockDevice,*new NullTimeProvider,_fs))
    return error("Failed to init file system");

  readImageCacheStatus();
  return true;
}


/*
 * Get the status of the image cache. /pframe/cacheloc-nnnnn encodes the first block in
 * the name. This is to keep the file zero length, i.e. does not allocate a cluster
 * and therefore cannot corrupt the cache when created.
 */

void FileSystemManager::readImageCacheStatus() {

  DirectoryIterator *it;

  // preset for not-set

  _imagesAreCached=false;

  // iterate the pframe directory

  if(!_fs->getDirectoryIterator("/pframe",it))
    return;

  while(it->next()) {

    if(!strncasecmp("cacheloc-",it->current().getFilename(),9)) {
      _firstCacheBlock=atoi(it->current().getFilename()+9);
      _imagesAreCached=true;
      break;
    }
  }

  delete it;
}


/*
 * Allocate sequential blocks for the buffer
 */

bool FileSystemManager::allocateBlocks(uint32_t imageCount) {

  uint32_t blocksRequired,blocksPerCluster,clustersRequired,firstCluster;
  char buffer[50];

  // the filesystem is required to be FATxx so this is safe

  fat::FatFileSystem& fs=(fat::FatFileSystem&)*_fs;
  fat::LinearFreeClusterFinder finder(fs);

  // status

  _term.writeString("Allocating image cache.\n");

  // get the number of clusters required for the buffer

  blocksRequired=2*320*imageCount;
  blocksPerCluster=fs.getBootSector().BPB_SecPerClus;
  clustersRequired=blocksRequired/blocksPerCluster;

  if(blocksRequired % blocksPerCluster!=0)
    clustersRequired++;

  // ask the FS for the first cluster of a sequential free block
  // long enough for our buffer

  if(!finder.findMultipleSequential(clustersRequired,firstCluster))
    return error("Not enough free space on card");

  // convert the cluster number to a block number

  _firstCacheBlock=fs.clusterToSector(firstCluster)+_fs->getFirstSector();

  // write the cache

  strcpy(buffer,"/pframe/cacheloc-");
  StringUtil::itoa(_firstCacheBlock,buffer+17,10);

  _fs->createFile(buffer);
  return true;
}
