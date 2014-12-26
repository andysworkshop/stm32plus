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

  SdCardSdioFeature::SdCardSdioFeature(SdCardBase& sdcard)
    : SdCardIoFeature(sdcard) {

    // clock on for the SDIO peripheral

    ClockControl<PERIPHERAL_SDIO>::On();

    // all pins are AFIO and they're all on the same AF ID for F1,F2,F4. SDIO is
    // not present on the F3.

    // clock (no pullup)

    GpioPinInitialiser::initialise(
        GPIOC,
        GPIO_Pin_12,        // SDIO_SCK
        Gpio::ALTERNATE_FUNCTION,
        (GPIOSpeed_TypeDef)SdioPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_NONE,
        Gpio::PUSH_PULL,
        GpioAlternateFunctionMapper<PERIPHERAL_SDIO,GPIOC_BASE,GPIO_Pin_12>::GPIO_AF);

    // data lines, pullup

    GpioPinInitialiser::initialise(
        GPIOC,
        GPIO_Pin_8 |        // DO
        GPIO_Pin_9 |        // D1
        GPIO_Pin_10 |       // D2
        GPIO_Pin_11,        // D3
        Gpio::ALTERNATE_FUNCTION,
        (GPIOSpeed_TypeDef)SdioPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_UP,
        Gpio::PUSH_PULL,
        GpioAlternateFunctionMapper<PERIPHERAL_SDIO,GPIOC_BASE,GPIO_Pin_8>::GPIO_AF);

    // CMD, pullup

    GpioPinInitialiser::initialise(
        GPIOD,
        GPIO_Pin_2,         // SDIO_CMD
        Gpio::ALTERNATE_FUNCTION,
        (GPIOSpeed_TypeDef)SdioPeripheralTraits::GPIO_SPEED,
        Gpio::PUPD_UP,
        Gpio::PUSH_PULL,
        GpioAlternateFunctionMapper<PERIPHERAL_SDIO,GPIOD_BASE,GPIO_Pin_2>::GPIO_AF);

    // detect the init and IO dividers

    detectDividers();

    // default card type

    _cardType=SDIO_STD_CAPACITY_SD_CARD_V1_1;
    _rca=0;
  }


  /**
   * The two clock maximum clock frequencies are 200kHz for init and 24Mhz for
   * transfer.
   */

  void SdCardSdioFeature::detectDividers() {

    uint32_t sdioBusSpeed;

    sdioBusSpeed=PeripheralBusSpeed<PERIPHERAL_SDIO>::getBusSpeed();

    // set the init clk speed to the max of 400kHz. we used to attempt 200kHz which
    // would fail due to overflow on the F1. sdioBusSpeed (SDIOCLK) is HCLK (e.g. 72MHz)
    // on the F1 and a fixed 48MHz on the F2/F4

    _initDivider=(sdioBusSpeed/400000L)-2;

    // transfer divider is calculated to be a fixed 24MHz (1MHz off the 25MHz maximum for SDIO)

    _transferDivider=(sdioBusSpeed/24000000L)-2;
  }


  /**
   * Do the power on sequence
   * @return true if it succeeded
   */

  bool SdCardSdioFeature::powerOn() {

    SD_Error errorstatus=SD_OK;
    uint32_t response=0,count=0,validvoltage=0;
    uint32_t SDType=SD_STD_CAPACITY;
    SDIO_InitTypeDef sdioInit;
    SDIO_CmdInitTypeDef cmdInit;

    /*!< Power ON Sequence -----------------------------------------------------*/

    sdioInit.SDIO_ClockDiv=_initDivider;
    sdioInit.SDIO_ClockEdge=SDIO_ClockEdge_Rising;
    sdioInit.SDIO_ClockBypass=SDIO_ClockBypass_Disable;
    sdioInit.SDIO_ClockPowerSave=SDIO_ClockPowerSave_Disable;
    sdioInit.SDIO_BusWide=SDIO_BusWide_1b;
    sdioInit.SDIO_HardwareFlowControl=SDIO_HardwareFlowControl_Disable;
    SDIO_Init(&sdioInit);

    /*!< Set Power State to ON */
    SDIO_SetPowerState(SDIO_PowerState_ON);

    /*!< Enable SDIO Clock */
    SDIO_ClockCmd(ENABLE);

    /*!< CMD0: GO_IDLE_STATE ---------------------------------------------------*/
    /*!< No CMD response required */
    cmdInit.SDIO_Argument=0x0;
    cmdInit.SDIO_CmdIndex=SD_CMD_GO_IDLE_STATE;
    cmdInit.SDIO_Response=SDIO_Response_No;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdError();

    if(errorstatus != SD_OK)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    /*!< CMD8: SEND_IF_COND ----------------------------------------------------*/
    /*!< Send CMD8 to verify SD card interface operating condition */
    /*!< Argument: - [31:12]: Reserved (shall be set to '0')
     - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
     - [7:0]: Check Pattern (recommended 0xAA) */
    /*!< CMD Response: R7 */

    cmdInit.SDIO_Argument=SD_CHECK_PATTERN;
    cmdInit.SDIO_CmdIndex=SDIO_SEND_IF_COND;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp7Error();

    if(errorstatus == SD_OK) {
      _cardType=SDIO_STD_CAPACITY_SD_CARD_V2_0; /*!< SD Card 2.0 */
      SDType=SD_HIGH_CAPACITY;
    } else {
      /*!< CMD55 */
      cmdInit.SDIO_Argument=0x00;
      cmdInit.SDIO_CmdIndex=SD_CMD_APP_CMD;
      cmdInit.SDIO_Response=SDIO_Response_Short;
      cmdInit.SDIO_Wait=SDIO_Wait_No;
      cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
      SDIO_SendCommand(&cmdInit);
      errorstatus=cmdResp1Error(SD_CMD_APP_CMD);
    }
    /*!< CMD55 */
    cmdInit.SDIO_Argument=0x00;
    cmdInit.SDIO_CmdIndex=SD_CMD_APP_CMD;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);
    errorstatus=cmdResp1Error(SD_CMD_APP_CMD);

    /*!< If errorstatus is Command TimeOut, it is a MMC card */
    /*!< If errorstatus is SD_OK it is a SD card: SD card 2.0 (voltage range mismatch)
     or SD card 1.x */
    if(errorstatus == SD_OK) {
      /*!< SD CARD */
      /*!< Send ACMD41 SD_APP_OP_COND with Argument 0x80100000 */
      while((!validvoltage) && (count < SD_MAX_VOLT_TRIAL)) {

        /*!< SEND CMD55 APP_CMD with RCA as 0 */
        cmdInit.SDIO_Argument=0x00;
        cmdInit.SDIO_CmdIndex=SD_CMD_APP_CMD;
        cmdInit.SDIO_Response=SDIO_Response_Short;
        cmdInit.SDIO_Wait=SDIO_Wait_No;
        cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
        SDIO_SendCommand(&cmdInit);

        errorstatus=cmdResp1Error(SD_CMD_APP_CMD);

        if(errorstatus != SD_OK)
          return errorstatus;

        cmdInit.SDIO_Argument=SD_VOLTAGE_WINDOW_SD | SDType;
        cmdInit.SDIO_CmdIndex=SD_CMD_SD_APP_OP_COND;
        cmdInit.SDIO_Response=SDIO_Response_Short;
        cmdInit.SDIO_Wait=SDIO_Wait_No;
        cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
        SDIO_SendCommand(&cmdInit);

        errorstatus=cmdResp3Error();
        if(errorstatus != SD_OK)
          return errorstatus;

        response=SDIO_GetResponse(SDIO_RESP1);
        validvoltage=(((response >> 31) == 1) ? 1 : 0);
        count++;
      }
      if(count >= SD_MAX_VOLT_TRIAL) {
        errorstatus=SD_INVALID_VOLTRANGE;
        return errorstatus;
      }

      if(response&=SD_HIGH_CAPACITY)
        _cardType=SDIO_HIGH_CAPACITY_SD_CARD;

    }/*!< else MMC Card */

    if(errorstatus != SD_OK)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    return true;
  }


  /**
  * @brief  Turns the SDIO output signals off.
  * @param  None
  */

  void SdCardSdioFeature::powerOff() {
    SDIO_SetPowerState(SDIO_PowerState_OFF);
  }


  /**
    * @brief  Intialises all cards or single card as the case may be Card(s) come
    *         into standby state.
    * @param  None
    * @retval true if it succeeded
    */

  bool SdCardSdioFeature::initialiseCard() {

    SD_Error errorstatus=SD_OK;
    uint16_t rca=0x01;
    SDIO_CmdInitTypeDef cmdInit;
    SDIO_InitTypeDef sdioInit;

    if(SDIO_GetPowerState() == SDIO_PowerState_OFF)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,SD_REQUEST_NOT_APPLICABLE);

    if(SDIO_SECURE_DIGITAL_IO_CARD != _cardType) {

      /*!< Send CMD2 ALL_SEND_CID */
      cmdInit.SDIO_Argument=0x0;
      cmdInit.SDIO_CmdIndex=SD_CMD_ALL_SEND_CID;
      cmdInit.SDIO_Response=SDIO_Response_Long;
      cmdInit.SDIO_Wait=SDIO_Wait_No;
      cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
      SDIO_SendCommand(&cmdInit);

      errorstatus=cmdResp2Error();

      if(SD_OK != errorstatus)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

      _cidTab[0]=SDIO_GetResponse(SDIO_RESP1);
      _cidTab[1]=SDIO_GetResponse(SDIO_RESP2);
      _cidTab[2]=SDIO_GetResponse(SDIO_RESP3);
      _cidTab[3]=SDIO_GetResponse(SDIO_RESP4);
    }
    if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == _cardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == _cardType) || (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == _cardType) || (SDIO_HIGH_CAPACITY_SD_CARD == _cardType)) {
      /*!< Send CMD3 SET_REL_ADDR with argument 0 */
      /*!< SD Card publishes its RCA. */
      cmdInit.SDIO_Argument=0x00;
      cmdInit.SDIO_CmdIndex=SD_CMD_SET_REL_ADDR;
      cmdInit.SDIO_Response=SDIO_Response_Short;
      cmdInit.SDIO_Wait=SDIO_Wait_No;
      cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
      SDIO_SendCommand(&cmdInit);

      errorstatus=cmdResp6Error(SD_CMD_SET_REL_ADDR,&rca);

      if(SD_OK != errorstatus)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);
    }

    if(SDIO_SECURE_DIGITAL_IO_CARD != _cardType) {

      _rca=rca;

      /*!< Send CMD9 SEND_CSD with argument as card's RCA */
      cmdInit.SDIO_Argument=(uint32_t)(rca << 16);
      cmdInit.SDIO_CmdIndex=SD_CMD_SEND_CSD;
      cmdInit.SDIO_Response=SDIO_Response_Long;
      cmdInit.SDIO_Wait=SDIO_Wait_No;
      cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
      SDIO_SendCommand(&cmdInit);

      errorstatus=cmdResp2Error();

      if(SD_OK != errorstatus)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

      _csdTab[0]=SDIO_GetResponse(SDIO_RESP1);
      _csdTab[1]=SDIO_GetResponse(SDIO_RESP2);
      _csdTab[2]=SDIO_GetResponse(SDIO_RESP3);
      _csdTab[3]=SDIO_GetResponse(SDIO_RESP4);
    }

    /*!< Configure the SDIO peripheral */

    sdioInit.SDIO_ClockDiv = _transferDivider;
    sdioInit.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    sdioInit.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
    sdioInit.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    sdioInit.SDIO_BusWide = SDIO_BusWide_1b;
    sdioInit.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
    SDIO_Init(&sdioInit);

    return detectCardInfo() &&
           selectDeselect((uint32_t) (_cardInfo.RCA << 16)) &&
           enableWideBusOperation(SDIO_BusWide_4b);
  }


  /**
   * @brief  Returns information about specific card.
   * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD card
   *         information.
   * @retval SD_Error: SD Card Error code.
   */

  bool SdCardSdioFeature::detectCardInfo() {

    SD_Error errorstatus=SD_OK;
    uint8_t tmp=0;

    _cardInfo.CardType=(uint8_t)_cardType;
    _cardInfo.RCA=(uint16_t)_rca;

    /*!< Byte 0 */
    tmp=(uint8_t)((_csdTab[0] & 0xFF000000) >> 24);
    _cardInfo.SD_csd.CSDStruct=(tmp & 0xC0) >> 6;
    _cardInfo.SD_csd.SysSpecVersion=(tmp & 0x3C) >> 2;
    _cardInfo.SD_csd.Reserved1=tmp & 0x03;

    /*!< Byte 1 */
    tmp=(uint8_t)((_csdTab[0] & 0x00FF0000) >> 16);
    _cardInfo.SD_csd.TAAC=tmp;

    /*!< Byte 2 */
    tmp=(uint8_t)((_csdTab[0] & 0x0000FF00) >> 8);
    _cardInfo.SD_csd.NSAC=tmp;

    /*!< Byte 3 */
    tmp=(uint8_t)(_csdTab[0] & 0x000000FF);
    _cardInfo.SD_csd.MaxBusClkFrec=tmp;

    /*!< Byte 4 */
    tmp=(uint8_t)((_csdTab[1] & 0xFF000000) >> 24);
    _cardInfo.SD_csd.CardComdClasses=tmp << 4;

    /*!< Byte 5 */
    tmp=(uint8_t)((_csdTab[1] & 0x00FF0000) >> 16);
    _cardInfo.SD_csd.CardComdClasses|=(tmp & 0xF0) >> 4;
    _cardInfo.SD_csd.RdBlockLen=tmp & 0x0F;

    /*!< Byte 6 */
    tmp=(uint8_t)((_csdTab[1] & 0x0000FF00) >> 8);
    _cardInfo.SD_csd.PartBlockRead=(tmp & 0x80) >> 7;
    _cardInfo.SD_csd.WrBlockMisalign=(tmp & 0x40) >> 6;
    _cardInfo.SD_csd.RdBlockMisalign=(tmp & 0x20) >> 5;
    _cardInfo.SD_csd.DSRImpl=(tmp & 0x10) >> 4;
    _cardInfo.SD_csd.Reserved2=0; /*!< Reserved */

    if((_cardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (_cardType == SDIO_STD_CAPACITY_SD_CARD_V2_0)) {
      _cardInfo.SD_csd.DeviceSize=(tmp & 0x03) << 10;

      /*!< Byte 7 */
      tmp=(uint8_t)(_csdTab[1] & 0x000000FF);
      _cardInfo.SD_csd.DeviceSize|=(tmp) << 2;

      /*!< Byte 8 */
      tmp=(uint8_t)((_csdTab[2] & 0xFF000000) >> 24);
      _cardInfo.SD_csd.DeviceSize|=(tmp & 0xC0) >> 6;

      _cardInfo.SD_csd.MaxRdCurrentVDDMin=(tmp & 0x38) >> 3;
      _cardInfo.SD_csd.MaxRdCurrentVDDMax=(tmp & 0x07);

      /*!< Byte 9 */
      tmp=(uint8_t)((_csdTab[2] & 0x00FF0000) >> 16);
      _cardInfo.SD_csd.MaxWrCurrentVDDMin=(tmp & 0xE0) >> 5;
      _cardInfo.SD_csd.MaxWrCurrentVDDMax=(tmp & 0x1C) >> 2;
      _cardInfo.SD_csd.DeviceSizeMul=(tmp & 0x03) << 1;
      /*!< Byte 10 */
      tmp=(uint8_t)((_csdTab[2] & 0x0000FF00) >> 8);
      _cardInfo.SD_csd.DeviceSizeMul|=(tmp & 0x80) >> 7;

      _cardInfo.CardCapacity=(_cardInfo.SD_csd.DeviceSize + 1);
      _cardInfo.CardCapacity*=(1 << (_cardInfo.SD_csd.DeviceSizeMul + 2));
      _cardInfo.CardBlockSize=1 << (_cardInfo.SD_csd.RdBlockLen);
      _cardInfo.CardCapacity*=_cardInfo.CardBlockSize;
    } else if(_cardType == SDIO_HIGH_CAPACITY_SD_CARD) {
      /*!< Byte 7 */
      tmp=(uint8_t)(_csdTab[1] & 0x000000FF);
      _cardInfo.SD_csd.DeviceSize=(tmp & 0x3F) << 16;

      /*!< Byte 8 */
      tmp=(uint8_t)((_csdTab[2] & 0xFF000000) >> 24);

      _cardInfo.SD_csd.DeviceSize|=(tmp << 8);

      /*!< Byte 9 */
      tmp=(uint8_t)((_csdTab[2] & 0x00FF0000) >> 16);

      _cardInfo.SD_csd.DeviceSize|=(tmp);

      /*!< Byte 10 */
      tmp=(uint8_t)((_csdTab[2] & 0x0000FF00) >> 8);

      _cardInfo.CardCapacity=(_cardInfo.SD_csd.DeviceSize + 1) * 512ULL * 1024ULL;
      _cardInfo.CardBlockSize=512;
    }

    _cardInfo.SD_csd.EraseGrSize=(tmp & 0x40) >> 6;
    _cardInfo.SD_csd.EraseGrMul=(tmp & 0x3F) << 1;

    /*!< Byte 11 */
    tmp=(uint8_t)(_csdTab[2] & 0x000000FF);
    _cardInfo.SD_csd.EraseGrMul|=(tmp & 0x80) >> 7;
    _cardInfo.SD_csd.WrProtectGrSize=(tmp & 0x7F);

    /*!< Byte 12 */
    tmp=(uint8_t)((_csdTab[3] & 0xFF000000) >> 24);
    _cardInfo.SD_csd.WrProtectGrEnable=(tmp & 0x80) >> 7;
    _cardInfo.SD_csd.ManDeflECC=(tmp & 0x60) >> 5;
    _cardInfo.SD_csd.WrSpeedFact=(tmp & 0x1C) >> 2;
    _cardInfo.SD_csd.MaxWrBlockLen=(tmp & 0x03) << 2;

    /*!< Byte 13 */
    tmp=(uint8_t)((_csdTab[3] & 0x00FF0000) >> 16);
    _cardInfo.SD_csd.MaxWrBlockLen|=(tmp & 0xC0) >> 6;
    _cardInfo.SD_csd.WriteBlockPaPartial=(tmp & 0x20) >> 5;
    _cardInfo.SD_csd.Reserved3=0;
    _cardInfo.SD_csd.ContentProtectAppli=(tmp & 0x01);

    /*!< Byte 14 */
    tmp=(uint8_t)((_csdTab[3] & 0x0000FF00) >> 8);
    _cardInfo.SD_csd.FileFormatGrouop=(tmp & 0x80) >> 7;
    _cardInfo.SD_csd.CopyFlag=(tmp & 0x40) >> 6;
    _cardInfo.SD_csd.PermWrProtect=(tmp & 0x20) >> 5;
    _cardInfo.SD_csd.TempWrProtect=(tmp & 0x10) >> 4;
    _cardInfo.SD_csd.FileFormat=(tmp & 0x0C) >> 2;
    _cardInfo.SD_csd.ECC=(tmp & 0x03);

    /*!< Byte 15 */
    tmp=(uint8_t)(_csdTab[3] & 0x000000FF);
    _cardInfo.SD_csd.CSD_CRC=(tmp & 0xFE) >> 1;
    _cardInfo.SD_csd.Reserved4=1;

    /*!< Byte 0 */
    tmp=(uint8_t)((_cidTab[0] & 0xFF000000) >> 24);
    _cardInfo.SD_cid.ManufacturerID=tmp;

    /*!< Byte 1 */
    tmp=(uint8_t)((_cidTab[0] & 0x00FF0000) >> 16);
    _cardInfo.SD_cid.OEM_AppliID=tmp << 8;

    /*!< Byte 2 */
    tmp=(uint8_t)((_cidTab[0] & 0x000000FF00) >> 8);
    _cardInfo.SD_cid.OEM_AppliID|=tmp;

    /*!< Byte 3 */
    tmp=(uint8_t)(_cidTab[0] & 0x000000FF);
    _cardInfo.SD_cid.ProdName1=tmp << 24;

    /*!< Byte 4 */
    tmp=(uint8_t)((_cidTab[1] & 0xFF000000) >> 24);
    _cardInfo.SD_cid.ProdName1|=tmp << 16;

    /*!< Byte 5 */
    tmp=(uint8_t)((_cidTab[1] & 0x00FF0000) >> 16);
    _cardInfo.SD_cid.ProdName1|=tmp << 8;

    /*!< Byte 6 */
    tmp=(uint8_t)((_cidTab[1] & 0x0000FF00) >> 8);
    _cardInfo.SD_cid.ProdName1|=tmp;

    /*!< Byte 7 */
    tmp=(uint8_t)(_cidTab[1] & 0x000000FF);
    _cardInfo.SD_cid.ProdName2=tmp;

    /*!< Byte 8 */
    tmp=(uint8_t)((_cidTab[2] & 0xFF000000) >> 24);
    _cardInfo.SD_cid.ProdRev=tmp;

    /*!< Byte 9 */
    tmp=(uint8_t)((_cidTab[2] & 0x00FF0000) >> 16);
    _cardInfo.SD_cid.ProdSN=tmp << 24;

    /*!< Byte 10 */
    tmp=(uint8_t)((_cidTab[2] & 0x0000FF00) >> 8);
    _cardInfo.SD_cid.ProdSN|=tmp << 16;

    /*!< Byte 11 */
    tmp=(uint8_t)(_cidTab[2] & 0x000000FF);
    _cardInfo.SD_cid.ProdSN|=tmp << 8;

    /*!< Byte 12 */
    tmp=(uint8_t)((_cidTab[3] & 0xFF000000) >> 24);
    _cardInfo.SD_cid.ProdSN|=tmp;

    /*!< Byte 13 */
    tmp=(uint8_t)((_cidTab[3] & 0x00FF0000) >> 16);
    _cardInfo.SD_cid.Reserved1|=(tmp & 0xF0) >> 4;
    _cardInfo.SD_cid.ManufactDate=(tmp & 0x0F) << 8;

    /*!< Byte 14 */
    tmp=(uint8_t)((_cidTab[3] & 0x0000FF00) >> 8);
    _cardInfo.SD_cid.ManufactDate|=tmp;

    /*!< Byte 15 */
    tmp=(uint8_t)(_cidTab[3] & 0x000000FF);
    _cardInfo.SD_cid.CID_CRC=(tmp & 0xFE) >> 1;
    _cardInfo.SD_cid.Reserved2=1;

    if(errorstatus != SD_OK)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    return true;
  }

  /**
   * @brief  Selects od Deselects the corresponding card.
   * @param  addr: Address of the Card to be selected.
   * @retval true if it worked
   */

  bool SdCardSdioFeature::selectDeselect(uint32_t addr) {

    SD_Error errorstatus=SD_OK;
    SDIO_CmdInitTypeDef cmdInit;

    /*!< Send CMD7 SDIO_SEL_DESEL_CARD */
    cmdInit.SDIO_Argument=addr;
    cmdInit.SDIO_CmdIndex=SD_CMD_SEL_DESEL_CARD;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;

    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_SEL_DESEL_CARD);

    if(errorstatus != SD_OK)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    return true;
  }

  /**
   * @brief  Enables wide bus opeartion for the requeseted card if supported by
   *         card.
   * @param  WideMode: Specifies the SD card wide bus mode.
   *   This parameter can be one of the following values:
   *     @arg SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
   *     @arg SDIO_BusWide_4b: 4-bit data transfer
   *     @arg SDIO_BusWide_1b: 1-bit data transfer
   * @retval true if it worked
   */

  bool SdCardSdioFeature::enableWideBusOperation(uint32_t WideMode) {

    SD_Error errorstatus=SD_OK;
    SDIO_InitTypeDef sdioInit;

    /*!< MMC Card doesn't support this feature */
    if(SDIO_MULTIMEDIA_CARD == _cardType) {
      errorstatus=SD_UNSUPPORTED_FEATURE;
      return errorstatus;
    } else if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == _cardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == _cardType) || (SDIO_HIGH_CAPACITY_SD_CARD == _cardType)) {
      if(SDIO_BusWide_8b == WideMode) {
        errorstatus=SD_UNSUPPORTED_FEATURE;
        return errorstatus;
      } else if(SDIO_BusWide_4b == WideMode) {
        errorstatus=enableWideBus(ENABLE);

        if(SD_OK == errorstatus) {
          /*!< Configure the SDIO peripheral */
          sdioInit.SDIO_ClockDiv=_transferDivider;
          sdioInit.SDIO_ClockEdge=SDIO_ClockEdge_Rising;
          sdioInit.SDIO_ClockBypass=SDIO_ClockBypass_Disable;
          sdioInit.SDIO_ClockPowerSave=SDIO_ClockPowerSave_Disable;
          sdioInit.SDIO_BusWide=SDIO_BusWide_4b;
          sdioInit.SDIO_HardwareFlowControl=SDIO_HardwareFlowControl_Disable;
          SDIO_Init(&sdioInit);
        }
      } else {
        errorstatus=enableWideBus(DISABLE);

        if(SD_OK == errorstatus) {
          /*!< Configure the SDIO peripheral */
          sdioInit.SDIO_ClockDiv=_transferDivider;
          sdioInit.SDIO_ClockEdge=SDIO_ClockEdge_Rising;
          sdioInit.SDIO_ClockBypass=SDIO_ClockBypass_Disable;
          sdioInit.SDIO_ClockPowerSave=SDIO_ClockPowerSave_Disable;
          sdioInit.SDIO_BusWide=SDIO_BusWide_1b;
          sdioInit.SDIO_HardwareFlowControl=SDIO_HardwareFlowControl_Disable;
          SDIO_Init(&sdioInit);
        }
      }
    }

    if(errorstatus != SD_OK)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    return true;
  }


  /**
   * @brief  Enables or disables the SDIO wide bus mode.
   * @param  NewState: new state of the SDIO wide bus mode.
   *   This parameter can be: ENABLE or DISABLE.
   * @retval SD_Error: SD Card Error code.
   */

  SdCardIoFeature::SD_Error SdCardSdioFeature::enableWideBus(FunctionalState NewState) {

    SD_Error errorstatus=SD_OK;
    SDIO_CmdInitTypeDef cmdInit;
    uint32_t scr[2]= { 0,0 };

    if(SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED) {
      errorstatus=SD_LOCK_UNLOCK_FAILED;
      return errorstatus;
    }

    /*!< Get SCR Register */
    errorstatus=findSCR(scr);

    if(errorstatus != SD_OK) {
      return errorstatus;
    }

    /*!< If wide bus operation to be enabled */
    if(NewState == ENABLE) {
      /*!< If requested card supports wide bus operation */
      if((scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO) {
        /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
        cmdInit.SDIO_Argument=(uint32_t)_rca << 16;
        cmdInit.SDIO_CmdIndex=SD_CMD_APP_CMD;
        cmdInit.SDIO_Response=SDIO_Response_Short;
        cmdInit.SDIO_Wait=SDIO_Wait_No;
        cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
        SDIO_SendCommand(&cmdInit);

        errorstatus=cmdResp1Error(SD_CMD_APP_CMD);

        if(errorstatus != SD_OK) {
          return errorstatus;
        }

        /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
        cmdInit.SDIO_Argument=0x2;
        cmdInit.SDIO_CmdIndex=SD_CMD_APP_SD_SET_BUSWIDTH;
        cmdInit.SDIO_Response=SDIO_Response_Short;
        cmdInit.SDIO_Wait=SDIO_Wait_No;
        cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
        SDIO_SendCommand(&cmdInit);

        errorstatus=cmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);

        if(errorstatus != SD_OK) {
          return errorstatus;
        }
        return errorstatus;
      } else {
        errorstatus=SD_REQUEST_NOT_APPLICABLE;
        return errorstatus;
      }
    } /*!< If wide bus operation to be disabled */
    else {
      /*!< If requested card supports 1 bit mode operation */
      if((scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO) {
        /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
        cmdInit.SDIO_Argument=(uint32_t)_rca << 16;
        cmdInit.SDIO_CmdIndex=SD_CMD_APP_CMD;
        cmdInit.SDIO_Response=SDIO_Response_Short;
        cmdInit.SDIO_Wait=SDIO_Wait_No;
        cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
        SDIO_SendCommand(&cmdInit);

        errorstatus=cmdResp1Error(SD_CMD_APP_CMD);

        if(errorstatus != SD_OK) {
          return errorstatus;
        }

        /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
        cmdInit.SDIO_Argument=0x00;
        cmdInit.SDIO_CmdIndex=SD_CMD_APP_SD_SET_BUSWIDTH;
        cmdInit.SDIO_Response=SDIO_Response_Short;
        cmdInit.SDIO_Wait=SDIO_Wait_No;
        cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
        SDIO_SendCommand(&cmdInit);

        errorstatus=cmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);

        if(errorstatus != SD_OK)
          return errorstatus;

        return errorstatus;
      } else {
        errorstatus=SD_REQUEST_NOT_APPLICABLE;
        return errorstatus;
      }
    }
  }


  /**
   * @brief  Find the SD card SCR register value.
   * @param  rca: selected card address.
   * @param  pscr: pointer to the buffer that will contain the SCR value.
   * @retval SD_Error: SD Card Error code.
   */

  SdCardIoFeature::SD_Error SdCardSdioFeature::findSCR(uint32_t *pscr) {

    uint32_t index=0;
    SD_Error errorstatus=SD_OK;
    SDIO_CmdInitTypeDef cmdInit;
    SDIO_DataInitTypeDef dataInit;

    uint32_t tempscr[2]= { 0,0 };

    /*!< Set Block Size To 8 Bytes */
    /*!< Send CMD55 APP_CMD with argument as card's RCA */
    cmdInit.SDIO_Argument=(uint32_t)8;
    cmdInit.SDIO_CmdIndex=SD_CMD_SET_BLOCKLEN;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_SET_BLOCKLEN);

    if(errorstatus != SD_OK) {
      return errorstatus;
    }

    /*!< Send CMD55 APP_CMD with argument as card's RCA */
    cmdInit.SDIO_Argument=(uint32_t)_rca << 16;
    cmdInit.SDIO_CmdIndex=SD_CMD_APP_CMD;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_APP_CMD);

    if(errorstatus != SD_OK) {
      return errorstatus;
    }
    dataInit.SDIO_DataTimeOut=SD_DATATIMEOUT;
    dataInit.SDIO_DataLength=8;
    dataInit.SDIO_DataBlockSize=SDIO_DataBlockSize_8b;
    dataInit.SDIO_TransferDir=SDIO_TransferDir_ToSDIO;
    dataInit.SDIO_TransferMode=SDIO_TransferMode_Block;
    dataInit.SDIO_DPSM=SDIO_DPSM_Enable;
    SDIO_DataConfig(&dataInit);

    /*!< Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
    cmdInit.SDIO_Argument=0x0;
    cmdInit.SDIO_CmdIndex=SD_CMD_SD_APP_SEND_SCR;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_SD_APP_SEND_SCR);

    if(errorstatus != SD_OK) {
      return errorstatus;
    }

    while(!(SDIO->STA & (SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))) {
      if(SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) {
        *(tempscr + index)=SDIO_ReadData();
        index++;
      }
    }

    if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
      SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
      errorstatus=SD_DATA_TIMEOUT;
      return errorstatus;
    } else if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
      SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
      errorstatus=SD_DATA_CRC_FAIL;
      return errorstatus;
    } else if(SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) {
      SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
      errorstatus=SD_RX_OVERRUN;
      return errorstatus;
    } else if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
      SDIO_ClearFlag(SDIO_FLAG_STBITERR);
      errorstatus=SD_START_BIT_ERR;
      return errorstatus;
    }

    /*!< Clear all the static flags */
    SDIO_ClearFlag (SDIO_STATIC_FLAGS);

    *(pscr + 1)=((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);

    *(pscr)=((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);

    return errorstatus;
  }


  /**
   * Send the command to initiate reading a number of blocks
   * @param blockIndex The first block
   * @param blockSize The block size to use
   * @param numberOfBlocks The number of blocks to read
   * @return true if succeeded
   */

  bool SdCardSdioFeature::readBlocksCommand(uint32_t blockIndex,uint16_t blockSize,uint32_t numberOfBlocks) {

    SD_Error errorstatus=SD_OK;
    SDIO_CmdInitTypeDef cmdInit;
    SDIO_DataInitTypeDef dataInit;

    SDIO->DCTRL=0x0;

    if(_cardType==SDIO_HIGH_CAPACITY_SD_CARD)
      blockSize=512;
    else
      blockIndex*=blockSize;      // convert to byte address for non-high cap

    /*!< Set Block Size for Card */
    cmdInit.SDIO_Argument=blockSize;
    cmdInit.SDIO_CmdIndex=SD_CMD_SET_BLOCKLEN;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_SET_BLOCKLEN);

    if(SD_OK != errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    dataInit.SDIO_DataTimeOut=SD_DATATIMEOUT;
    dataInit.SDIO_DataLength=numberOfBlocks*blockSize;
    dataInit.SDIO_DataBlockSize=(uint32_t)9 << 4;
    dataInit.SDIO_TransferDir=SDIO_TransferDir_ToSDIO;
    dataInit.SDIO_TransferMode=SDIO_TransferMode_Block;
    dataInit.SDIO_DPSM=SDIO_DPSM_Enable;
    SDIO_DataConfig(&dataInit);

    /*!< Send CMD18 READ_MULT_BLOCK with argument data address */
    cmdInit.SDIO_Argument=blockIndex;
    cmdInit.SDIO_CmdIndex=SD_CMD_READ_MULT_BLOCK;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_READ_MULT_BLOCK);

    if(SD_OK!=errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    // the command has been issued, now call one of the features that gets the data back for you

    return true;
  }


  /**
   * Issue the command to read a single block
   * @param blockIndex
   * @param blockSize
   */

  bool SdCardSdioFeature::readBlockCommand(uint32_t blockIndex,uint16_t blockSize) {

    SD_Error errorstatus = SD_OK;
    SDIO_CmdInitTypeDef cmdInit;
    SDIO_DataInitTypeDef dataInit;

    SDIO->DCTRL = 0x0;

    if(_cardType==SDIO_HIGH_CAPACITY_SD_CARD)
      blockSize=512;
    else
      blockIndex*=blockSize;      // convert to byte address for non-high cap

    /* Set Block Size for Card */

    cmdInit.SDIO_Argument = blockSize;
    cmdInit.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
    cmdInit.SDIO_Response = SDIO_Response_Short;
    cmdInit.SDIO_Wait = SDIO_Wait_No;
    cmdInit.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus = cmdResp1Error(SD_CMD_SET_BLOCKLEN);

    if (SD_OK != errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    dataInit.SDIO_DataTimeOut = SD_DATATIMEOUT;
    dataInit.SDIO_DataLength = blockSize;
    dataInit.SDIO_DataBlockSize = (uint32_t) 9 << 4;
    dataInit.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
    dataInit.SDIO_TransferMode = SDIO_TransferMode_Block;
    dataInit.SDIO_DPSM = SDIO_DPSM_Enable;
    SDIO_DataConfig(&dataInit);

    /*!< Send CMD17 READ_SINGLE_BLOCK */
    cmdInit.SDIO_Argument = blockIndex;
    cmdInit.SDIO_CmdIndex = SD_CMD_READ_SINGLE_BLOCK;
    cmdInit.SDIO_Response = SDIO_Response_Short;
    cmdInit.SDIO_Wait = SDIO_Wait_No;
    cmdInit.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus = cmdResp1Error(SD_CMD_READ_SINGLE_BLOCK);

    if(SD_OK!=errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    // the command has been issued, now call one of the features that gets the data back for you

    return true;
  }


  /**
   * Send the command to initiate writing a number of blocks
   * @param blockIndex The first block
   * @param blockSize The block size to use
   * @param numberOfBlocks The number of blocks to write
   * @return true if succeeded
   */

  bool SdCardSdioFeature::writeBlocksCommand(uint32_t blockIndex,uint16_t blockSize,uint32_t numberOfBlocks) {

    SD_Error errorstatus=SD_OK;
    SDIO_CmdInitTypeDef cmdInit;
    SDIO_DataInitTypeDef dataInit;

    SDIO->DCTRL=0x0;

    if(_cardType==SDIO_HIGH_CAPACITY_SD_CARD)
      blockSize=512;
    else
      blockIndex*=blockSize;      // convert to byte address for non-high cap

    /* Set Block Size for Card */
    cmdInit.SDIO_Argument=blockSize;
    cmdInit.SDIO_CmdIndex=SD_CMD_SET_BLOCKLEN;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_SET_BLOCKLEN);

    if(SD_OK != errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    /*!< To improve performance */
    cmdInit.SDIO_Argument=(uint32_t)(_rca << 16);
    cmdInit.SDIO_CmdIndex=SD_CMD_APP_CMD;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_APP_CMD);

    if(SD_OK != errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    /*!< To improve performance */
    cmdInit.SDIO_Argument=(uint32_t)numberOfBlocks;
    cmdInit.SDIO_CmdIndex=SD_CMD_SET_BLOCK_COUNT;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_SET_BLOCK_COUNT);

    if(SD_OK != errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    /*!< Send CMD25 WRITE_MULT_BLOCK with argument data address */
    cmdInit.SDIO_Argument=blockIndex;
    cmdInit.SDIO_CmdIndex=SD_CMD_WRITE_MULT_BLOCK;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_WRITE_MULT_BLOCK);

    if(SD_OK != errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    dataInit.SDIO_DataTimeOut=SD_DATATIMEOUT;
    dataInit.SDIO_DataLength=numberOfBlocks * blockSize;
    dataInit.SDIO_DataBlockSize=(uint32_t)9 << 4;
    dataInit.SDIO_TransferDir=SDIO_TransferDir_ToCard;
    dataInit.SDIO_TransferMode=SDIO_TransferMode_Block;
    dataInit.SDIO_DPSM=SDIO_DPSM_Enable;
    SDIO_DataConfig(&dataInit);

    if(SD_OK!=errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    // the command has been issued, now call one of the features that transfers the data out for you

    return true;
  }


  /**
   * Write a single block
   * @param blockIndex
   * @param blockSize
   * @return
   */

  bool SdCardSdioFeature::writeBlockCommand(uint32_t blockIndex,uint16_t blockSize) {

    SD_Error errorstatus=SD_OK;
    SDIO_CmdInitTypeDef cmdInit;
    SDIO_DataInitTypeDef dataInit;

    SDIO->DCTRL=0x0;

    if(_cardType==SDIO_HIGH_CAPACITY_SD_CARD)
      blockSize=512;
    else
      blockIndex*=blockSize;      // convert to byte address for non-high cap

    /* Set Block Size for Card */
    cmdInit.SDIO_Argument=blockSize;
    cmdInit.SDIO_CmdIndex=SD_CMD_SET_BLOCKLEN;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_SET_BLOCKLEN);

    if (SD_OK != errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    /*!< Send CMD24 WRITE_SINGLE_BLOCK */
    cmdInit.SDIO_Argument=blockIndex;
    cmdInit.SDIO_CmdIndex=SD_CMD_WRITE_SINGLE_BLOCK;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_WRITE_SINGLE_BLOCK);

    if(errorstatus != SD_OK) {
      return errorstatus;
    }

    dataInit.SDIO_DataTimeOut=SD_DATATIMEOUT;
    dataInit.SDIO_DataLength=blockSize;
    dataInit.SDIO_DataBlockSize=(uint32_t)9 << 4;
    dataInit.SDIO_TransferDir=SDIO_TransferDir_ToCard;
    dataInit.SDIO_TransferMode=SDIO_TransferMode_Block;
    dataInit.SDIO_DPSM=SDIO_DPSM_Enable;
    SDIO_DataConfig (&dataInit);

    if(SD_OK!=errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    // the command has been issued, now call one of the features that transfers the data out for you

    return true;
  }


  /**
   * @brief  Checks for error conditions for CMD0.
   * @param  None
   * @retval SD_Error: SD Card Error code.
   */

  SdCardIoFeature::SD_Error SdCardSdioFeature::cmdError() {

    SD_Error errorstatus=SD_OK;
    uint32_t timeout;

    timeout=SDIO_CMD0TIMEOUT; /*!< 10000 */

    while((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) == RESET))
      timeout--;

    if(timeout == 0) {
      errorstatus=SD_CMD_RSP_TIMEOUT;
      return errorstatus;
    }

    /*!< Clear all the static flags */
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);

    return errorstatus;
  }


  /**
   * @brief  Checks for error conditions for R7 response.
   * @param  None
   * @retval SD_Error: SD Card Error code.
   */

  SdCardIoFeature::SD_Error SdCardSdioFeature::cmdResp7Error() {

    SD_Error errorstatus=SD_OK;
    uint32_t status;
    uint32_t timeout=SDIO_CMD0TIMEOUT;

    status=SDIO->STA;

    while(!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) && (timeout > 0)) {
      timeout--;
      status=SDIO->STA;
    }

    if((timeout == 0) || (status & SDIO_FLAG_CTIMEOUT)) {
      /*!< Card is not V2.0 complient or card does not support the set voltage range */
      errorstatus=SD_CMD_RSP_TIMEOUT;
      SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
      return errorstatus;
    }

    if(status & SDIO_FLAG_CMDREND) {
      /*!< Card is SD V2.0 compliant */
      errorstatus=SD_OK;
      SDIO_ClearFlag(SDIO_FLAG_CMDREND);
      return errorstatus;
    }
    return errorstatus;
  }


  /**
   * @brief  Checks for error conditions for R1 response.
   * @param  cmd: The sent command index.
   * @retval SD_Error: SD Card Error code.
   */

  SdCardIoFeature::SD_Error SdCardSdioFeature::cmdResp1Error(uint8_t cmd) const {

    SD_Error errorstatus=SD_OK;
    uint32_t status;
    uint32_t response_r1;

    status=waitStatusFlags();

    if(status & SDIO_FLAG_CTIMEOUT) {
      errorstatus=SD_CMD_RSP_TIMEOUT;
      SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);

      return errorstatus;
    } else if(status & SDIO_FLAG_CCRCFAIL) {
      errorstatus=SD_CMD_CRC_FAIL;
      SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);

      return errorstatus;
    }

    /*!< Check response received is of desired command */
    if(SDIO_GetCommandResponse() != cmd) {
      errorstatus=SD_ILLEGAL_CMD;
      return errorstatus;
    }

    /*!< Clear all the static flags */
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);

    /*!< We have received response, retrieve it for analysis  */
    response_r1=SDIO_GetResponse(SDIO_RESP1);

    if((response_r1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
      return errorstatus;

    if(response_r1 & SD_OCR_ADDR_OUT_OF_RANGE)
      return SD_ADDR_OUT_OF_RANGE;

    if(response_r1 & SD_OCR_ADDR_MISALIGNED)
      return SD_ADDR_MISALIGNED;

    if(response_r1 & SD_OCR_BLOCK_LEN_ERR)
      return SD_BLOCK_LEN_ERR;

    if(response_r1 & SD_OCR_ERASE_SEQ_ERR)
      return SD_ERASE_SEQ_ERR;

    if(response_r1 & SD_OCR_BAD_ERASE_PARAM)
      return SD_BAD_ERASE_PARAM;

    if(response_r1 & SD_OCR_WRITE_PROT_VIOLATION)
      return SD_WRITE_PROT_VIOLATION;

    if(response_r1 & SD_OCR_LOCK_UNLOCK_FAILED)
      return SD_LOCK_UNLOCK_FAILED;

    if(response_r1 & SD_OCR_COM_CRC_FAILED)
      return SD_COM_CRC_FAILED;

    if(response_r1 & SD_OCR_ILLEGAL_CMD)
      return SD_ILLEGAL_CMD;

    if(response_r1 & SD_OCR_CARD_ECC_FAILED)
      return SD_CARD_ECC_FAILED;

    if(response_r1 & SD_OCR_CC_ERROR)
      return SD_CC_ERROR;

    if(response_r1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
      return SD_GENERAL_UNKNOWN_ERROR;

    if(response_r1 & SD_OCR_STREAM_READ_UNDERRUN)
      return SD_STREAM_READ_UNDERRUN;

    if(response_r1 & SD_OCR_STREAM_WRITE_OVERRUN)
      return SD_STREAM_WRITE_OVERRUN;

    if(response_r1 & SD_OCR_CID_CSD_OVERWRIETE)
      return SD_CID_CSD_OVERWRITE;

    if(response_r1 & SD_OCR_WP_ERASE_SKIP)
      return SD_WP_ERASE_SKIP;

    if(response_r1 & SD_OCR_CARD_ECC_DISABLED)
      return SD_CARD_ECC_DISABLED;

    if(response_r1 & SD_OCR_ERASE_RESET)
      return SD_ERASE_RESET;

    if(response_r1 & SD_OCR_AKE_SEQ_ERROR)
      return SD_AKE_SEQ_ERROR;

    return errorstatus;
  }


  /**
   * @brief  Checks for error conditions for R3 (OCR) response.
   * @param  None
   * @retval SD_Error: SD Card Error code.
   */

  SdCardIoFeature::SD_Error SdCardSdioFeature::cmdResp3Error() {

    uint32_t status;

    status=waitStatusFlags();

    if(status & SDIO_FLAG_CTIMEOUT) {
      SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
      return SD_CMD_RSP_TIMEOUT;
    }
    /*!< Clear all the static flags */
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);
    return SD_OK;
  }


  /**
   * @brief  Checks for error conditions for R2 (CID or CSD) response.
   * @param  None
   * @retval SD_Error: SD Card Error code.
   */

  SdCardIoFeature::SD_Error SdCardSdioFeature::cmdResp2Error() {

    SD_Error errorstatus=SD_OK;
    uint32_t status;

    status=waitStatusFlags();

    if(status & SDIO_FLAG_CTIMEOUT) {
      errorstatus=SD_CMD_RSP_TIMEOUT;
      SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
      return errorstatus;
    } else if(status & SDIO_FLAG_CCRCFAIL) {
      errorstatus=SD_CMD_CRC_FAIL;
      SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
      return errorstatus;
    }

    /*!< Clear all the static flags */
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);
    return errorstatus;
  }


  /*
   * Wait for STA register to indicate end or failure
   */

  uint32_t SdCardSdioFeature::waitStatusFlags() const {

    uint32_t status;

    do {
      status=SDIO->STA;
    } while(!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)));

    return status;
  }


  /**
   * @brief  Checks for error conditions for R6 (RCA) response.
   * @param  cmd: The sent command index.
   * @param  prca: pointer to the variable that will contain the SD card relative
   *         address RCA.
   * @retval SD_Error: SD Card Error code.
   */

  SdCardIoFeature::SD_Error SdCardSdioFeature::cmdResp6Error(uint8_t cmd,uint16_t *prca) {

    SD_Error errorstatus=SD_OK;
    uint32_t status;
    uint32_t response_r1;

    status=waitStatusFlags();

    if(status & SDIO_FLAG_CTIMEOUT) {
      errorstatus=SD_CMD_RSP_TIMEOUT;
      SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
      return errorstatus;
    } else if(status & SDIO_FLAG_CCRCFAIL) {
      errorstatus=SD_CMD_CRC_FAIL;
      SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
      return errorstatus;
    }

    /*!< Check response received is of desired command */
    if(SDIO_GetCommandResponse() != cmd) {
      errorstatus=SD_ILLEGAL_CMD;
      return errorstatus;
    }

    /*!< Clear all the static flags */
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);

    /*!< We have received response, retrieve it.  */
    response_r1=SDIO_GetResponse(SDIO_RESP1);

    if(SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED))) {
      *prca=(uint16_t)(response_r1 >> 16);
      return errorstatus;
    }

    if(response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
      return SD_GENERAL_UNKNOWN_ERROR;

    if(response_r1 & SD_R6_ILLEGAL_CMD)
      return SD_ILLEGAL_CMD;

    if(response_r1 & SD_R6_COM_CRC_FAILED)
      return SD_COM_CRC_FAILED;

    return errorstatus;
  }


  /**
   * Wait for receive to be complete
   */

  bool SdCardSdioFeature::waitForReceiveComplete() {

    SDCardState cardstate;

    while((SDIO->STA & SDIO_FLAG_RXACT)!=0);
    clearAllFlags();

    do {
      cardstate=getCardState();
    } while(cardstate==(int)SD_TRANSFER_BUSY);

    if(cardstate == SD_CARD_ERROR)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,SD_ERROR,SD_CARD_ERROR);

    while(getStatus()!=SD_TRANSFER_OK);
    return true;
  }


  /**
   * Wait for transmission to be complete
   */

  bool SdCardSdioFeature::waitForTransmitComplete() {

    SDCardState cardstate;

    while((SDIO->STA & SDIO_FLAG_TXACT)!=0);
    clearAllFlags();

    do {
      cardstate=getCardState();
    } while(cardstate==(int)SD_TRANSFER_BUSY);

    if(cardstate == SD_CARD_ERROR)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,SD_ERROR,SD_CARD_ERROR);

    while(getStatus()!=SD_TRANSFER_OK);
    return true;
  }


  /**
   * @brief  Returns the current card's state.
   * @param  None
   * @retval SDCardState: SD Card Error or SD Card Current State.
   */

  SdCardSdioFeature::SDCardState SdCardSdioFeature::getCardState() const {

    uint32_t resp1;

    if(sendStatus(resp1)!=SD_OK)
      return SD_CARD_ERROR;
    else
      return (SDCardState)((resp1 >> 9) & 0x0F);
  }


 /**
  * @brief  Gets the cuurent sd card data transfer status.
  * @param  None
  * @retval SDTransferState: Data Transfer state.
  *   This value can be:
  *        - SD_TRANSFER_OK: No data transfer is acting
  *        - SD_TRANSFER_BUSY: Data transfer is acting
  */

  SdCardSdioFeature::SDTransferState SdCardSdioFeature::getStatus() const {

    SDCardState cardstate;

    cardstate=getCardState();

    if(cardstate == SD_CARD_TRANSFER)
      return SD_TRANSFER_OK;
    else if(cardstate == SD_CARD_ERROR)
      return SD_TRANSFER_ERROR;

    return SD_TRANSFER_BUSY;
  }

  /**
   * @brief  Returns the current card's status.
   * @param  pcardstatus: pointer to the buffer that will contain the SD card
   *         status (Card Status register).
   * @retval SD_Error: SD Card Error code.
   */


  SdCardIoFeature::SD_Error SdCardSdioFeature::sendStatus(uint32_t& cardstatus) const {

    SD_Error errorstatus=SD_OK;
    SDIO_CmdInitTypeDef cmdInit;

    cmdInit.SDIO_Argument=(uint32_t)_rca << 16;
    cmdInit.SDIO_CmdIndex=SD_CMD_SEND_STATUS;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    errorstatus=cmdResp1Error(SD_CMD_SEND_STATUS);

    if(errorstatus != SD_OK)
      return errorstatus;

    cardstatus=SDIO_GetResponse(SDIO_RESP1);
    return errorstatus;
  }


  /**
   * @brief Allows to erase memory area specified for the given card.
   */

  bool SdCardSdioFeature::eraseBlocks(uint32_t firstBlock,uint32_t lastBlock) {

    SD_Error errorstatus=SD_OK;
    uint32_t delay;
    uint8_t cardstate;
    SDIO_CmdInitTypeDef cmdInit;

    /*!< Check if the card coomnd class supports erase command */

    if(((_csdTab[1] >> 20) & SD_CCCC_ERASE) == 0)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,SD_ERROR,SD_REQUEST_NOT_APPLICABLE);

    // check for a locked card

    if(SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,SD_ERROR,SD_LOCK_UNLOCK_FAILED);

    // scale down for non high-cap cards

    if(_cardType!=SDIO_HIGH_CAPACITY_SD_CARD) {
      firstBlock*=512;
      lastBlock*=512;
    }

    /*!< According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */

    if(SDIO_STD_CAPACITY_SD_CARD_V1_1==_cardType || SDIO_STD_CAPACITY_SD_CARD_V2_0==_cardType || SDIO_HIGH_CAPACITY_SD_CARD==_cardType) {

      /*!< Send CMD32 SD_ERASE_GRP_START with argument as addr  */

      cmdInit.SDIO_Argument=firstBlock;
      cmdInit.SDIO_CmdIndex=SD_CMD_SD_ERASE_GRP_START;
      cmdInit.SDIO_Response=SDIO_Response_Short;
      cmdInit.SDIO_Wait=SDIO_Wait_No;
      cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
      SDIO_SendCommand(&cmdInit);

      if((errorstatus=cmdResp1Error(SD_CMD_SD_ERASE_GRP_START))!=SD_OK)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,SD_ERROR,errorstatus);

      /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
      cmdInit.SDIO_Argument=lastBlock;
      cmdInit.SDIO_CmdIndex=SD_CMD_SD_ERASE_GRP_END;
      cmdInit.SDIO_Response=SDIO_Response_Short;
      cmdInit.SDIO_Wait=SDIO_Wait_No;
      cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
      SDIO_SendCommand(&cmdInit);

      if((errorstatus=cmdResp1Error(SD_CMD_SD_ERASE_GRP_END))!=SD_OK)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,SD_ERROR,errorstatus);
    }

    /*!< Send CMD38 ERASE */
    cmdInit.SDIO_Argument=0;
    cmdInit.SDIO_CmdIndex=SD_CMD_ERASE;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    if(errorstatus!=SD_OK)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,SD_ERROR,errorstatus);

    ///< Wait till the card is in the programming state

    errorstatus=isCardProgramming(cardstate);

    delay=SD_DATATIMEOUT;
    while((delay > 0) && (errorstatus == SD_OK) && ((SD_CARD_PROGRAMMING == cardstate) || (SD_CARD_RECEIVING == cardstate))) {
      errorstatus=isCardProgramming(cardstate);
      delay--;
    }

    if(SD_OK!=errorstatus)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    return true;
  }


  /**
   * @brief  Aborts an ongoing data transfer.
   * @param  None
   * @retval SD_Error: SD Card Error code.
   */

  bool SdCardSdioFeature::stopTransfer() const {

    SDIO_CmdInitTypeDef cmdInit;
    SD_Error errorstatus;

    /*!< Send CMD12 STOP_TRANSMISSION  */

    cmdInit.SDIO_Argument = 0x0;
    cmdInit.SDIO_CmdIndex = SD_CMD_STOP_TRANSMISSION;
    cmdInit.SDIO_Response = SDIO_Response_Short;
    cmdInit.SDIO_Wait = SDIO_Wait_No;
    cmdInit.SDIO_CPSM = SDIO_CPSM_Enable;

    SDIO_SendCommand(&cmdInit);

    if((errorstatus=cmdResp1Error(SD_CMD_STOP_TRANSMISSION))!=SD_OK)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SD_SDIO,E_SDIO_ERROR,errorstatus);

    return true;
  }


  /**
   * @brief  Checks if the SD card is in programming state.
   * @param  pstatus: pointer to the variable that will contain the SD card state.
   * @retval SD_Error: SD Card Error code.
   */

  SdCardIoFeature::SD_Error SdCardSdioFeature::isCardProgramming(uint8_t& cardstatus) const {

    SD_Error errorstatus=SD_OK;
    volatile uint32_t respR1,status;
    SDIO_CmdInitTypeDef cmdInit;

    cmdInit.SDIO_Argument=(uint32_t)_rca << 16;
    cmdInit.SDIO_CmdIndex=SD_CMD_SEND_STATUS;
    cmdInit.SDIO_Response=SDIO_Response_Short;
    cmdInit.SDIO_Wait=SDIO_Wait_No;
    cmdInit.SDIO_CPSM=SDIO_CPSM_Enable;
    SDIO_SendCommand(&cmdInit);

    status=waitStatusFlags();

    if(status & SDIO_FLAG_CTIMEOUT) {
      errorstatus=SD_CMD_RSP_TIMEOUT;
      SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
      return errorstatus;
    } else if(status & SDIO_FLAG_CCRCFAIL) {
      errorstatus=SD_CMD_CRC_FAIL;
      SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
      return errorstatus;
    }

    MillisecondTimer::delay(2);
    status=(uint32_t)SDIO_GetCommandResponse();

    /*!< Check response received is of desired command */
    if(status != SD_CMD_SEND_STATUS) {
      errorstatus=SD_ILLEGAL_CMD;
      return errorstatus;
    }

    /*!< Clear all the static flags */
    SDIO_ClearFlag (SDIO_STATIC_FLAGS);

    /*!< We have received response, retrieve it for analysis  */
    respR1=SDIO_GetResponse(SDIO_RESP1);

    /*!< Find out card status */
    cardstatus=(uint8_t)((respR1 >> 9) & 0x0000000F);

    if((respR1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
      return errorstatus;

    if(respR1 & SD_OCR_ADDR_OUT_OF_RANGE)
      return SD_ADDR_OUT_OF_RANGE;

    if(respR1 & SD_OCR_ADDR_MISALIGNED)
      return SD_ADDR_MISALIGNED;

    if(respR1 & SD_OCR_BLOCK_LEN_ERR)
      return SD_BLOCK_LEN_ERR;

    if(respR1 & SD_OCR_ERASE_SEQ_ERR)
      return SD_ERASE_SEQ_ERR;

    if(respR1 & SD_OCR_BAD_ERASE_PARAM)
      return SD_BAD_ERASE_PARAM;

    if(respR1 & SD_OCR_WRITE_PROT_VIOLATION)
      return SD_WRITE_PROT_VIOLATION;

    if(respR1 & SD_OCR_LOCK_UNLOCK_FAILED)
      return SD_LOCK_UNLOCK_FAILED;

    if(respR1 & SD_OCR_COM_CRC_FAILED)
      return SD_COM_CRC_FAILED;

    if(respR1 & SD_OCR_ILLEGAL_CMD)
      return SD_ILLEGAL_CMD;

    if(respR1 & SD_OCR_CARD_ECC_FAILED)
      return SD_CARD_ECC_FAILED;

    if(respR1 & SD_OCR_CC_ERROR)
      return SD_CC_ERROR;

    if(respR1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
      return SD_GENERAL_UNKNOWN_ERROR;

    if(respR1 & SD_OCR_STREAM_READ_UNDERRUN)
      return SD_STREAM_READ_UNDERRUN;

    if(respR1 & SD_OCR_STREAM_WRITE_OVERRUN)
      return SD_STREAM_WRITE_OVERRUN;

    if(respR1 & SD_OCR_CID_CSD_OVERWRIETE)
      return SD_CID_CSD_OVERWRITE;

    if(respR1 & SD_OCR_WP_ERASE_SKIP)
      return SD_WP_ERASE_SKIP;

    if(respR1 & SD_OCR_CARD_ECC_DISABLED)
      return SD_CARD_ECC_DISABLED;

    if(respR1 & SD_OCR_ERASE_RESET)
      return SD_ERASE_RESET;

    if(respR1 & SD_OCR_AKE_SEQ_ERROR)
      return SD_AKE_SEQ_ERROR;

    return errorstatus;
  }
}

#endif
