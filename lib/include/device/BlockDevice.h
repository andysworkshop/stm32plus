/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

  /**
   * @brief Base class for block devices
   *
   * Block devices are expected to be able to read and write data in
   * units of 'blocks'. The size of a block is defined by the device.
   */

  class BlockDevice {
    public:

      /**
       * Error codes
       */

      enum {
        /// first sector on device is not an MBR
        E_INVALID_MBR=1,

        /// device does not have an MBR
        E_NO_MBR=2
      };


      /**
       * Type of format of this block device. Describes whether or not
       * it has an MBR
       */

      enum formatType {
        /// device has no MBR, the one-and-only partition starts at sector 0
        formatNoMbr,

        /// device has an MBR with a partition table
        formatMbr,

        /// unknown or unsupported format
        formatUnknown
      };

    public:

      /**
       * Virtual destructor, do nothing
       */

      virtual ~BlockDevice() {
      }


      /**
       * Get the total number of blocks on this device.
       * @return The number of blocks on the device
       */

      virtual uint32_t getTotalBlocksOnDevice()=0;


      /**
       * Get the block size in bytes.
       * @return The block size.
       */

      virtual uint32_t getBlockSizeInBytes()=0;

      /**
       * Read a single block from the device.
       * @param[in,out] dest Caller supplied memory buffer to hold the block.
       * @param[in] blockIndex The zero-based block index to read.
       * @return false if it fails
       */

      virtual bool readBlock(void *dest,uint32_t blockIndex)=0;

      /**
       * Read multiple blocks from the device.
       * @param[in,out] dest Caller supplied memory buffer to hold the blocks.
       * @param[in] blockIndex The zero-based first block index to read.
       * @param[in] numBlocks The number of blocks to read.
       * @return false if it fails
       */

      virtual bool readBlocks(void *dest,uint32_t blockIndex,uint32_t numBlocks)=0;

      /**
       * Write a single block to the device.
       * @param[in] src The memory buffer that the device sources data from
       * @param[in] blockIndex The zero-based block index to write.
       */

      virtual bool writeBlock(const void *src,uint32_t blockIndex)=0;

      /**
       * Write multiple blocks to the device.
       * @param[in] src The memory buffer that the device sources data from
       * @param[in] blockIndex The zero-based first block index to write.
       * @param[in] numBlocks The number of blocks to write.
       */

      virtual bool writeBlocks(const void *src,uint32_t blockIndex,uint32_t numBlocks)=0;


      /**
       * Get the type of this block device.
       * @return The format type, i.e. whether this is device has an MBR or whether it behaves like a large floppy.
       */

      virtual formatType getFormatType()=0;


      /**
       * Get the MBR off this device, if it has one
       * @see getFormatType()
       * @return false if it fails.
       */

      virtual bool getMbr(Mbr *mbr);
  };
}
