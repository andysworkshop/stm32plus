/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

  /**
   * Implementation of BlockDevice for an SD Card accessed over DMA. This class gathers
   * together the required parts to form a coherent read/write SDIO accessor that
   * uses the DMA channels, albeit blocking the CPU until DMA transfer is complete
   */

  class SdioDmaSdCard : public BlockDevice,
                        public SdCard<SdCardSdioFeature,SdioInterruptFeature>,
                        public SdioDmaChannel<SdioDmaChannelInterruptFeature,
                                              SdioDmaReaderFeature<>,
                                              SdioDmaWriterFeature<> > {

    protected:
      volatile int _sdioErrorCode;
      volatile int _dmaErrorCode;
      volatile bool _dmaFinished;
      volatile bool _sdioFinished;

    public:
      enum { BLOCK_SIZE = 512 };

    public:
      SdioDmaSdCard(bool autoInitialise=true);
      virtual ~SdioDmaSdCard() {}

      bool waitForTransfer() const;
      uint64_t getCardCapacityInBytes() const;

      // overrides from BlockDevice

      virtual uint32_t getTotalBlocksOnDevice() override;
      virtual uint32_t getBlockSizeInBytes() override;

      virtual bool readBlock(void *dest,uint32_t blockIndex) override;
      virtual bool readBlocks(void *dest,uint32_t blockIndex,uint32_t numBlocks) override;

      virtual bool writeBlock(const void *src,uint32_t blockIndex) override;
      virtual bool writeBlocks(const void *src,uint32_t blockIndex,uint32_t numBlocks) override;

      virtual formatType getFormatType() override;

      // SDIO event subscription

      void onSdioEvent(SdioEventType set);
      void onDmaEvent(DmaEventType det);
  };
}
