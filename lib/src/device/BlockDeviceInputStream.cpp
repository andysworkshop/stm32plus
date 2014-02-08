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
   */

  BlockDeviceInputStream::BlockDeviceInputStream(BlockDevice& device,uint32_t firstBlock)
    : _device(device),
      _block(device.getBlockSizeInBytes()) {

    _blockIndex=_originalBlockIndex=firstBlock;
    _indexInBlock=0;
  }


  /**
   * Read a character from the stream
   * @return the character read or E_END_OF_STREAM / E_STREAM_ERROR
   */

  int16_t BlockDeviceInputStream::read() {

    int16_t errorCode;

    // need 1 byte

    if((errorCode=requireBytes())!=ErrorProvider::ERROR_NO_ERROR)
      return errorCode;

    // get from the block

    return _block[_indexInBlock++];
  }


  /**
   * Read a sequence of bytes. Return false if an error occurs or there are no more
   * bytes to read. Returns true if at least one byte was read successfully. Fewer
   * bytes may be read than you asked for if the end of the device was reached. Check
   * actuallyRead for the total returned.
   * @param buffer Where to write to
   * @param Maximum byte size to read
   * @param actuallyRead How much was actually read.
   * @return false if it fails or no bytes are available
   */

  bool BlockDeviceInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

    uint32_t available,count;
    uint8_t *ptr;
    int16_t errorCode;

    // reset count

    actuallyRead=0;
    ptr=reinterpret_cast<uint8_t *>(buffer);

    while(size>0) {

      // max per loop can be up to the block end

      available=_device.getBlockSizeInBytes()-_indexInBlock;
      count=size<available ? size : available;

      // make sure they're here

      if((errorCode=requireBytes())!=ErrorProvider::ERROR_NO_ERROR) {

        // no bytes available now is an error if no bytes
        // have been available at all

        if(errorCode==E_END_OF_STREAM)
          return actuallyRead>0;

        // some other device failure

        return false;
      }

      // copy what we have

      memcpy(ptr,_block.getData()+_indexInBlock,count);
      ptr+=count;
      actuallyRead+=count;
      size-=count;
      _indexInBlock+=count;
    }

    return true;
  }


  /**
   *  Can't close but it's not a fail
   *  @return true
   */

  bool BlockDeviceInputStream::close() {
    return true;
  }


  /**
   * Can skip if there's enough blocks to move into
   * @return false if it would fail - the internal position is not changed on failure
   */

  bool BlockDeviceInputStream::skip(uint32_t howMuch) {

    uint32_t newBlock,newIndex;

    // bump forward as many whole blocks as required

    newBlock=_blockIndex+(howMuch / _device.getBlockSizeInBytes());

    // and by how much in addition to whole blocks

    newIndex=_blockIndex+(howMuch % _device.getBlockSizeInBytes());

    // gone past a block boundary? update if so

    if(newIndex>=_device.getBlockSizeInBytes()) {
      newBlock++;
      newIndex-=_device.getBlockSizeInBytes();
    }

    // check for end of device

    if(newBlock>=_device.getTotalBlocksOnDevice())
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BLOCK_DEVICE_INPUT_STREAM,E_INVALID_SEEK_POSITION);

    // update positions and read in the new block

    if(newBlock!=_blockIndex)
      if(!_device.readBlock(_block,newBlock))
        return false;

    _blockIndex=newBlock;
    _indexInBlock=newIndex;

    return true;
  }


  /**
   * return true if there is more data available
   * @return false if no more data is available
   */

  bool BlockDeviceInputStream::available() {

    // data is available if we're not at the last block
    // or the index in the block is less than the end

    return _blockIndex<_device.getTotalBlocksOnDevice() ||
           _indexInBlock<_device.getBlockSizeInBytes();
  }


  /**
   * Reset stream to the beginning. This is supported
   * @return true
   */

  bool BlockDeviceInputStream::reset() {

    _blockIndex=_originalBlockIndex;
    _indexInBlock=0;

    return true;
  }


  /**
   * require the given number of bytes to be available.
   * @param numBytes must not run past the size of a block
   */

  int16_t BlockDeviceInputStream::requireBytes() {

    // if the index has hit the end, advance to new block

    if(_indexInBlock==_device.getBlockSizeInBytes()) {

      _indexInBlock=0;
      _blockIndex++;

      // check for end of the device

      if(_blockIndex==_device.getTotalBlocksOnDevice())
        return E_END_OF_STREAM;
    }

    // if we're at position zero then new bytes must be read in

    if(_indexInBlock==0)
      if(!_device.readBlock(_block,_blockIndex))
        return E_STREAM_ERROR;

    return ErrorProvider::ERROR_NO_ERROR;
  }
}
