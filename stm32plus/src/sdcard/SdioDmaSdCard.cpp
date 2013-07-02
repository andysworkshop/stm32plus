/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/sdcard.h"


namespace stm32plus {

  /**
   * Constructor
   */

  SdioDmaSdCard::SdioDmaSdCard(bool autoInitialise) {

    // insert ourselves as an observer of the SDIO and DMA interrupts

    SdioInterruptFeature::insertObserver(*this);
    SdioDmaChannelInterruptFeature::insertObserver(*this);

    // initialise if we're supposed to do that

    if(autoInitialise) {

      // user must check error provider after the constructor finishes

      errorProvider.clear();
      powerOn() && initialiseCard();
    }
  }


  /*
   * Get block size in bytes. This is hardcoded to 512.
   */

  uint32_t SdioDmaSdCard::getBlockSizeInBytes() {
    return BLOCK_SIZE;
  }


  /**
   * Get the total number of blocks on this card.
   * @return The total number of blocks
   */

  uint32_t SdioDmaSdCard::getTotalBlocksOnDevice() {
    return getCardCapacityInBytes()/getBlockSizeInBytes();
  }


  /**
   * Get the card capacity in bytes
   * @return The total capacity as a 64-bit total
   */

  uint64_t SdioDmaSdCard::getCardCapacityInBytes() const {
    return _cardInfo.CardCapacity;
  }


  /**
   * Get the device format type
   * @return An enumeration that tells you whether this device has a Master Boot Record (MBR) or not.
   */

  BlockDevice::formatType SdioDmaSdCard::getFormatType() {

    switch(_cardInfo.SD_csd.FileFormat) {

      case 0:
        return formatMbr;

      case 1:
        return formatNoMbr;

      default:
        return formatUnknown;
    }
  }


  /**
   * Read a single block
   * @param dest Where to store the block data
   * @param blockIndex The block index to read
   * @return true if it worked
   */

  bool SdioDmaSdCard::readBlock(void *dest,uint32_t blockIndex) {

    _dmaFinished=_sdioFinished=false;

    // enable the relevant interrupts

    SdioInterruptFeature::enableInterrupts(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR);
    SdioDmaChannelInterruptFeature::enableInterrupts(SdioDmaChannelInterruptFeature::COMPLETE | SdioDmaChannelInterruptFeature::TRANSFER_ERROR);

    // issue the command

    if(!readBlockCommand(blockIndex,BLOCK_SIZE))
      return false;

    // use DMA to transfer the data

    beginRead(dest,BLOCK_SIZE);

    // wait for completion or error

    if(!waitForTransfer())
      return false;

    // wait for the peripheral to go quiet

    SdCardSdioFeature::waitForReceiveComplete();
    return true;
  }


  /**
   * Read multiple blocks. As of today I have an issue in that the low level multiblock commands are not
   * signalling completion. Until I fix that, multiblock commands are emulated with single block commands
   * @param dest Where to read the data
   * @param blockIndex The block index to start reading from
   * @param numBlocks The total number of 512 byte blocks to read
   * @return false if it fails
   */

  bool SdioDmaSdCard::readBlocks(void *dest,uint32_t blockIndex,uint32_t numBlocks) {

    uint8_t *ptr;

    for(ptr=static_cast<uint8_t *>(dest);numBlocks;numBlocks--,blockIndex++,ptr+=BLOCK_SIZE)
      if(!readBlock(ptr,blockIndex))
        return false;

    return true;

#if 0
    _dmaFinished=_sdioFinished=false;

    // enable the relevant interrupts

    SdioInterruptFeature::enableInterrupts(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR);
    SdioDmaChannelInterruptFeature::enableInterrupts(SdioDmaChannelInterruptFeature::COMPLETE | SdioDmaChannelInterruptFeature::TRANSFER_ERROR);

    // issue the command

    if(!readBlocksCommand(blockIndex,BLOCK_SIZE,numBlocks))
      return false;

    // use DMA to transfer the data

    beginRead(dest,numBlocks*BLOCK_SIZE);

    // wait for completion or error

    if(!waitForTransfer())
      return false;

    // wait for the peripheral to go quiet

    SdCardSdioFeature::waitForReceiveComplete();
    return true;
#endif
  }


  /**
   * Write a single block
   * @param src Memory address of the data to write
   * @param blockIndex The block index to write
   * @return false if it fails
   */

  bool SdioDmaSdCard::writeBlock(const void *src,uint32_t blockIndex) {

    _dmaFinished=_sdioFinished=false;

    // enable the relevant interrupts

    SdioInterruptFeature::enableInterrupts(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR);
    SdioDmaChannelInterruptFeature::enableInterrupts(SdioDmaChannelInterruptFeature::COMPLETE | SdioDmaChannelInterruptFeature::TRANSFER_ERROR);

    // issue the command

    if(!writeBlockCommand(blockIndex,BLOCK_SIZE))
      return false;

    // use DMA to transfer the data

    beginWrite(src,BLOCK_SIZE);

    // wait for completion or error

    if(!waitForTransfer())
      return false;

    // wait for the peripheral to go quiet

    SdCardSdioFeature::waitForTransmitComplete();
    return true;
  }


  /**
   * Write many blocks. As of today I have an issue in that the low level multiblock commands are not
   * signalling completion. Until I fix that, multiblock commands are emulated with single block commands
   * @param src Source of the data to write
   * @param blockIndex The first block start writing at
   * @param numBlocks The number of blocks to write
   * @return false if it fails
   */

  bool SdioDmaSdCard::writeBlocks(const void *src,uint32_t blockIndex,uint32_t numBlocks) {

    const uint8_t *ptr;

    for(ptr=static_cast<const uint8_t *>(src);numBlocks;numBlocks--,blockIndex++,ptr+=BLOCK_SIZE)
      if(!writeBlock(ptr,blockIndex))
        return false;

    return true;

#if 0
    _dmaFinished=_sdioFinished=false;

    // enable the relevant interrupts

    SdioInterruptFeature::enableInterrupts(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR);
    SdioDmaChannelInterruptFeature::enableInterrupts(SdioDmaChannelInterruptFeature::COMPLETE | SdioDmaChannelInterruptFeature::TRANSFER_ERROR);

    // issue the command

    if(!writeBlocksCommand(blockIndex,BLOCK_SIZE,numBlocks))
      return false;

    // use DMA to transfer the data

    beginWrite(dest,numBlocks*BLOCK_SIZE);

    // wait for completion or error

    if(!waitForTransfer())
      return false;

    // wait for the peripheral to go quiet

    SdCardSdioFeature::waitForTransmitComplete();
    return true;
#endif
  }


  /**
   * Wait for transfer complete, or error
   * @return
   */

  bool SdioDmaSdCard::waitForTransfer() const {

    // first wait for the SDIO interrupt

    while(!_sdioFinished);
    while(!_dmaFinished);

    // check for error

    if(_sdioErrorCode!=ErrorProvider::ERROR_NO_ERROR || _dmaErrorCode!=ErrorProvider::ERROR_NO_ERROR)
      return false;

    // wait for DMA to signal complete

    return true;
  }


  /**
   * SDIO interrupt observer
   * @param sender
   * @param event
   */

  void SdioDmaSdCard::onNotify(Observable& /* sender */,ObservableEvent::E event,void * /* context */) {

    bool disableSdioInterrupts=false,disableDmaInterrupts=false;

    switch(event) {

      case ObservableEvent::SDIO_DataEnd:
        _sdioErrorCode=ErrorProvider::ERROR_NO_ERROR;
        disableSdioInterrupts=true;
        break;

      case ObservableEvent::SDIO_CrcFail:
        _sdioErrorCode=E_CRC_FAIL;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      case ObservableEvent::SDIO_Timeout:
        _sdioErrorCode=E_TIMEOUT;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      case ObservableEvent::SDIO_RxOverrun:
        _sdioErrorCode=E_RXOVERRUN;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      case ObservableEvent::SDIO_TxOverrun:
        _sdioErrorCode=E_TXOVERRUN;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      case ObservableEvent::SDIO_StartBitError:
        _sdioErrorCode=E_STARTBITERROR;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      case ObservableEvent::DMA_TransferComplete:
        _dmaErrorCode=ErrorProvider::ERROR_NO_ERROR;
        disableDmaInterrupts=true;
        break;

      case ObservableEvent::DMA_TransferError:
        _dmaErrorCode=E_DMA_TRANSFER_ERROR;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      default:
        _sdioErrorCode=E_SDIO_ERROR;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;
    }

    // disable the relevant interrupts

    if(disableSdioInterrupts) {
      SdioInterruptFeature::disableInterrupts(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR);
      _sdioFinished=true;
    }

    if(disableDmaInterrupts) {
      SdioDmaChannelInterruptFeature::disableInterrupts(DMA_IT_TC | DMA_IT_TE);
      _dmaFinished=true;
    }
  }
}
