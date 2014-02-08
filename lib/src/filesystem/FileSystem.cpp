/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {

  /*
   * Constructor
   */

  FileSystem::FileSystem(BlockDevice& blockDevice_,const TimeProvider& timeProvider_,uint32_t firstSectorIndex_) :
    _blockDevice(blockDevice_), _timeProvider(timeProvider_) {

    _firstSectorIndex=firstSectorIndex_;
  }

  /**
   * Destructor
   */

  FileSystem::~FileSystem() {
  }

  /**
   * Get a filesystem instance given the input parameters.
   *
   * @param[in] blockDevice The block device that holds the filesystem.
   * @param[in] timeProvider The provider of time information for write operations.
   * @param[out] newFileSystem The pointer to the new file system that the caller can use. Caller must delete when finished.
   * @return false if it fails.
   */

  bool FileSystem::getInstance(BlockDevice& blockDevice,const TimeProvider& timeProvider,FileSystem*& newFileSystem) {

    fat::FatFileSystem *fatFs;

    // is it FAT?

    if(fat::FatFileSystem::getInstance(blockDevice,timeProvider,fatFs)) {
      newFileSystem=fatFs;
      return true;
    }

    // nothing else supported

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_UNKNOWN_FILESYSTEM);
  }

  /**
   * Read a sector from the file system.
   *
   * @param[in] sectorIndex The sector index on the file system to read. Zero is the first sector on the file system, not the block device.
   * @param[in,out] buffer Caller supplied buffer large enough to hold a sector.
   * @return false if it fails.
   */

  bool FileSystem::readSector(uint32_t sectorIndex,void *buffer) {

    uint32_t blockIndex,sectorOffset;

    // convert sector to block index and read

    blockIndex=sectorIndexToBlockIndex(_firstSectorIndex + sectorIndex);

    // optimisation: where block size = sector size, read directly to the output buffer

    if(_blockDevice.getBlockSizeInBytes() == getSectorSizeInBytes())
      return _blockDevice.readBlock(buffer,blockIndex);
    else {

      Memblock<uint8_t> block(_blockDevice.getBlockSizeInBytes());

      if(!_blockDevice.readBlock(block,blockIndex))
        return false;

      // copy out of the block to the sector

      sectorOffset=sectorOffsetIntoBlock(sectorIndex);
      memcpy(buffer,static_cast<uint8_t *> (block) + sectorOffset,getSectorSizeInBytes());

      // done

      return true;
    }
  }

  /**
   * Write a sector to the file system.
   *
   * @param[in] sectorIndex The sector index on the file system to read. Zero is the first sector on the file system, not the block device.
   * @param[in] buffer Buffer that holds the sector data to write.
   * @return false if it fails.
   */

  bool FileSystem::writeSector(uint32_t sectorIndex,void *buffer) {

    uint32_t blockIndex;

    errorProvider.clear();

    // not supporting non-aligned block/sector sizes for now

    if(_blockDevice.getBlockSizeInBytes() != getSectorSizeInBytes())
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_FILESYSTEM,E_UNEQUAL_BLOCK_SECTOR_SIZES);

    // convert sector to block index and write

    blockIndex=sectorIndexToBlockIndex(_firstSectorIndex + sectorIndex);

    // optimisation: where block size = sector size, read directly to the output buffer

    return _blockDevice.writeBlock(buffer,blockIndex);
  }

  /*
   * Convert a sector index to a block index
   */

  uint32_t FileSystem::sectorIndexToBlockIndex(uint32_t sectorIndex_) const {
    return sectorIndex_ / _sectorsPerBlock;
  }

  /*
   * Get the byte offset into a block of the sector
   */

  uint32_t FileSystem::sectorOffsetIntoBlock(uint32_t sectorIndex_) const {
    return (sectorIndex_ % _sectorsPerBlock) * getSectorSizeInBytes();
  }

  /**
   * Get the time provider reference.
   *
   * @return A constant reference to the time provider.
   */

  const TimeProvider& FileSystem::getTimeProvider() const {
    return _timeProvider;
  }
}
