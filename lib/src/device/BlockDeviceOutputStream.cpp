/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/device.h"


namespace stm32plus {

  /**
   * Constructor
   * @param device The block device that this stream is attached to
   * @param firstBlock The index of the first block to begin writing to
   * @param buffered true if the block should be written back after each
   * and every write. false to only write back on flush() or when we move
   * to a new block.
   */

  BlockDeviceOutputStream::BlockDeviceOutputStream(BlockDevice& device,uint32_t firstBlock,bool buffered)
    : _device(device),
      _block(device.getBlockSizeInBytes()) {

    _blockIndex=firstBlock;
    _indexInBlock=0;
    _buffered=buffered;
  }


  /**
   * Destructor. Calls close.
   */

  BlockDeviceOutputStream::~BlockDeviceOutputStream() {
    close();
  }


  /**
   * Write a single byte to the stream/device
   * @param c The byte to write.
   * @return false if it fails
   */

  bool BlockDeviceOutputStream::write(uint8_t c) {
    return write(&c,1);
  }


  /**
   * Write a buffer of bytes
   * @param buffer A pointer to the buffer of bytes to write.
   * @param size The number of bytes in the buffer
   */

  bool BlockDeviceOutputStream::write(const void *buffer,uint32_t size) {

    uint32_t maxToWrite,toWrite;
    const uint8_t *ptr;

    ptr=reinterpret_cast<const uint8_t *>(buffer);

    while(size>0) {

      // write limit per iteration is up to the end of the block

      maxToWrite=_device.getBlockSizeInBytes()-_indexInBlock;
      toWrite=size<maxToWrite ? size : maxToWrite;

      // if necessary, fill the buffer with current data from the device

      if(!checkFillBuffer(toWrite))
        return false;

      // copy in this data

      memcpy(_block.getData()+_indexInBlock,ptr,toWrite);

      ptr+=toWrite;
      _indexInBlock+=toWrite;
      size-=toWrite;

      if(!checkFlush())
        return false;
    }

    return true;
  }


  /**
   * Check if the device must now be flushed given the user preference
   * and the current position of the write pointer
   * @return false if the device fails
   */

  bool BlockDeviceOutputStream::checkFlush() {

    if(!_buffered || _indexInBlock==_device.getBlockSizeInBytes()) {

      // user wants us to flush, or we've run out of space in the block

      if(!_device.writeBlock(_block,_blockIndex))
        return false;

      // if run out of space, increment block and reset pointer

      if(_indexInBlock==_device.getBlockSizeInBytes()) {
        _blockIndex++;
        _indexInBlock=0;
      }
    }

    return true;
  }


  /**
   * Check to see if the byte buffer needs filling to satisfy a
   * prospective write for toWrite bytes
   * @param toWrite The number of bytes we want to write
   * @return false if the device fails
   */

  bool BlockDeviceOutputStream::checkFillBuffer(uint32_t toWrite) {

    // if we are at the start of the block and we want to write less
    // than a full block then we need to get the current contents of
    // that block from the device

    if(_indexInBlock==0) {

      // cannot write anything at this point - we've run out of blocks

      if(_blockIndex==_device.getTotalBlocksOnDevice())
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BLOCK_DEVICE_OUTPUT_STREAM,E_DEVICE_FULL);

      // need to read if we write less than a block

      if(toWrite<_device.getBlockSizeInBytes())
        return _device.readBlock(_block,_blockIndex);
    }

    return true;
  }


  /**
   * Close will call flush on a buffered stream
   */

  bool BlockDeviceOutputStream::close() {

    if(_buffered)
      return flush();

    return true;
  }


  /**
   * Flush the current block to disk
   */

  bool BlockDeviceOutputStream::flush() {

    // write out if there is something to write

    if(_indexInBlock>0)
      return _device.writeBlock(_block,_blockIndex);

    return true;
  }
}
