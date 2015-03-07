/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/usb/device/device.h"


using namespace stm32plus;
using namespace stm32plus::usb;


/**
  * @brief  Initializes the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */

USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev) {
  return stm32plus::usb::usb_device_internal::notifyEvent(pdev,UsbEventDescriptor::EventType::DEVICE_INIT);
}


/**
  * @brief  De-Initializes the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */

USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev) {
  return stm32plus::usb::usb_device_internal::notifyEvent(pdev,UsbEventDescriptor::EventType::DEVICE_DEINIT);
}


/**
  * @brief  Starts the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */

USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev) {
  return stm32plus::usb::usb_device_internal::notifyEvent(pdev,UsbEventDescriptor::EventType::DEVICE_START);
}


/**
  * @brief  Stops the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */

USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev) {
  return stm32plus::usb::usb_device_internal::notifyEvent(pdev,UsbEventDescriptor::EventType::DEVICE_STOP);
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

  DeviceSdkOpenEndpointEvent event(ep_addr,static_cast<EndpointType>(ep_type),ep_mps);

  reinterpret_cast<UsbEventSource *>(pdev->pUserData)->UsbEventSender.raiseEvent(event);
  return event.retval;
}


/**
  * @brief  Closes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */

USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev,uint8_t ep_addr) {
  return stm32plus::usb::usb_device_internal::endpointEvent(pdev,UsbEventDescriptor::EventType::DEVICE_CLOSE_ENDPOINT,ep_addr);
}


/**
  * @brief  Flushes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */

USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev,uint8_t ep_addr) {
  return stm32plus::usb::usb_device_internal::endpointEvent(pdev,UsbEventDescriptor::EventType::DEVICE_FLUSH_ENDPOINT,ep_addr);
}


/**
  * @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */

USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr) {
  return stm32plus::usb::usb_device_internal::endpointEvent(pdev,UsbEventDescriptor::EventType::DEVICE_STALL_ENDPOINT,ep_addr);
}


/**
  * @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */

USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev,uint8_t ep_addr) {
  return stm32plus::usb::usb_device_internal::endpointEvent(pdev,UsbEventDescriptor::EventType::DEVICE_CLEAR_STALL_ENDPOINT,ep_addr);
}


/**
  * @brief  Returns Stall condition.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval Stall (1: Yes, 0: No)
  */

uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev,uint8_t ep_addr) {

  DeviceSdkIsStalledEndpointEvent event(ep_addr);

  reinterpret_cast<UsbEventSource *>(pdev->pUserData)->UsbEventSender.raiseEvent(event);
  return event.isStalled;
}


/**
  * @brief  Assigns a USB address to the device.
  * @param  pdev: Device handle
  * @param  dev_addr: device address - pointless parameter because it's in the pdev structure!
  * @retval USBD Status
  */

USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev,uint8_t /* dev_addr */) {
  return stm32plus::usb::usb_device_internal::notifyEvent(pdev,UsbEventDescriptor::EventType::DEVICE_SET_USB_ADDRESS);
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

  DeviceSdkDataEndpointEvent event(UsbEventDescriptor::EventType::DEVICE_TRANSMIT,ep_addr,pbuf,size);

  reinterpret_cast<UsbEventSource *>(pdev->pUserData)->UsbEventSender.raiseEvent(event);
  return event.retval;
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

  DeviceSdkDataEndpointEvent event(UsbEventDescriptor::EventType::DEVICE_PREPARE_RECEIVE,ep_addr,pbuf,size);

  reinterpret_cast<UsbEventSource *>(pdev->pUserData)->UsbEventSender.raiseEvent(event);
  return event.retval;
}


/**
  * @brief  Returns the last transfered packet size.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval Recived Data Size
  */

uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev,uint8_t ep_addr) {

  DeviceSdkGetLastTransferredSizeEndpointEvent event(ep_addr);

  reinterpret_cast<UsbEventSource *>(pdev->pUserData)->UsbEventSender.raiseEvent(event);
  return event.lastTransferred;
}


/**
  * @brief  Delays routine for the USB Device Library.
  * @param  Delay: Delay in ms
  * @retval None
  */

void USBD_LL_Delay(uint32_t delay) {
  MillisecondTimer::delay(delay);
}


/**
 * Reset IRQ callback
 */

void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd) {

  reinterpret_cast<UsbEventSource *>(
    reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)->pUserData
  )->UsbEventSender.raiseEvent(DeviceSdkResetInterruptEvent());
}


/**
 * Suspend IRQ callback
 */

void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd) {
  reinterpret_cast<UsbEventSource *>(
    reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)->pUserData
  )->UsbEventSender.raiseEvent(DeviceSdkSuspendInterruptEvent());
}


/**
 * Resume IRQ callback
 */

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd) {
  reinterpret_cast<UsbEventSource *>(
    reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)->pUserData
  )->UsbEventSender.raiseEvent(DeviceSdkResumeInterruptEvent());
}


/**
 * Connect callback
 */

void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd) {
  reinterpret_cast<UsbEventSource *>(
    reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)->pUserData
  )->UsbEventSender.raiseEvent(DeviceSdkConnectInterruptEvent());
}


/**
 * Disconnect callback
 */

void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd) {
  reinterpret_cast<UsbEventSource *>(
    reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)->pUserData
  )->UsbEventSender.raiseEvent(DeviceSdkDisconnectInterruptEvent());
}


/**
 * Setup stage callback
 */

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd) {
  reinterpret_cast<UsbEventSource *>(
    reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)->pUserData
  )->UsbEventSender.raiseEvent(DeviceSdkSetupStageInterruptEvent());
}


/**
 * Data out stage callback
 */

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd,uint8_t epnum) {
  reinterpret_cast<UsbEventSource *>(
    reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)->pUserData
  )->UsbEventSender.raiseEvent(DeviceSdkDataOutStageInterruptEvent(epnum));
}


/**
 * Data in stage callback
 */

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd,uint8_t epnum) {
  reinterpret_cast<UsbEventSource *>(
    reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)->pUserData
  )->UsbEventSender.raiseEvent(DeviceSdkDataInStageInterruptEvent(epnum));
}


#endif
