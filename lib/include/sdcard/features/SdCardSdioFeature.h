/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base Implementation of the SD card for SDIO. Provides SDIO initialisation functionality.
   *
   * The code here is nearly all adapted from the driver sample code provided by ST. The rights of the original
   * authors are preserved under the following license statement provided by ST:
   *
   * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
   *
   * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
   * You may not use this file except in compliance with the License.
   * You may obtain a copy of the License at:
   *
   *        http://www.st.com/software_license_agreement_liberty_v2
   *
   * Unless required by applicable law or agreed to in writing, software
   * distributed under the License is distributed on an "AS IS" BASIS,
   * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   * See the License for the specific language governing permissions and
   * limitations under the License.
   *
   */

  class SdCardSdioFeature : public SdCardIoFeature {

    public:

      struct CardInfo {
        SdCsd SD_csd;
        SdCid SD_cid;
        uint64_t CardCapacity;  /*!< Card Capacity */
        uint32_t CardBlockSize; /*!< Card Block Size */
        uint16_t RCA;
        uint8_t CardType;
      } _cardInfo;

    protected:
      uint8_t _initDivider;
      uint8_t _transferDivider;
      uint32_t _cardType;
      uint32_t _csdTab[4];
      uint32_t _cidTab[4];
      uint32_t _rca;

      enum {
        E_SDIO_ERROR = 1,
        E_CRC_FAIL,
        E_TIMEOUT,
        E_RXOVERRUN,
        E_TXOVERRUN,
        E_STARTBITERROR,
        E_DMA_TRANSFER_ERROR
      };

      enum {
        SD_STD_CAPACITY = 0,
        SD_HIGH_CAPACITY = 0x40000000,
        SD_CHECK_PATTERN = 0x000001AA,
        SD_MAX_VOLT_TRIAL = 0x0000FFFF,
        SD_VOLTAGE_WINDOW_SD = 0x80100000,
        SDIO_CMD0TIMEOUT = 0x00010000,
        SD_DATATIMEOUT = 0xFFFFFFFF,
        SDIO_STATIC_FLAGS = 0x000005FF,
        SDIO_SEND_IF_COND = 0x00000008,
        SD_ALLZERO = 0,
        SD_CARD_LOCKED = 0x02000000,
        SD_WIDE_BUS_SUPPORT = 0x00040000,
        SD_SINGLE_BUS_SUPPORT = 0x00010000,
        SD_0TO7BITS = 0x000000FF,
        SD_8TO15BITS = 0x0000FF00,
        SD_16TO23BITS = 0x00FF0000,
        SD_24TO31BITS = 0xFF000000
      };

      enum {
        SD_OCR_ADDR_OUT_OF_RANGE        =0x80000000,
        SD_OCR_ADDR_MISALIGNED          =0x40000000,
        SD_OCR_BLOCK_LEN_ERR            =0x20000000,
        SD_OCR_ERASE_SEQ_ERR            =0x10000000,
        SD_OCR_BAD_ERASE_PARAM          =0x08000000,
        SD_OCR_WRITE_PROT_VIOLATION     =0x04000000,
        SD_OCR_LOCK_UNLOCK_FAILED       =0x01000000,
        SD_OCR_COM_CRC_FAILED           =0x00800000,
        SD_OCR_ILLEGAL_CMD              =0x00400000,
        SD_OCR_CARD_ECC_FAILED          =0x00200000,
        SD_OCR_CC_ERROR                 =0x00100000,
        SD_OCR_GENERAL_UNKNOWN_ERROR    =0x00080000,
        SD_OCR_STREAM_READ_UNDERRUN     =0x00040000,
        SD_OCR_STREAM_WRITE_OVERRUN     =0x00020000,
        SD_OCR_CID_CSD_OVERWRIETE       =0x00010000,
        SD_OCR_WP_ERASE_SKIP            =0x00008000,
        SD_OCR_CARD_ECC_DISABLED        =0x00004000,
        SD_OCR_ERASE_RESET              =0x00002000,
        SD_OCR_AKE_SEQ_ERROR            =0x00000008,
        SD_OCR_ERRORBITS                =0xFDFFE008
      };

      enum {
        SD_R6_GENERAL_UNKNOWN_ERROR =0x00002000,
        SD_R6_ILLEGAL_CMD           =0x00004000,
        SD_R6_COM_CRC_FAILED        =0x00008000
      };

      enum {
        SD_CCCC_LOCK_UNLOCK = (uint32_t)0x00000080,
        SD_CCCC_WRITE_PROT  = (uint32_t)0x00000040,
        SD_CCCC_ERASE       = (uint32_t)0x00000020
      };

    protected:
      SD_Error enableWideBus(FunctionalState NewState);
      SD_Error findSCR(uint32_t *pscr);

      SD_Error cmdError();
      SD_Error cmdResp1Error(uint8_t cmd) const;
      SD_Error cmdResp2Error();
      SD_Error cmdResp3Error();
      SD_Error cmdResp6Error(uint8_t cmd,uint16_t *prca);
      SD_Error cmdResp7Error();

      SD_Error sendStatus(uint32_t& pcardstatus) const;
      SD_Error isCardProgramming(uint8_t& pstatus) const;

      bool stopTransfer() const;
      uint32_t waitStatusFlags() const;

    public:
      SdCardSdioFeature(SdCardBase& sdcard);

      void detectDividers();

      bool powerOn();
      void powerOff();

      bool initialiseCard();
      bool detectCardInfo();
      bool selectDeselect(uint32_t addr);
      bool enableWideBusOperation(uint32_t WideMode);

      bool readBlockCommand(uint32_t blockIndex,uint16_t blockSize);
      bool readBlocksCommand(uint32_t blockIndex,uint16_t blockSize,uint32_t numberOfBlocks);

      bool writeBlockCommand(uint32_t blockIndex,uint16_t blockSize);
      bool writeBlocksCommand(uint32_t blockIndex,uint16_t blockSize,uint32_t numberOfBlocks);
      bool eraseBlocks(uint32_t firstBlock,uint32_t lastBlock);

      bool waitForReceiveComplete();
      bool waitForTransmitComplete();
      void clearAllFlags() const;

      void setInitDivider(uint8_t divider);
      void setTransferDivider(uint8_t divider);

      const CardInfo& getCardInfo() const;
      uint32_t getCardType() const;

      SDCardState getCardState() const;
      SDTransferState getStatus() const;
  };


  /**
   * Set the initialisation clock divider
   * @param divider
   */

  inline void SdCardSdioFeature::setInitDivider(uint8_t divider) {
    _initDivider=divider;
  }


  /**
   * Set the initialisation clock divider
   * @param divider
   */

  inline void SdCardSdioFeature::setTransferDivider(uint8_t divider) {
    _transferDivider=divider;
  }


  /**
   * Clear all flags
   */

  inline void SdCardSdioFeature::clearAllFlags() const {
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);
  }


  /**
   * Return the card information structure
   * @return the reference
   */

  inline const SdCardSdioFeature::CardInfo& SdCardSdioFeature::getCardInfo() const {
    return _cardInfo;
  }


  /**
   * Get the card type
   * @return
   */

  inline uint32_t SdCardSdioFeature::getCardType() const {
    return _cardType;
  }
}
