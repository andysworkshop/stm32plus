/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Base class for the CAN peripheral
   */

  class Can {

    public:

      enum {
        E_TX_NO_MAILBOX = 1
      };

    protected:
      CAN_TypeDef *_peripheralAddress;
      CAN_InitTypeDef _init;

    protected:
      Can(CAN_TypeDef *peripheralAddress);

    public:
      void sleep();
      void wakeup();

      bool send(uint32_t ID,uint8_t IDE,uint8_t RTR,uint8_t DLC,const uint8_t *data);
      bool send(CanTxMsg& Msg);

      operator CAN_TypeDef *();
      operator CAN_InitTypeDef *();
  };


  /**
   * Constructor
   * @param peripheralAddress the CAN base address
   */

  inline Can::Can(CAN_TypeDef *peripheralAddress) :
    _peripheralAddress(peripheralAddress) {
  }


  /**
   * Go to sleep
   */

  inline void Can::enablePeripheral() const {
    CAN_Sleep(_peripheralAddress);
  }


  /**
   * Wake up
   */

  inline void Can::wakeup() const {
    CAN_WakeUp(_peripheralAddress);
  }


  /**
   * Get the peripheral address
   * @return The CAN peripheral address
   */

  inline Can::operator CAN_TypeDef *() {
    return _peripheralAddress;
  }


  /**
   * Get the init structure
   * @return The CAN_InitTypeDef structure
   */

  inline Can::operator CAN_InitTypeDef *() {
    return &_init;
  }


  /**
   * Send a message
   * @param ID
   * @param IDE
   * @param RTR
   * @param DLC
   * @param data
   * @return true if it worked
   */

  inline bool Can::send(const uint32_t ID,const uint8_t IDE,const uint8_t RTR,const uint8_t DLC,uint8_t* data) {

    CanTxMsg msg;
    uint8_t i;

    msg.IDE=IDE;
    msg.RTR=RTR;
    msg.DLC=DLC;
    msg.ExtId=ID;

    for(i=0;i<DLC;i++)
      msg.Data[i]=data[i];

    return send(msg);
  }


  /**
   * Send a message given a message structure
   * @param msg
   * @return true if it worked
   */

  inline bool Can::send(CanTxMsg& Msg) {

    if(CAN_Transmit(_peripheralAddress,&msg)==CAN_TxStatus_NoMailBox)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_CAN,E_TX_NO_MAILBOX,CAN_TxStatus_NoMailBox);

    return true;
  }
}

