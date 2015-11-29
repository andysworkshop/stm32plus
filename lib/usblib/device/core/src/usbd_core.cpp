/**
  ******************************************************************************
  * @file    usbd_core.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    13-June-2014
  * @brief   This file provides all the USBD core functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
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
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "config/stm32plus.h"
#include "config/event.h"
#include "usb/f4/UsbEventDescriptor.h"
#include "usb/f4/UsbErrorEvent.h"
#include "usb/f4/UsbEventSource.h"
#include "usblib/device/core/inc/usbd_core.h"
#include "usb/f4/device/events/class/DeviceClassSdkInitEvent.h"
#include "usb/f4/device/events/class/DeviceClassSdkDeInitEvent.h"
#include "usb/f4/device/events/class/DeviceClassSdkEp0ReadyEvent.h"
#include "usb/f4/device/events/class/DeviceClassSdkEp0TxSentEvent.h"
#include "usb/f4/device/events/class/DeviceClassSdkDataInEvent.h"
#include "usb/f4/device/events/class/DeviceClassSdkDataOutEvent.h"
#include "usb/f4/device/events/class/DeviceClassSdkSofEvent.h"


/**
* @brief  USBD_Init
*         Initailizes the device stack and load the class driver
* @param  pdev: device instance
* @param  pdesc: Descriptor structure address
* @param  id: Low level core index
* @retval None
*/
USBD_StatusTypeDef USBD_Init(USBD_HandleTypeDef *pdev, USBD_DescriptorsTypeDef *pdesc, uint8_t id)
{
  /* Check whether the USB Host handle is valid */
  if(pdev == NULL)
  {
    USBD_ErrLog("Invalid Device handle");
    return USBD_FAIL; 
  }
  
  /* Unlink previous class*/
  if(pdev->pEventSource)
    pdev->pEventSource = NULL;
  
  /* Assign USBD Descriptors */
  if(pdesc != NULL)
  {
    pdev->pDesc = pdesc;
  }
  
  /* Set Device initial State */
  pdev->dev_state  = USBD_STATE_DEFAULT;
  pdev->id = id;
  /* Initialize low level driver */
  USBD_LL_Init(pdev);
  
  return USBD_OK; 
}

/**
* @brief  USBD_DeInit 
*         Re-Initialize th device library
* @param  pdev: device instance
* @retval status: status
*/
USBD_StatusTypeDef USBD_DeInit(USBD_HandleTypeDef *pdev)
{
  /* Set Default State */
  pdev->dev_state  = USBD_STATE_DEFAULT;
  
  /* Free Class Resources */
  pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkDeInitEvent(pdev->dev_config));
  
    /* Stop the low level driver  */
  USBD_LL_Stop(pdev); 
  
  /* Initialize low level driver */
  USBD_LL_DeInit(pdev);
  
  return USBD_OK;
}


/**
  * @brief  USBD_RegisterClass 
  *         Link class driver to Device Core.
  * @param  pDevice : Device Handle
  * @param  pEventSource: Pointer to event source
  * @retval USBD Status
  */

USBD_StatusTypeDef  USBD_RegisterClass(USBD_HandleTypeDef *pdev, stm32plus::usb::UsbEventSource *eventSource)
{
  pdev->pEventSource=eventSource;
  return USBD_OK;
}


/**
  * @brief  USBD_Start 
  *         Start the USB Device Core.
  * @param  pdev: Device Handle
  * @retval USBD Status
  */
USBD_StatusTypeDef  USBD_Start  (USBD_HandleTypeDef *pdev)
{
  
  /* Start the low level driver  */
  USBD_LL_Start(pdev); 
  
  return USBD_OK;  
}

/**
  * @brief  USBD_Stop 
  *         Stop the USB Device Core.
  * @param  pdev: Device Handle
  * @retval USBD Status
  */
USBD_StatusTypeDef  USBD_Stop   (USBD_HandleTypeDef *pdev)
{
  /* Free Class Resources */
  pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkDeInitEvent(pdev->dev_config));

  /* Stop the low level driver  */
  USBD_LL_Stop(pdev); 
  
  return USBD_OK;  
}

/**
* @brief  USBD_RunTestMode 
*         Launch test mode process
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef  USBD_RunTestMode (USBD_HandleTypeDef  *pdev __attribute__((unused)))
{
  return USBD_OK;
}


/**
* @brief  USBD_SetClassConfig 
*        Configure device and start the interface
* @param  pdev: device instance
* @param  cfgidx: configuration index
* @retval status
*/

USBD_StatusTypeDef USBD_SetClassConfig(USBD_HandleTypeDef  *pdev, uint8_t cfgidx)
{
  if(pdev->pEventSource)
  {
    // send init event

    stm32plus::usb::DeviceClassSdkInitEvent event(cfgidx);
    pdev->pEventSource->UsbEventSender.raiseEvent(event);

    return event.status;
  }
  else
    return USBD_FAIL;
}

/**
* @brief  USBD_ClrClassConfig 
*         Clear current configuration
* @param  pdev: device instance
* @param  cfgidx: configuration index
* @retval status: USBD_StatusTypeDef
*/
USBD_StatusTypeDef USBD_ClrClassConfig(USBD_HandleTypeDef  *pdev, uint8_t cfgidx)
{
  /* Clear configuration  and Deinitialize the Class process*/
  pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkDeInitEvent(cfgidx));
  return USBD_OK;
}


/**
* @brief  USBD_SetupStage 
*         Handle the setup stage
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_SetupStage(USBD_HandleTypeDef *pdev, uint8_t *psetup)
{

  USBD_ParseSetupRequest(&pdev->request, psetup);
  
  pdev->ep0_state = USBD_EP0_SETUP;
  pdev->ep0_data_len = pdev->request.wLength;
  
  switch (pdev->request.bmRequest & 0x1F) 
  {
  case USB_REQ_RECIPIENT_DEVICE:   
    USBD_StdDevReq (pdev, &pdev->request);
    break;
    
  case USB_REQ_RECIPIENT_INTERFACE:     
    USBD_StdItfReq(pdev, &pdev->request);
    break;
    
  case USB_REQ_RECIPIENT_ENDPOINT:        
    USBD_StdEPReq(pdev, &pdev->request);   
    break;
    
  default:           
    USBD_LL_StallEP(pdev , pdev->request.bmRequest & 0x80);
    break;
  }  
  return USBD_OK;  
}

/**
* @brief  USBD_DataOutStage 
*         Handle data OUT stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
USBD_StatusTypeDef USBD_LL_DataOutStage(USBD_HandleTypeDef *pdev , uint8_t epnum, uint8_t *pdata)
{
  USBD_EndpointTypeDef    *pep;
  
  if(epnum == 0) 
  {
    pep = &pdev->ep_out[0];
    
    if ( pdev->ep0_state == USBD_EP0_DATA_OUT)
    {
      if(pep->rem_length > pep->maxpacket)
      {
        pep->rem_length -=  pep->maxpacket;
       
        USBD_CtlContinueRx (pdev, 
                            pdata,
                            MIN(pep->rem_length ,pep->maxpacket));
      }
      else
      {
        if(pdev->dev_state == USBD_STATE_CONFIGURED)
          pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkEp0ReadyEvent());

        USBD_CtlSendStatus(pdev);
      }
    }
  }
  else if(pdev->dev_state == USBD_STATE_CONFIGURED)
    pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkDataOutEvent(epnum));

  return USBD_OK;
}

/**
* @brief  USBD_DataInStage 
*         Handle data in stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
USBD_StatusTypeDef USBD_LL_DataInStage(USBD_HandleTypeDef *pdev ,uint8_t epnum, uint8_t *pdata)
{
  USBD_EndpointTypeDef    *pep;
    
  if(epnum == 0) 
  {
    pep = &pdev->ep_in[0];
    
    if ( pdev->ep0_state == USBD_EP0_DATA_IN)
    {
      if(pep->rem_length > pep->maxpacket)
      {
        pep->rem_length -=  pep->maxpacket;
        
        USBD_CtlContinueSendData (pdev, 
                                  pdata, 
                                  pep->rem_length);
      }
      else
      { /* last packet is MPS multiple, so send ZLP packet */
        if((pep->total_length % pep->maxpacket == 0) &&
           (pep->total_length >= pep->maxpacket) &&
             (pep->total_length < pdev->ep0_data_len ))
        {
          
          USBD_CtlContinueSendData(pdev , NULL, 0);
          pdev->ep0_data_len = 0;
        }
        else
        {
          if(pdev->dev_state == USBD_STATE_CONFIGURED)
            pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkEp0TxSentEvent());

          USBD_CtlReceiveStatus(pdev);
        }
      }
    }
    if (pdev->dev_test_mode == 1)
    {
      USBD_RunTestMode(pdev); 
      pdev->dev_test_mode = 0;
    }
  }
  else if(pdev->dev_state == USBD_STATE_CONFIGURED)
    pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkDataInEvent(epnum));

  return USBD_OK;
}

/**
* @brief  USBD_LL_Reset 
*         Handle Reset event
* @param  pdev: device instance
* @retval status
*/

USBD_StatusTypeDef USBD_LL_Reset(USBD_HandleTypeDef  *pdev)
{
  /* Open EP0 OUT */
  USBD_LL_OpenEP(pdev,
              0x00,
              USBD_EP_TYPE_CTRL,
              USB_MAX_EP0_SIZE);
  
  pdev->ep_out[0].maxpacket = USB_MAX_EP0_SIZE;
  
  /* Open EP0 IN */
  USBD_LL_OpenEP(pdev,
              0x80,
              USBD_EP_TYPE_CTRL,
              USB_MAX_EP0_SIZE);
  
  pdev->ep_in[0].maxpacket = USB_MAX_EP0_SIZE;
  /* Upon Reset call usr call back */
  pdev->dev_state = USBD_STATE_DEFAULT;
  
  if (pdev->pEventSource)
    pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkDeInitEvent(pdev->dev_config));
 
  
  return USBD_OK;
}




/**
* @brief  USBD_LL_Reset 
*         Handle Reset event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_SetSpeed(USBD_HandleTypeDef  *pdev, USBD_SpeedTypeDef speed)
{
  pdev->dev_speed = speed;
  return USBD_OK;
}

/**
* @brief  USBD_Suspend 
*         Handle Suspend event
* @param  pdev: device instance
* @retval status
*/

USBD_StatusTypeDef USBD_LL_Suspend(USBD_HandleTypeDef  *pdev)
{
  pdev->dev_old_state =  pdev->dev_state;
  pdev->dev_state  = USBD_STATE_SUSPENDED;
  return USBD_OK;
}

/**
* @brief  USBD_Resume 
*         Handle Resume event
* @param  pdev: device instance
* @retval status
*/

USBD_StatusTypeDef USBD_LL_Resume(USBD_HandleTypeDef  *pdev)
{
  pdev->dev_state = pdev->dev_old_state;  
  return USBD_OK;
}

/**
* @brief  USBD_SOF 
*         Handle SOF event
* @param  pdev: device instance
* @retval status
*/

USBD_StatusTypeDef USBD_LL_SOF(USBD_HandleTypeDef  *pdev)
{
  if(pdev->dev_state == USBD_STATE_CONFIGURED && pdev->pEventSource)
    pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkSofEvent());

  return USBD_OK;
}

/**
* @brief  USBD_IsoINIncomplete 
*         Handle iso in incomplete event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_IsoINIncomplete(USBD_HandleTypeDef  *pdev __attribute__((unused)),
                                           uint8_t epnum __attribute__((unused)))
{
  return USBD_OK;
}

/**
* @brief  USBD_IsoOUTIncomplete 
*         Handle iso out incomplete event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_IsoOUTIncomplete(USBD_HandleTypeDef  *pdev __attribute__((unused)),
                                            uint8_t epnum __attribute__((unused)))
{
  return USBD_OK;
}

/**
* @brief  USBD_DevConnected 
*         Handle device connection event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_DevConnected(USBD_HandleTypeDef  *pdev __attribute__((unused)))
{
  return USBD_OK;
}

/**
* @brief  USBD_DevDisconnected 
*         Handle device disconnection event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_DevDisconnected(USBD_HandleTypeDef  *pdev)
{
  /* Free Class Resources */
  pdev->dev_state = USBD_STATE_DEFAULT;
  pdev->pEventSource->UsbEventSender.raiseEvent(stm32plus::usb::DeviceClassSdkDeInitEvent(pdev->dev_config));
   
  return USBD_OK;
}
