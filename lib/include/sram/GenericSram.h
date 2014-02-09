/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Generic implementation of the SRAM interface
   */

  template<class TFsmc>
  class GenericSram : public FsmcSram<TFsmc>,
                      public BlockDevice {

    protected:
      static const uint32_t BLOCK_SIZE = 512;

      uint8_t *_sramStartAddress;

      /**
       * Error codes
       */

      enum {
        /// SRAM block device has no MBR
        E_NO_MBR = 1
      };

    public:
      GenericSram(const FsmcSramTiming& timing,const uint32_t lastAddressLineIndex);
      virtual ~GenericSram() {}

      // overrides from BlockDevice

      virtual uint32_t getBlockSizeInBytes() override;
      virtual bool readBlock(void *dest,uint32_t blockIndex) override;
      virtual bool readBlocks(void *dest,uint32_t blockIndex,uint32_t numBlocks) override;
      virtual bool writeBlock(const void *src,uint32_t blockIndex) override;
      virtual bool writeBlocks(const void *src,uint32_t blockIndex,uint32_t numBlocks) override;
      virtual formatType getFormatType() override;
      virtual bool getMbr(Mbr *mbr) override;

      /**
       * Cast to a void * for user access
       */

      operator void *() {
        return _sramStartAddress;
      }
  };


  /**
   * Constructor. Initialise the device
   */

  template<class TFsmc>
  inline GenericSram<TFsmc>::GenericSram(const FsmcSramTiming& timing,const uint32_t lastAddressLineIndex)
    : FsmcSram<TFsmc>(timing,lastAddressLineIndex) {

    // the bank of SRAM is not volatile (does not contain peripheral registers)

    _sramStartAddress=TFsmc::template getBaseAddress<uint8_t>();
  }


  /*
   * Get the block size.
   * @return always 512
   */

  template<class TFsmc>
  inline uint32_t GenericSram<TFsmc>::getBlockSizeInBytes() {
    return BLOCK_SIZE;
  }


  /*
   * Read a single block
   */

  template<class TFsmc>
  inline bool GenericSram<TFsmc>::readBlock(void *dest,uint32_t blockIndex)  {
    memcpy(dest,_sramStartAddress+(blockIndex * BLOCK_SIZE),BLOCK_SIZE);
    return true;
  }


  /*
   * Read many blocks
   */

  template<class TFsmc>
  inline bool GenericSram<TFsmc>::readBlocks(void *dest,uint32_t blockIndex,uint32_t numBlocks) {
    memcpy(dest,_sramStartAddress+(blockIndex * BLOCK_SIZE),numBlocks * BLOCK_SIZE);
    return true;
  }


  /*
   * Write a block
   */

  template<class TFsmc>
  inline bool GenericSram<TFsmc>::writeBlock(const void *src,uint32_t blockIndex) {
    memcpy(_sramStartAddress+(blockIndex * BLOCK_SIZE),src,BLOCK_SIZE);
    return true;
  }


  /*
   * Write many blocks
   */

  template<class TFsmc>
  inline bool GenericSram<TFsmc>::writeBlocks(const void *src,uint32_t blockIndex,uint32_t numBlocks) {
    memcpy(_sramStartAddress+(blockIndex * BLOCK_SIZE),src,numBlocks * BLOCK_SIZE);
    return true;
  }


  /*
   * Get the format type - no MBR
   */

  template<class TFsmc>
  inline BlockDevice::formatType GenericSram<TFsmc>::getFormatType() {
    return BlockDevice::formatNoMbr;
  }


  /*
   * Cannot get the MBR
   */

  template<class TFsmc>
  inline bool GenericSram<TFsmc>::getMbr(Mbr *mbr __attribute((unused))) {
    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SRAM,E_NO_MBR);
  }
}
