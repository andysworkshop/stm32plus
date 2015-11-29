/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Command status wrapper (CSW) structure
     */

    struct MscBotCommandStatusWrapper {

      enum {
        SIGNATURE = 0x53425355,
        TRANSMIT_SIZE = 13
      };

      uint32_t dSignature;
      uint32_t dTag;
      uint32_t dDataResidue;
      MscBotCswStatus bStatus;
      uint8_t reservedAlign[3];

      /**
       * Constructor
       */

      MscBotCommandStatusWrapper() {
        static_assert(sizeof(MscBotCommandStatusWrapper)==16,"Compiler error: sizeof(MscBotCommandStatusWrapper)!=16");
        dSignature=SIGNATURE;
      }


      /**
       * Utility function to send the CSW. The CSW is passed bSetween BOT and SCSI objects so a common
       * place to call the send function is required
       * @param status The CSW status
       * @param botState Refence to bot state so it can be updated
       * @param deviceHandle Reference to the device handle
       * @param cbw The CBW object to receive next command
       */

      template<uint8_t TInEpAddress,uint8_t TOutEpAddress>
      void send(MscBotCswStatus status,MscBotState& botState,USBD_HandleTypeDef& deviceHandle,MscBotCommandBlockWrapper& cbw) {

        bStatus=status;
        botState=MscBotState::IDLE;

        // send the status

        USBD_LL_Transmit(
            &deviceHandle,
            TInEpAddress,
            reinterpret_cast<uint8_t *>(this),
            TRANSMIT_SIZE);

        // prepare EP to receive next command

        USBD_LL_PrepareReceive(
            &deviceHandle,
            TOutEpAddress,
            reinterpret_cast<uint8_t *>(&cbw),
            MscBotCommandBlockWrapper::RECEIVE_SIZE);
      }

    } __attribute__((packed));
  }
}
