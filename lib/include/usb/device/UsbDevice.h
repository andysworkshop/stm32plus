/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /*
   * USB Device
   */

  template<class TPhy,class... Features>
  class UsbDevice : public UsbCore<TPhy>,
                    public UsbDeviceEventSource,
                    public Features... {

  };


  /*
   * The following C functions are callbacks from the ST driver code. We'll translate them
   * to an event and pass them to subscribers
   */

  extern "C" {

    /**
      * @brief  Initializes the Low Level portion of the Device driver.
      * @param  pdev: Device handle
      * @retval USBD Status
      */

    inline USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev) {
      return USBD_OK;
    }


    /**
      * @brief  De-Initializes the Low Level portion of the Device driver.
      * @param  pdev: Device handle
      * @retval USBD Status
      */
    inline USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev) {
      return USBD_OK;
    }


    /**
      * @brief  Starts the Low Level portion of the Device driver.
      * @param  pdev: Device handle
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev) {
      return USBD_OK;
    }


    /**
      * @brief  Stops the Low Level portion of the Device driver.
      * @param  pdev: Device handle
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev) {
      return USBD_OK;
    }


    /**
      * @brief  Opens an endpoint of the Low Level Driver.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @param  ep_type: Endpoint Type
      * @param  ep_mps: Endpoint Max Packet Size
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev,uint8_t ep_addr,uint8_t ep_type,uint16_t ep_mps) {
      return USBD_OK;
    }


    /**
      * @brief  Closes an endpoint of the Low Level Driver.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev,uint8_t ep_addr) {
      return USBD_OK;
    }


    /**
      * @brief  Flushes an endpoint of the Low Level Driver.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev,uint8_t ep_addr) {
      return USBD_OK;
    }


    /**
      * @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr) {
      return USBD_OK;
    }


    /**
      * @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdevuint8_t ep_addr) {
      return USBD_OK;
    }


    /**
      * @brief  Returns Stall condition.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @retval Stall (1: Yes, 0: No)
      */

    uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev,uint8_t ep_addr) {
       return 0;
    }


    /**
      * @brief  Assigns a USB address to the device.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev,uint8_t dev_addr) {
      return USBD_OK;
    }


    /**
      * @brief  Transmits data over an endpoint.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @param  pbuf: Pointer to data to be sent
      * @param  size: Data size
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev,uint8_t ep_addr,uint8_t *pbuf,uint16_t size) {
      return USBD_OK;
    }

    /**
      * @brief  Prepares an endpoint for reception.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @param  pbuf: Pointer to data to be received
      * @param  size: Data size
      * @retval USBD Status
      */

    USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev,uint8_t ep_addr,uint8_t *pbuf,uint16_t size) {
      return USBD_OK;
    }

    /**
      * @brief  Returns the last transfered packet size.
      * @param  pdev: Device handle
      * @param  ep_addr: Endpoint Number
      * @retval Recived Data Size
      */

    uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev,uint8_t ep_addr) {
      return 0;
    }

    /**
      * @brief  Delays routine for the USB Device Library.
      * @param  Delay: Delay in ms
      * @retval None
      */

    void USBD_LL_Delay(uint32_t Delay) {
    }
  }
}
