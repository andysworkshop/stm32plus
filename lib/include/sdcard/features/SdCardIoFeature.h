/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Generic base class for the possible IO interfaces to the SD card
   */

  class SdCardIoFeature : public SdCardFeatureBase {

    public:
      struct SdCsd {
        volatile uint8_t  CSDStruct;            /*!< CSD structure */
        volatile uint8_t  SysSpecVersion;       /*!< System specification version */
        volatile uint8_t  Reserved1;            /*!< Reserved */
        volatile uint8_t  TAAC;                 /*!< Data read access-time 1 */
        volatile uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
        volatile uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
        volatile uint16_t CardComdClasses;      /*!< Card command classes */
        volatile uint8_t  RdBlockLen;           /*!< Max. read data block length */
        volatile uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
        volatile uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
        volatile uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
        volatile uint8_t  DSRImpl;              /*!< DSR implemented */
        volatile uint8_t  Reserved2;            /*!< Reserved */
        volatile uint32_t DeviceSize;           /*!< Device Size */
        volatile uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
        volatile uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
        volatile uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
        volatile uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
        volatile uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
        volatile uint8_t  EraseGrSize;          /*!< Erase group size */
        volatile uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
        volatile uint8_t  WrProtectGrSize;      /*!< Write protect group size */
        volatile uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
        volatile uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
        volatile uint8_t  WrSpeedFact;          /*!< Write speed factor */
        volatile uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
        volatile uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
        volatile uint8_t  Reserved3;            /*!< Reserded */
        volatile uint8_t  ContentProtectAppli;  /*!< Content protection application */
        volatile uint8_t  FileFormatGrouop;     /*!< File format group */
        volatile uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
        volatile uint8_t  PermWrProtect;        /*!< Permanent write protection */
        volatile uint8_t  TempWrProtect;        /*!< Temporary write protection */
        volatile uint8_t  FileFormat;           /*!< File Format */
        volatile uint8_t  ECC;                  /*!< ECC code */
        volatile uint8_t  CSD_CRC;              /*!< CSD CRC */
        volatile uint8_t  Reserved4;            /*!< always 1*/
    };

    struct SdCid {
      volatile uint8_t  ManufacturerID;       /*!< ManufacturerID */
      volatile uint16_t OEM_AppliID;          /*!< OEM/Application ID */
      volatile uint32_t ProdName1;            /*!< Product Name part1 */
      volatile uint8_t  ProdName2;            /*!< Product Name part2*/
      volatile uint8_t  ProdRev;              /*!< Product Revision */
      volatile uint32_t ProdSN;               /*!< Product Serial Number */
      volatile uint8_t  Reserved1;            /*!< Reserved1 */
      volatile uint16_t ManufactDate;         /*!< Manufacturing Date */
      volatile uint8_t  CID_CRC;              /*!< CID CRC */
      volatile uint8_t  Reserved2;            /*!< always 1 */
    };

    enum SD_Error {
      SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
      SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
      SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
      SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
      SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
      SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
      SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
      SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
      SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
      SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
      SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
      SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
      SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
      SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
      SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
      SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
      SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
      SD_CC_ERROR                        = (18), /*!< Internal card controller error */
      SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
      SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
      SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
      SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
      SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
      SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
      SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
      SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
      SD_INVALID_VOLTRANGE               = (27),
      SD_ADDR_OUT_OF_RANGE               = (28),
      SD_SWITCH_ERROR                    = (29),
      SD_SDIO_DISABLED                   = (30),
      SD_SDIO_FUNCTION_BUSY              = (31),
      SD_SDIO_FUNCTION_FAILED            = (32),
      SD_SDIO_UNKNOWN_FUNCTION           = (33),

    /**
      * @brief  Standard error defines
      */

      SD_INTERNAL_ERROR,
      SD_NOT_CONFIGURED,
      SD_REQUEST_PENDING,
      SD_REQUEST_NOT_APPLICABLE,
      SD_INVALID_PARAMETER,
      SD_UNSUPPORTED_FEATURE,
      SD_UNSUPPORTED_HW,
      SD_ERROR,
      SD_OK = 0
    };

    enum SDTransferState {
      SD_TRANSFER_OK  = 0,
      SD_TRANSFER_BUSY = 1,
      SD_TRANSFER_ERROR
    };

    enum SDCardState {
      SD_CARD_READY                  = ((uint32_t)0x00000001),
      SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
      SD_CARD_STANDBY                = ((uint32_t)0x00000003),
      SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
      SD_CARD_SENDING                = ((uint32_t)0x00000005),
      SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
      SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
      SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
      SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
    };

    enum {
      SD_CMD_GO_IDLE_STATE                       =0,
      SD_CMD_SEND_OP_COND                        =1,
      SD_CMD_ALL_SEND_CID                        =2,
      SD_CMD_SET_REL_ADDR                        =3, /*!< SDIO_SEND_REL_ADDR for SD Card */
      SD_CMD_SET_DSR                             =4,
      SD_CMD_SDIO_SEN_OP_COND                    =5,
      SD_CMD_HS_SWITCH                           =6,
      SD_CMD_SEL_DESEL_CARD                      =7,
      SD_CMD_HS_SEND_EXT_CSD                     =8,
      SD_CMD_SEND_CSD                            =9,
      SD_CMD_SEND_CID                            =10,
      SD_CMD_READ_DAT_UNTIL_STOP                 =11, /*!< SD Card doesn't support it */
      SD_CMD_STOP_TRANSMISSION                   =12,
      SD_CMD_SEND_STATUS                         =13,
      SD_CMD_HS_BUSTEST_READ                     =14,
      SD_CMD_GO_INACTIVE_STATE                   =15,
      SD_CMD_SET_BLOCKLEN                        =16,
      SD_CMD_READ_SINGLE_BLOCK                   =17,
      SD_CMD_READ_MULT_BLOCK                     =18,
      SD_CMD_HS_BUSTEST_WRITE                    =19,
      SD_CMD_WRITE_DAT_UNTIL_STOP                =20, /*!< SD Card doesn't support it */
      SD_CMD_SET_BLOCK_COUNT                     =23, /*!< SD Card doesn't support it */
      SD_CMD_WRITE_SINGLE_BLOCK                  =24,
      SD_CMD_WRITE_MULT_BLOCK                    =25,
      SD_CMD_PROG_CID                            =26, /*!< reserved for manufacturers */
      SD_CMD_PROG_CSD                            =27,
      SD_CMD_SET_WRITE_PROT                      =28,
      SD_CMD_CLR_WRITE_PROT                      =29,
      SD_CMD_SEND_WRITE_PROT                     =30,
      SD_CMD_SD_ERASE_GRP_START                  =32, /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only, */
      SD_CMD_SD_ERASE_GRP_END                    =33, /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only, */
      SD_CMD_ERASE_GRP_START                     =35, /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31, */

      SD_CMD_ERASE_GRP_END                       =36, /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31, */

      SD_CMD_ERASE                               =38,
      SD_CMD_FAST_IO                             =39, /*!< SD Card doesn't support it */
      SD_CMD_GO_IRQ_STATE                        =40, /*!< SD Card doesn't support it */
      SD_CMD_LOCK_UNLOCK                         =42,
      SD_CMD_APP_CMD                             =55,
      SD_CMD_GEN_CMD                             =56,
      SD_CMD_NO_CMD                              =64,

      /**
       * @brief Following commands are SD Card Specific commands.
       *        SDIO_APP_CMD should be sent before sending these commands.
       */
      SD_CMD_APP_SD_SET_BUSWIDTH                 =6,  /*!< For SD Card only */
      SD_CMD_SD_APP_STAUS                        =13, /*!< For SD Card only */
      SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        =22, /*!< For SD Card only */
      SD_CMD_SD_APP_OP_COND                      =41, /*!< For SD Card only */
      SD_CMD_SD_APP_SET_CLR_CARD_DETECT          =42, /*!< For SD Card only */
      SD_CMD_SD_APP_SEND_SCR                     =51, /*!< For SD Card only */
      SD_CMD_SDIO_RW_DIRECT                      =52, /*!< For SD I/O Card only */
      SD_CMD_SDIO_RW_EXTENDED                    =53, /*!< For SD I/O Card only */

      /**
       * @brief Following commands are SD Card Specific security commands.
       *        SDIO_APP_CMD should be sent before sending these commands.
       */
      SD_CMD_SD_APP_GET_MKB                      =43, /*!< For SD Card only */
      SD_CMD_SD_APP_GET_MID                      =44, /*!< For SD Card only */
      SD_CMD_SD_APP_SET_CER_RN1                  =45, /*!< For SD Card only */
      SD_CMD_SD_APP_GET_CER_RN2                  =46, /*!< For SD Card only */
      SD_CMD_SD_APP_SET_CER_RES2                 =47, /*!< For SD Card only */
      SD_CMD_SD_APP_GET_CER_RES1                 =48, /*!< For SD Card only */
      SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   =18, /*!< For SD Card only */
      SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  =25, /*!< For SD Card only */
      SD_CMD_SD_APP_SECURE_ERASE                 =38, /*!< For SD Card only */
      SD_CMD_SD_APP_CHANGE_SECURE_AREA           =49, /*!< For SD Card only */
      SD_CMD_SD_APP_SECURE_WRITE_MKB             =48 /*!< For SD Card only */
    };

    enum {
      SDIO_STD_CAPACITY_SD_CARD_V1_1             =((uint32_t)0x00000000),
      SDIO_STD_CAPACITY_SD_CARD_V2_0             =((uint32_t)0x00000001),
      SDIO_HIGH_CAPACITY_SD_CARD                 =((uint32_t)0x00000002),
      SDIO_MULTIMEDIA_CARD                       =((uint32_t)0x00000003),
      SDIO_SECURE_DIGITAL_IO_CARD                =((uint32_t)0x00000004),
      SDIO_HIGH_SPEED_MULTIMEDIA_CARD            =((uint32_t)0x00000005),
      SDIO_SECURE_DIGITAL_IO_COMBO_CARD          =((uint32_t)0x00000006),
      SDIO_HIGH_CAPACITY_MMC_CARD                =((uint32_t)0x00000007)
    };

    protected:
      SdCardIoFeature(SdCardBase& sdcard)
        : SdCardFeatureBase(sdcard) {
      }
  };
}
