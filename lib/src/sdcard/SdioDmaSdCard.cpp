/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F1_HD) || defined(STM32PLUS_F4)

#include "config/sdcard.h"


namespace stm32plus {

  /**
   * Constructor
   */

  SdioDmaSdCard::SdioDmaSdCard(bool autoInitialise) {

    // subscribe to the SDIO and DMA events

    SdioEventSource::SdioInterruptEventSender.insertSubscriber(SdioInterruptEventSourceSlot::bind(this,&SdioDmaSdCard::onSdioEvent));
    DmaEventSource::DmaInterruptEventSender.insertSubscriber(DmaInterruptEventSourceSlot::bind(this,&SdioDmaSdCard::onDmaEvent));

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

    _dmaFinished=_sdioFinished=false;

    // enable the relevant interrupts

    SdioInterruptFeature::enableInterrupts(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR);
    SdioDmaChannelInterruptFeature::enableInterrupts(SdioDmaChannelInterruptFeature::COMPLETE | SdioDmaChannelInterruptFeature::TRANSFER_ERROR);

    // issue the command

    if(!readBlocksCommand(blockIndex,BLOCK_SIZE,numBlocks))
      return false;

    // use DMA to transfer the data

    beginRead(dest,numBlocks*BLOCK_SIZE);

    // wait for completion or error and send the stop transfer command

    if(!waitForTransfer() || !stopTransfer())
      return false;

    // wait for the peripheral to go quiet

    SdCardSdioFeature::waitForReceiveComplete();
    return true;
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

    _dmaFinished=_sdioFinished=false;

    // enable the relevant interrupts

    SdioInterruptFeature::enableInterrupts(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR);
    SdioDmaChannelInterruptFeature::enableInterrupts(SdioDmaChannelInterruptFeature::COMPLETE | SdioDmaChannelInterruptFeature::TRANSFER_ERROR);

    // issue the command

    if(!writeBlocksCommand(blockIndex,BLOCK_SIZE,numBlocks))
      return false;

    // use DMA to transfer the data

    beginWrite(src,numBlocks*BLOCK_SIZE);

    // wait for completion or error and issue the stop transfer command

    if(!waitForTransfer() || !stopTransfer())
      return false;

    // wait for the peripheral to go quiet

    SdCardSdioFeature::waitForTransmitComplete();
    return true;
  }


  /**
   * Wait for transfer complete, or error
   * @return
   */

  bool SdioDmaSdCard::waitForTransfer() const {

    // first wait for the SDIO interrupt

    while(!_sdioFinished);
    while(!_dmaFinished);

    // clear static flags

    SDIO_ClearFlag(SDIO_STATIC_FLAGS);

    // check for error

    return _sdioErrorCode==ErrorProvider::ERROR_NO_ERROR && _dmaErrorCode==ErrorProvider::ERROR_NO_ERROR;
  }


  /**
   * SDIO interrupt handler
   * @param set The type of event
   */

  void SdioDmaSdCard::onSdioEvent(SdioEventType set) {

    bool disableSdioInterrupts=false,disableDmaInterrupts=false;

    switch(set) {

      case SdioEventType::EVENT_DATA_END:
        _sdioErrorCode=ErrorProvider::ERROR_NO_ERROR;
        disableSdioInterrupts=true;
        break;

      case SdioEventType::EVENT_CRC_FAIL:
        _sdioErrorCode=E_CRC_FAIL;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      case SdioEventType::EVENT_TIMEOUT:
        _sdioErrorCode=E_TIMEOUT;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      case SdioEventType::EVENT_RX_OVERRUN:
        _sdioErrorCode=E_RXOVERRUN;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      case SdioEventType::EVENT_TX_OVERRUN:
        _sdioErrorCode=E_TXOVERRUN;
        disableSdioInterrupts=disableDmaInterrupts=true;
        break;

      case SdioEventType::EVENT_START_BIT_ERROR:
        _sdioErrorCode=E_STARTBITERROR;
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


  /**
   * SDIO DMA interrupt events
   */

  void SdioDmaSdCard::onDmaEvent(DmaEventType det) {

    bool disableSdioInterrupts=false,disableDmaInterrupts=false;

    switch(det) {

      case DmaEventType::EVENT_COMPLETE:
        _dmaErrorCode=ErrorProvider::ERROR_NO_ERROR;
        disableDmaInterrupts=true;
        break;

      case DmaEventType::EVENT_TRANSFER_ERROR:
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

#endif
