/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0_42
#error This class can only be used with the STM32F042 series
#endif


#define USB_CONFIG_REMOTE_WAKEUP                           2
#define USB_CONFIG_SELF_POWERED                            1

#define USB_FEATURE_EP_HALT                                0
#define USB_FEATURE_REMOTE_WAKEUP                          1
#define USB_FEATURE_TEST_MODE                              2

#define  USBD_IDX_LANGID_STR                            0x00
#define  USBD_IDX_MFC_STR                               0x01
#define  USBD_IDX_PRODUCT_STR                           0x02
#define  USBD_IDX_SERIAL_STR                            0x03
#define  USBD_IDX_CONFIG_STR                            0x04
#define  USBD_IDX_INTERFACE_STR                         0x05

#define  USB_DESC_TYPE_DEVICE                              1
#define  USB_DESC_TYPE_CONFIGURATION                       2
#define  USB_DESC_TYPE_STRING                              3
#define  USB_DESC_TYPE_INTERFACE                           4
#define  USB_DESC_TYPE_ENDPOINT                            5
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    6
#define  USB_DESC_TYPE_HID_DESCRIPTOR                   0x21
#define  USB_DESC_TYPE_HID_REPORT                       0x22

#define  USB_REQ_RECIPIENT_DEVICE                       0x00
#define  USB_REQ_RECIPIENT_INTERFACE                    0x01
#define  USB_REQ_RECIPIENT_ENDPOINT                     0x02
#define  USB_REQ_RECIPIENT_MASK                         0x03

#define  USB_REQ_TYPE_STANDARD                          0x00
#define  USB_REQ_TYPE_CLASS                             0x20
#define  USB_REQ_TYPE_VENDOR                            0x40
#define  USB_REQ_TYPE_MASK                              0x60

#define  USB_REQ_GET_STATUS                             0x00
#define  USB_REQ_CLEAR_FEATURE                          0x01
#define  USB_REQ_SET_FEATURE                            0x03
#define  USB_REQ_SET_ADDRESS                            0x05
#define  USB_REQ_GET_DESCRIPTOR                         0x06
#define  USB_REQ_SET_DESCRIPTOR                         0x07
#define  USB_REQ_GET_CONFIGURATION                      0x08
#define  USB_REQ_SET_CONFIGURATION                      0x09
#define  USB_REQ_GET_INTERFACE                          0x0A
#define  USB_REQ_SET_INTERFACE                          0x0B
#define  USB_REQ_SYNCH_FRAME                            0x0C

#define CUSTOM_HID_REQ_SET_PROTOCOL          0x0B
#define CUSTOM_HID_REQ_GET_PROTOCOL          0x03

#define CUSTOM_HID_REQ_SET_IDLE              0x0A
#define CUSTOM_HID_REQ_GET_IDLE              0x02

#define CUSTOM_HID_REQ_SET_REPORT            0x09
#define CUSTOM_HID_REQ_GET_REPORT            0x01

#define GPIO_AF2_USB ((uint8_t)0x02)

/*!< USB Clock source selection */
#define  RCC_CFGR3_USBSW                     ((uint32_t)0x00000080)        /*!< USBSW bits */

#define  RCC_CFGR3_USBSW_HSI48               ((uint32_t)0x00000000)        /*!< HSI48 oscillator clock used as USB clock source */
#define  RCC_CFGR3_USBSW_PLLCLK              ((uint32_t)0x00000080)        /*!< PLLCLK selected as USB clock source */


/******************************************************************************/
/*                                                                            */
/*                         USB Device General registers                       */
/*                                                                            */
/******************************************************************************/

#define USB_BASE              (APBPERIPH_BASE + 0x00005C00) /*!< USB_IP Peripheral Registers base address */

#define USB_CNTR                             (USB_BASE + 0x40)             /*!< Control register */
#define USB_ISTR                             (USB_BASE + 0x44)             /*!< Interrupt status register */
#define USB_FNR                              (USB_BASE + 0x48)             /*!< Frame number register */
#define USB_DADDR                            (USB_BASE + 0x4C)             /*!< Device address register */
#define USB_BTABLE                           (USB_BASE + 0x50)             /*!< Buffer Table address register */
#define USB_LPMCSR                           (USB_BASE + 0x54)             /*!< LPM Control and Status register */
#define USB_BCDR                             (USB_BASE + 0x58)             /*!< Battery Charging detector register*/

/****************************  ISTR interrupt events  *************************/
#define USB_ISTR_CTR                         ((uint16_t)0x8000)             /*!< Correct TRansfer (clear-only bit) */
#define USB_ISTR_PMAOVR                      ((uint16_t)0x4000)             /*!< DMA OVeR/underrun (clear-only bit) */
#define USB_ISTR_ERR                         ((uint16_t)0x2000)             /*!< ERRor (clear-only bit) */
#define USB_ISTR_WKUP                        ((uint16_t)0x1000)             /*!< WaKe UP (clear-only bit) */
#define USB_ISTR_SUSP                        ((uint16_t)0x0800)             /*!< SUSPend (clear-only bit) */
#define USB_ISTR_RESET                       ((uint16_t)0x0400)             /*!< RESET (clear-only bit) */
#define USB_ISTR_SOF                         ((uint16_t)0x0200)             /*!< Start Of Frame (clear-only bit) */
#define USB_ISTR_ESOF                        ((uint16_t)0x0100)             /*!< Expected Start Of Frame (clear-only bit) */
#define USB_ISTR_L1REQ                       ((uint16_t)0x0080)             /*!< LPM L1 state request  */
#define USB_ISTR_DIR                         ((uint16_t)0x0010)             /*!< DIRection of transaction (read-only bit)  */
#define USB_ISTR_EP_ID                       ((uint16_t)0x000F)             /*!< EndPoint IDentifier (read-only bit)  */

#define USB_CLR_CTR                          (~USB_ISTR_CTR)             /*!< clear Correct TRansfer bit */
#define USB_CLR_PMAOVR                       (~USB_ISTR_PMAOVR)          /*!< clear DMA OVeR/underrun bit*/
#define USB_CLR_ERR                          (~USB_ISTR_ERR)             /*!< clear ERRor bit */
#define USB_CLR_WKUP                         (~USB_ISTR_WKUP)            /*!< clear WaKe UP bit */
#define USB_CLR_SUSP                         (~USB_ISTR_SUSP)            /*!< clear SUSPend bit */
#define USB_CLR_RESET                        (~USB_ISTR_RESET)           /*!< clear RESET bit */
#define USB_CLR_SOF                          (~USB_ISTR_SOF)             /*!< clear Start Of Frame bit */
#define USB_CLR_ESOF                         (~USB_ISTR_ESOF)            /*!< clear Expected Start Of Frame bit */
#define USB_CLR_L1REQ                        (~USB_ISTR_L1REQ)           /*!< clear LPM L1  bit */

/*************************  CNTR control register bits definitions  ***********/
#define USB_CNTR_CTRM                        ((uint16_t)0x8000)             /*!< Correct TRansfer Mask */
#define USB_CNTR_PMAOVRM                     ((uint16_t)0x4000)             /*!< DMA OVeR/underrun Mask */
#define USB_CNTR_ERRM                        ((uint16_t)0x2000)             /*!< ERRor Mask */
#define USB_CNTR_WKUPM                       ((uint16_t)0x1000)             /*!< WaKe UP Mask */
#define USB_CNTR_SUSPM                       ((uint16_t)0x0800)             /*!< SUSPend Mask */
#define USB_CNTR_RESETM                      ((uint16_t)0x0400)             /*!< RESET Mask   */
#define USB_CNTR_SOFM                        ((uint16_t)0x0200)             /*!< Start Of Frame Mask */
#define USB_CNTR_ESOFM                       ((uint16_t)0x0100)             /*!< Expected Start Of Frame Mask */
#define USB_CNTR_L1REQM                      ((uint16_t)0x0080)             /*!< LPM L1 state request interrupt mask */
#define USB_CNTR_L1RESUME                    ((uint16_t)0x0020)             /*!< LPM L1 Resume request */
#define USB_CNTR_RESUME                      ((uint16_t)0x0010)             /*!< RESUME request */
#define USB_CNTR_FSUSP                       ((uint16_t)0x0008)             /*!< Force SUSPend */
#define USB_CNTR_LPMODE                      ((uint16_t)0x0004)             /*!< Low-power MODE */
#define USB_CNTR_PDWN                        ((uint16_t)0x0002)             /*!< Power DoWN */
#define USB_CNTR_FRES                        ((uint16_t)0x0001)             /*!< Force USB RESet */

/*************************  BCDR control register bits definitions  ***********/
#define USB_BCDR_DPPU                        ((uint16_t)0x8000)             /*!< DP Pull-up Enable */
#define USB_BCDR_PS2DET                      ((uint16_t)0x0080)             /*!< PS2 port or proprietary charger detected */
#define USB_BCDR_SDET                        ((uint16_t)0x0040)             /*!< Secondary detection (SD) status */
#define USB_BCDR_PDET                        ((uint16_t)0x0020)             /*!< Primary detection (PD) status */
#define USB_BCDR_DCDET                       ((uint16_t)0x0010)             /*!< Data contact detection (DCD) status */
#define USB_BCDR_SDEN                        ((uint16_t)0x0008)             /*!< Secondary detection (SD) mode enable */
#define USB_BCDR_PDEN                        ((uint16_t)0x0004)             /*!< Primary detection (PD) mode enable */
#define USB_BCDR_DCDEN                       ((uint16_t)0x0002)             /*!< Data contact detection (DCD) mode enable */
#define USB_BCDR_BCDEN                       ((uint16_t)0x0001)             /*!< Battery charging detector (BCD) enable */

/***************************  LPM register bits definitions  ******************/
#define USB_LPMCSR_BESL                      ((uint16_t)0x00F0)             /*!< BESL value received with last ACKed LPM Token  */
#define USB_LPMCSR_REMWAKE                   ((uint16_t)0x0008)             /*!< bRemoteWake value received with last ACKed LPM Token */
#define USB_LPMCSR_LPMACK                    ((uint16_t)0x0002)             /*!< LPM Token acknowledge enable*/
#define USB_LPMCSR_LMPEN                     ((uint16_t)0x0001)             /*!< LPM support enable  */

/********************  FNR Frame Number Register bit definitions   ************/
#define USB_FNR_RXDP                         ((uint16_t)0x8000)             /*!< status of D+ data line */
#define USB_FNR_RXDM                         ((uint16_t)0x4000)             /*!< status of D- data line */
#define USB_FNR_LCK                          ((uint16_t)0x2000)             /*!< LoCKed */
#define USB_FNR_LSOF                         ((uint16_t)0x1800)             /*!< Lost SOF */
#define USB_FNR_FN                           ((uint16_t)0x07FF)             /*!< Frame Number */

/********************  DADDR Device ADDRess bit definitions    ****************/
#define USB_DADDR_EF                         ((uint8_t)0x80)                /*!< USB device address Enable Function */
#define USB_DADDR_ADD                        ((uint8_t)0x7F)                /*!< USB device address */

/******************************  Endpoint register    *************************/
#define USB_EP0R                             USB_BASE                   /*!< endpoint 0 register address */
#define USB_EP1R                             (USB_BASE + 0x04)           /*!< endpoint 1 register address */
#define USB_EP2R                             (USB_BASE + 0x08)           /*!< endpoint 2 register address */
#define USB_EP3R                             (USB_BASE + 0x0C)           /*!< endpoint 3 register address */
#define USB_EP4R                             (USB_BASE + 0x10)           /*!< endpoint 4 register address */
#define USB_EP5R                             (USB_BASE + 0x14)           /*!< endpoint 5 register address */
#define USB_EP6R                             (USB_BASE + 0x18)           /*!< endpoint 6 register address */
#define USB_EP7R                             (USB_BASE + 0x1C)           /*!< endpoint 7 register address */
/* bit positions */
#define USB_EP_CTR_RX                        ((uint16_t)0x8000)             /*!<  EndPoint Correct TRansfer RX */
#define USB_EP_DTOG_RX                       ((uint16_t)0x4000)             /*!<  EndPoint Data TOGGLE RX */
#define USB_EPRX_STAT                        ((uint16_t)0x3000)             /*!<  EndPoint RX STATus bit field */
#define USB_EP_SETUP                         ((uint16_t)0x0800)             /*!<  EndPoint SETUP */
#define USB_EP_T_FIELD                       ((uint16_t)0x0600)             /*!<  EndPoint TYPE */
#define USB_EP_KIND                          ((uint16_t)0x0100)             /*!<  EndPoint KIND */
#define USB_EP_CTR_TX                        ((uint16_t)0x0080)             /*!<  EndPoint Correct TRansfer TX */
#define USB_EP_DTOG_TX                       ((uint16_t)0x0040)             /*!<  EndPoint Data TOGGLE TX */
#define USB_EPTX_STAT                        ((uint16_t)0x0030)             /*!<  EndPoint TX STATus bit field */
#define USB_EPADDR_FIELD                     ((uint16_t)0x000F)             /*!<  EndPoint ADDRess FIELD */

/* EndPoint REGister MASK (no toggle fields) */
#define USB_EPREG_MASK     (USB_EP_CTR_RX|USB_EP_SETUP|USB_EP_T_FIELD|USB_EP_KIND|USB_EP_CTR_TX|USB_EPADDR_FIELD)
                                                                               /*!< EP_TYPE[1:0] EndPoint TYPE */
#define USB_EP_TYPE_MASK                     ((uint16_t)0x0600)             /*!< EndPoint TYPE Mask */
#define USB_EP_BULK                          ((uint16_t)0x0000)             /*!< EndPoint BULK */
#define USB_EP_CONTROL                       ((uint16_t)0x0200)             /*!< EndPoint CONTROL */
#define USB_EP_ISOCHRONOUS                   ((uint16_t)0x0400)             /*!< EndPoint ISOCHRONOUS */
#define USB_EP_INTERRUPT                     ((uint16_t)0x0600)             /*!< EndPoint INTERRUPT */
#define USB_EP_T_MASK      (~USB_EP_T_FIELD & USB_EPREG_MASK)

#define USB_EPKIND_MASK    (~USB_EP_KIND & USB_EPREG_MASK)            /*!< EP_KIND EndPoint KIND */
                                                                               /*!< STAT_TX[1:0] STATus for TX transfer */
#define USB_EP_TX_DIS                        ((uint16_t)0x0000)             /*!< EndPoint TX DISabled */
#define USB_EP_TX_STALL                      ((uint16_t)0x0010)             /*!< EndPoint TX STALLed */
#define USB_EP_TX_NAK                        ((uint16_t)0x0020)             /*!< EndPoint TX NAKed */
#define USB_EP_TX_VALID                      ((uint16_t)0x0030)             /*!< EndPoint TX VALID */
#define USB_EPTX_DTOG1                       ((uint16_t)0x0010)             /*!< EndPoint TX Data TOGgle bit1 */
#define USB_EPTX_DTOG2                       ((uint16_t)0x0020)             /*!< EndPoint TX Data TOGgle bit2 */
#define USB_EPTX_DTOGMASK  (USB_EPTX_STAT|USB_EPREG_MASK)
                                                                               /*!< STAT_RX[1:0] STATus for RX transfer */
#define USB_EP_RX_DIS                        ((uint16_t)0x0000)             /*!< EndPoint RX DISabled */
#define USB_EP_RX_STALL                      ((uint16_t)0x1000)             /*!< EndPoint RX STALLed */
#define USB_EP_RX_NAK                        ((uint16_t)0x2000)             /*!< EndPoint RX NAKed */
#define USB_EP_RX_VALID                      ((uint16_t)0x3000)             /*!< EndPoint RX VALID */
#define USB_EPRX_DTOG1                       ((uint16_t)0x1000)             /*!< EndPoint RX Data TOGgle bit1 */
#define USB_EPRX_DTOG2                       ((uint16_t)0x2000)             /*!< EndPoint RX Data TOGgle bit1 */
#define USB_EPRX_DTOGMASK  (USB_EPRX_STAT|USB_EPREG_MASK)

typedef struct
{
  __IO uint16_t EP0R;            /*!< USB Endpoint 0 register,                Address offset: 0x00 */
  __IO uint16_t RESERVED0;       /*!< Reserved */
  __IO uint16_t EP1R;            /*!< USB Endpoint 1 register,                Address offset: 0x04 */
  __IO uint16_t RESERVED1;       /*!< Reserved */
  __IO uint16_t EP2R;            /*!< USB Endpoint 2 register,                Address offset: 0x08 */
  __IO uint16_t RESERVED2;       /*!< Reserved */
  __IO uint16_t EP3R;            /*!< USB Endpoint 3 register,                Address offset: 0x0C */
  __IO uint16_t RESERVED3;       /*!< Reserved */
  __IO uint16_t EP4R;            /*!< USB Endpoint 4 register,                Address offset: 0x10 */
  __IO uint16_t RESERVED4;       /*!< Reserved */
  __IO uint16_t EP5R;            /*!< USB Endpoint 5 register,                Address offset: 0x14 */
  __IO uint16_t RESERVED5;       /*!< Reserved */
  __IO uint16_t EP6R;            /*!< USB Endpoint 6 register,                Address offset: 0x18 */
  __IO uint16_t RESERVED6;       /*!< Reserved */
  __IO uint16_t EP7R;            /*!< USB Endpoint 7 register,                Address offset: 0x1C */
  __IO uint16_t RESERVED7[17];   /*!< Reserved */
  __IO uint16_t CNTR;            /*!< Control register,                       Address offset: 0x40 */
  __IO uint16_t RESERVED8;       /*!< Reserved */
  __IO uint16_t ISTR;            /*!< Interrupt status register,              Address offset: 0x44 */
  __IO uint16_t RESERVED9;       /*!< Reserved */
  __IO uint16_t FNR;             /*!< Frame number register,                  Address offset: 0x48 */
  __IO uint16_t RESERVEDA;       /*!< Reserved */
  __IO uint16_t DADDR;           /*!< Device address register,                Address offset: 0x4C */
  __IO uint16_t RESERVEDB;       /*!< Reserved */
  __IO uint16_t BTABLE;          /*!< Buffer Table address register,          Address offset: 0x50 */
  __IO uint16_t RESERVEDC;       /*!< Reserved */
  __IO uint16_t LPMCSR;          /*!< LPM Control and Status register,        Address offset: 0x54 */
  __IO uint16_t RESERVEDD;       /*!< Reserved */
  __IO uint16_t BCDR;            /*!< Battery Charging detector register,     Address offset: 0x58 */
  __IO uint16_t RESERVEDE;       /*!< Reserved */
} USB_TypeDef;


#define USBR ((USB_TypeDef *)USB_BASE)
