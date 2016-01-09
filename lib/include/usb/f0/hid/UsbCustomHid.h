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


namespace stm32plus {

  /**
   * The UsbTxComplete event is raised when a report has successfully been sent to the host
   */

  DECLARE_EVENT_SIGNATURE(UsbTxComplete,void(uint8_t,uint16_t));


  /**
   * The UxbRx event is raised when a new report has just arrived from the host.
   */

  DECLARE_EVENT_SIGNATURE(UsbRx,void(uint8_t,const uint16_t *,uint16_t));


  /**
   * The UsbStatus event is raised when there has been some change in the overall subsystem status
   */

  DECLARE_EVENT_SIGNATURE(UsbStatus,void(UsbStatusType));


  /**
   * The UsbCustomHid class handles all the interaction with the USB peripheral. You are provided with one
   * IN and one OUT endpoint. You may customize the size of the reports and the USB device parameters such
   * as the identifying strings and the PID and VID. This is done by creating a type (usually a struct) and
   * declaring constants inside of it. See the example code for how to do this. You may also include any
   * optional USB features in your type declaration such as 'UsbSelfPoweredFeature' if your device is
   * self powered.
   *
   * A typical type declaration might be:
   *   typedef UsbCustomHid<MyUsbConfiguration> MyUsb;
   *
   * Once declared and constructed you must call start() to initiate host enumeration. You can call stop()
   * or use the class destructor to disconnect from the host and release resources.
   *
   * Two reports are declared, id #1 is for reports TO the host (IN direction) and report id #2 is for
   * reports FROM the host (OUT direction). A report is nothing more than USB's way of identifying a data
   * packet that's transferred to/from the host.
   *
   * @tparam TConfiguration The name of a type (usually a struct) that contains the configuration constants.
   * @tparam Features The variadic list of feature type names, if any
   */

  template<class TConfiguration,class... Features>
  class UsbCustomHid : public Features... {

    public:

      /**
       * Forward declare the descriptors that we maintain
       */

      static const uint8_t _deviceDescriptor[18];
      static const uint8_t _configurationDescriptor[41];
      static const uint8_t _reportDescriptor[32];
      static const uint8_t _hidDescriptor[9];
      static const uint8_t _langIdDescriptor[4];

      /**
       * Error codes
       */

      enum {
        E_UNCONFIGURED = 1,     //!< You tried to communicate with the host while this device was not ready
      };

      /**
       * Declare the events that we will manage
       */

      DECLARE_EVENT_SOURCE(UsbTxComplete);
      DECLARE_EVENT_SOURCE(UsbRx);
      DECLARE_EVENT_SOURCE(UsbStatus);

    protected:

      /**
       * Constants that we need
       */

      enum {
        BTABLE_BASE = 0x40006000,           //!< Base address of BTABLE in peripheral memory

        CONTROL_OUT_PMA_ADDRESS = 0x18,     //!< Packet Memory Address of the control OUT endpoint
        CONTROL_IN_PMA_ADDRESS = 0x58,      //!< Packet Memory Address of the control IN endpoint
        CONTROL_MAX_PACKET_SIZE = 0x40,     //!< Maximum control packet size

        IN_ENDPOINT_ADDRESS = 0x81,         //!< Address of the IN endpoint
        IN_ENDPOINT_PMA_ADDRESS = 0x98,     //!< Packet Memory Address of the IN endpoint

        OUT_ENDPOINT_ADDRESS = 0x1,         //!< Address of the OUT endpoint
        OUT_ENDPOINT_PMA_ADDRESS = 0xD8     //!< Packet Memory Address of the OUT endpoint
      };

      /**
       * Possible device states
       */

      enum class DeviceStateType {
        NONE,               //!< Object constructed but not started
        DEFAULT,            //!< Device is starting, or has disconnected
        ADDRESSED,          //!< We've received an address from the host
        CONFIGURED,         //!< Enumeration is complete, we can talk to the host
        SUSPENDED           //!< The host has suspended us
      } _deviceState,_savedState;

      /**
       * The possible states for the control endpoint
       */

      enum class ControlEndpointStateType {
        IDLE,         //!< IDLE
        SETUP,        //!< SETUP
        DATA_IN,      //!< DATA_IN
        DATA_OUT,     //!< DATA_OUT
        STATUS_IN,    //!< STATUS_IN
        STATUS_OUT,   //!< STATUS_OUT
        STALL         //!< STALL
      } _controlEndpointState;


      /*
       * What an endpoint buffer description entry looks like in the BDT
       */

      struct EndpointBufferDescription {
        UsbBufferDescriptionTableEntry tx;
        UsbBufferDescriptionTableEntry rx;
      } __attribute__((packed));

      UsbInEndpointData _inEndpointData[2];   // 2 user endpoints in this implementation
      uint16_t _setupDataLength;
      uint8_t _protocol;
      uint8_t _idleState;
      uint8_t _altSetting;
      uint8_t _address;
      uint8_t _configurationIndex;
      bool _remoteWakeup;
      bool _reportAvailable;

    protected:
      void onInterrupt();

      uint16_t setRxStatus(uint16_t reg,uint16_t status) const;
      uint16_t setTxStatus(uint16_t reg,uint16_t status) const;
      void setRxEndpointStatus(volatile uint16_t *epreg,uint16_t status) const;
      void setTxEndpointStatus(volatile uint16_t *epreg,uint16_t status) const;
      uint16_t getRxEndpointStatus(volatile uint16_t *epreg) const;
      uint16_t getTxEndpointStatus(volatile uint16_t *epreg) const;
      void controlEndpointError() const;
      void stallEndpoint(uint8_t epAddr) const;
      void unstallEndpoint(uint8_t epAddr) const;
      bool isStalled(uint8_t epAddr) const;
      void setDeviceState(DeviceStateType newState);
      void openDeviceEndpoints() const;
      void openTxEndpoint(volatile uint16_t *reg,uint8_t addr,uint16_t type,uint16_t pmaAddress) const;
      void openRxEndpoint(volatile uint16_t *reg,uint8_t addr,uint16_t type,uint16_t pmaAddress,uint16_t maxPacketSize) const;
      void closeDeviceEndpoints() const;
      void closeTxEndpoint(volatile uint16_t *reg) const;
      void closeRxEndpoint(volatile uint16_t *reg) const;

      void handleControlOutTransfer();
      void handleControlInTransfer();
      void handleControlSetupOutTransfer();
      void handleControlDataOutTransfer();
      void handleEndpointInTransfer(volatile uint16_t *reg,uint8_t endpointIndex);
      void handleEndpointOutTransfer(volatile uint16_t *reg,uint8_t endpointIndex);

      void handleDeviceRequest(const UsbSetupHeader& header);
      void handleInterfaceRequest(const UsbSetupHeader& header);
      void handleEndpointRequest(const UsbSetupHeader& header);
      void handleGetDescriptor(const UsbSetupHeader& header);
      void handleSetAddress(const UsbSetupHeader& header);
      void handleSetConfig(const UsbSetupHeader& header);
      void handleGetConfig(const UsbSetupHeader& header);
      void handleGetStatus();
      void handleSetFeature(const UsbSetupHeader& header);
      void handleClearFeature(const UsbSetupHeader& header);
      void handleGetDescriptorString(const UsbSetupHeader& header,uint8_t*& dataToSend,uint16_t& length);
      void handleSetup(const UsbSetupHeader& header);
      void handleClassSetup(const UsbSetupHeader& header);
      void handleStandardSetup(const UsbSetupHeader& header);

      void sendControlData(const void *data,uint16_t length);
      void sendControlZeroLengthPacket();
      void sendData(uint8_t endpointIndex,uint16_t pmaInAddress,uint16_t maxPacketSize,const void *data,uint16_t length);
      void continueSendData(uint8_t endpointIndex);
      void copyReceivedData(void *dest,uint32_t src,uint16_t byteCount) const;

      void onCorrectTransfer();
      void onReset();
      void onError() const;
      void onPacketMemoryOverUnderFlow() const;
      void onWakeup();
      void onSuspend();
      void onStartOfFrame() const;
      void onExpectedStartOfFrame() const;

      void setAddress(uint8_t address);
      void setInterruptMask() const;
      void clearInterrupts() const;
      void openControlEndpoints();
      void closeControlEndpoints();

    public:
      UsbCustomHid();
      ~UsbCustomHid();

      void reset() const;
      void start();
      void stop();

      bool sendReport(const void *report,uint16_t length);
      DeviceStateType getDeviceState() const;
  };

  #define USBR_BDT ((volatile EndpointBufferDescription *)BTABLE_BASE)


  /**
   * Report definition. 1 report each for IN and OUT
   */

  template<class TConfiguration,class... Features>
  const uint8_t UsbCustomHid<TConfiguration,Features...>::_reportDescriptor[32] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x00,                    // USAGE (Undefined)
    0xa1, 0x01,                    // COLLECTION (Application)

    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)

    // IN report

    0x85, 0x01,                    //   REPORT_ID (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, TConfiguration::IN_ENDPOINT_MAX_PACKET_SIZE-1, // REPORT_COUNT (this is the byte length)
    0x09, 0x00,                    //   USAGE (Undefined)
    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)

    // OUT report

    0x85, 0x02,                    //   REPORT_ID (2)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, TConfiguration::OUT_ENDPOINT_MAX_PACKET_SIZE-1, // REPORT_COUNT (this is the byte length)
    0x09, 0x00,                    //   USAGE (Undefined)
    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)

    0xc0                           // END_COLLECTION
  };


  /**
   * The 18 byte device descriptor
   */

  template<class TConfiguration,class... Features>
  const uint8_t UsbCustomHid<TConfiguration,Features...>::_deviceDescriptor[18]={
    sizeof(UsbCustomHid<TConfiguration,Features...>::_deviceDescriptor),  // bLength
    USB_DESC_TYPE_DEVICE,       // bDescriptorType
    0x00,0x02,                  // bcdUSB

    0x00,                       // bDeviceClass
    0x00,                       // bDeviceSubClass
    0x00,                       // bDeviceProtocol
    64,                         // bMaxPacketSize

    TConfiguration::VID & 0xff, // idVendor
    TConfiguration::VID >> 8,   // idVendor
    TConfiguration::PID & 0xff, // idProduct
    TConfiguration::PID >> 8,   // idProduct

    0x00,0x02,                  // bcdDevice rel. 2.00

    USBD_IDX_MFC_STR,           // Index of manufacturer string
    USBD_IDX_PRODUCT_STR,       // Index of product string
    USBD_IDX_SERIAL_STR,        // Index of serial number string
    1                           // bNumConfigurations
  };


  /**
   * The configuration descriptor
   */

  template<class TConfiguration,class... Features>
  const uint8_t UsbCustomHid<TConfiguration,Features...>::_configurationDescriptor[41]={

    // configuration descriptor header

    0x09,                           // bLength: Configuration Descriptor size
    USB_DESC_TYPE_CONFIGURATION,    // bDescriptorType: Configuration
    sizeof(UsbCustomHid<TConfiguration,Features...>::_configurationDescriptor),0,  // wTotalLength
    0x01,                           // bNumInterfaces
    0x01,                           // bConfigurationValue
    0x00,                           // iConfiguration
    0x80 | TConfiguration::CONFIGURATION_FLAGS,  // bmAttributes: bus powered
    TConfiguration::MILLIAMPS/2,    // MaxPower 100 mA: this current is used for detecting Vbus

    // interface descriptor

    0x09,                           // bLength: Interface Descriptor size
    USB_DESC_TYPE_INTERFACE,        // bDescriptorType: Interface descriptor type
    0x00,                           // bInterfaceNumber: Number of Interface
    0x00,                           // bAlternateSetting: Alternate setting
    0x02,                           // bNumEndpoints
    0x03,                           // bInterfaceClass: CUSTOM_HID
    0x00,                           // bInterfaceSubClass : 1=BOOT, 0=no boot
    0x00,                           // nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse
    0x00,                           // iInterface: Index of string descriptor

    // HID descriptor

    0x09,                           // bLength: CUSTOM_HID Descriptor size
    USB_DESC_TYPE_HID_DESCRIPTOR,   // bDescriptorType: CUSTOM_HID
    0x11,0x01,                      // bcdHID HID Class Spec release number (1.11)
    0x00,                           // bCountryCode: Hardware target country
    0x01,                           // bNumDescriptors: Number of class descriptors to follow

    0x22,                           // bDescriptorType
    sizeof(UsbCustomHid<TConfiguration,Features...>::_reportDescriptor),0, // wItemLength: Total length of Report descriptor

    // IN endpoint descriptor

    0x07,                           // bLength: Endpoint Descriptor size
    USB_DESC_TYPE_ENDPOINT,         // bDescriptorType
    IN_ENDPOINT_ADDRESS,            // bEndpointAddress: Endpoint Address (IN)
    0x03,                           // bmAttributes: Interrupt endpoint
    TConfiguration::IN_ENDPOINT_MAX_PACKET_SIZE,0,  // wMaxPacketSize: 2 Byte max
    0x20,                           // bInterval: Polling Interval (20 ms)

    // OUT endpoint descriptor

    0x07,                           // bLength: Endpoint Descriptor size
    USB_DESC_TYPE_ENDPOINT,         // bDescriptorType
    OUT_ENDPOINT_ADDRESS,           // bEndpointAddress: Endpoint Address (OUT)
    0x03,                           // bmAttributes: Interrupt endpoint
    TConfiguration::OUT_ENDPOINT_MAX_PACKET_SIZE,0, // wMaxPacketSize: 2 Bytes max
    0x20,                           // bInterval: Polling Interval (20 ms)
  };


  /**
   * The HID descriptor (this is a copy of the descriptor embedded in the
   * above configuration descriptor)
   */

  template<class TConfiguration,class... Features>
  const uint8_t UsbCustomHid<TConfiguration,Features...>::_hidDescriptor[9]={
    0x09,                           // bLength: CUSTOM_HID Descriptor size
    USB_DESC_TYPE_HID_DESCRIPTOR,   // bDescriptorType: CUSTOM_HID
    0x11,0x01,                      // bcdHID HID Class Spec release number (1.11)
    0x00,                           // bCountryCode: Hardware target country
    0x01,                           // bNumDescriptors: Number of class descriptors to follow

    0x22,                           // bDescriptorType
    sizeof(UsbCustomHid<TConfiguration,Features...>::_reportDescriptor),0 // wItemLength: Total length of Report descriptor
  };


  /**
   * The descriptor strings
   */

  template<class TConfiguration,class... Features>
  const uint8_t UsbCustomHid<TConfiguration,Features...>::_langIdDescriptor[4] = {
    sizeof(UsbCustomHid<TConfiguration,Features...>::_langIdDescriptor),
    USB_DESC_TYPE_STRING,
    TConfiguration::LANGUAGE_ID & 0xff,
    TConfiguration::LANGUAGE_ID >> 8
  };


  /**
   * Constructor
   */

  template<class TConfiguration,class... Features>
  inline UsbCustomHid<TConfiguration,Features...>::UsbCustomHid() {

    // sanity checks

    static_assert(sizeof(EndpointBufferDescription)==8,"Invalid compiler options. sizeof(BufferDescriptionTableEntry)!=8");
    static_assert(TConfiguration::IN_ENDPOINT_MAX_PACKET_SIZE<=64,"IN_ENDPOINT_MAX_PACKET_SIZE must be no greater than 64");
    static_assert(TConfiguration::OUT_ENDPOINT_MAX_PACKET_SIZE<=64,"OUT_ENDPOINT_MAX_PACKET_SIZE must be no greater than 64");

    // nothing state

    _deviceState=DeviceStateType::NONE;

    // subscribe to USB interrupts

    usbInterrupt.UsbInterruptEventSender.insertSubscriber(
        UsbInterruptEventSourceSlot::bind(this,&UsbCustomHid<TConfiguration,Features...>::onInterrupt));

    // initialise pins

    GpioPinInitialiser::initialise(GPIOA,
                                   GPIO_Pin_11 | GPIO_Pin_12,
                                   Gpio::ALTERNATE_FUNCTION,
                                   GPIO_Speed_Level_3,
                                   Gpio::PUPD_NONE,
                                   Gpio::PUSH_PULL,
                                   GPIO_AF2_USB);
  }


  /**
   * Destructor
   */

  template<class TConfiguration,class... Features>
  inline UsbCustomHid<TConfiguration,Features...>::~UsbCustomHid() {
    stop();
  }


  /**
   * Disconnect from the host and deinitialise the peripheral
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::stop() {

    // disable endpoints

    closeDeviceEndpoints();
    closeControlEndpoints();

    // interrupts off

    Nvic::configureIrq(USB_IRQn,DISABLE);

    // tell the host that we're gone by disabling pull-up on DP

    USBR->BCDR &= ~USB_BCDR_DPPU;

    // USB clock off

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB,DISABLE);
  }


  /**
   * Set the interrupt mask
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::setInterruptMask() const {

    // set the interrupt mask

    USBR->CNTR=USB_CNTR_CTRM          // correct transfer
              | USB_CNTR_WKUPM        // wakeup
              | USB_CNTR_SUSPM        // suspend
              | USB_CNTR_ERRM         // error
              | USB_CNTR_ESOFM        // expected start of frame
              | USB_CNTR_RESETM;      // reset
  }


  /**
   * soft-reset the peripheral
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::reset() const {
    USBR->CNTR=USB_CNTR_FRES;
    USBR->CNTR=0;
  }


  /**
   * clear any pending interrupts
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::clearInterrupts() const {
    USBR->ISTR=0;
  }


  /**
   * Start (set the pullup on DP)
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::start() {

    // set address to zero and default state

    _address=0;
    setDeviceState(DeviceStateType::DEFAULT);
    _remoteWakeup=false;

    // the buffer description table is at address 0 in the packet memory

    USBR->BTABLE=0;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB,ENABLE);
    Nvic::configureIrq(USB_IRQn);

    // reset the peripheral and any pending interrupts

    reset();
    clearInterrupts();
    setInterruptMask();

    USBR->BCDR |= USB_BCDR_DPPU;

    // notify

    UsbStatusEventSender.raiseEvent(UsbStatusType::STATE_DEFAULT);
  }


  /**
   * Send a report to the host. This is an asynchronous operation. 'report' must remain in scope
   * until transmission is complete if 'length' is greater than a single packet. If the report
   * fits within a single packet then the data will be completely copied into the peripheral's internal
   * RAM when this function returns and so it may safely go out of scope.
   * @param report the report to send
   * @param length the length of the report
   * @return true if the report is scheduled for sending
   */

  template<class TConfiguration,class... Features>
  inline bool UsbCustomHid<TConfiguration,Features...>::sendReport(const void *report,uint16_t length) {

    // check if configured

    if(_deviceState!=DeviceStateType::CONFIGURED)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,E_UNCONFIGURED);

    // send and return. interrupts drive the actual transmission of the data.

    sendData(IN_ENDPOINT_ADDRESS & 0x7F,
             IN_ENDPOINT_PMA_ADDRESS,
             TConfiguration::IN_ENDPOINT_MAX_PACKET_SIZE,
             report,
             length);

    return true;
  }


  /**
   * Set the address (as sent by the host)
   * @param address, or 0 to reset
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::setAddress(uint8_t address) {

    if(address==0)
      USBR->DADDR=USB_DADDR_EF;
    else
      _address=address;
  }


  /**
   * Open the control IN and OUT endpoints
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::openControlEndpoints() {
    openTxEndpoint(&USBR->EP0R,0,USB_EP_CONTROL,CONTROL_IN_PMA_ADDRESS);
    openRxEndpoint(&USBR->EP0R,0,USB_EP_CONTROL,CONTROL_OUT_PMA_ADDRESS,CONTROL_MAX_PACKET_SIZE);
  }


  /**
   * Close the control IN and OUT endpoints
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::closeControlEndpoints() {
    closeTxEndpoint(&USBR->EP0R);
    closeRxEndpoint(&USBR->EP0R);
  }


  /**
   * Open an RX (OUT) endpoint
   * @param reg the endpoint register address
   * @param addr the endpoint address
   * @param type the endpoint type (e.g. USB_EP_CONTROL)
   * @param pmaAddress the address in packet memory for this endpoint
   * @param maxPacketSize the maximum packet size
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::openRxEndpoint(volatile uint16_t *reg,uint8_t addr,uint16_t type,uint16_t pmaAddress,uint16_t maxPacketSize) const {

    // set up the endpoint type and address

    *reg=(*reg & USB_EP_T_MASK) | type;
    *reg=(*reg & USB_EPREG_MASK) | addr | USB_EP_CTR_RX;

    // set up the RX address in the BTABLE and initialise first packet read

    USBR_BDT[addr].rx.addr=pmaAddress;
    USBR_BDT[addr].rx.setRxCount(maxPacketSize);

    // if DTOG_RX is 1 then we need to write 1 to toggle it to zero

    if((*reg & USB_EP_DTOG_RX)!=0)
      *reg=(*reg & USB_EPREG_MASK) | USB_EP_CTR_RX | USB_EP_DTOG_RX;

    // inititate reception of the first packet

    setRxEndpointStatus(reg,USB_EP_RX_VALID);
  }


  /**
   * Open a TX (IN) endpoint
   * @param reg the endpoint register address
   * @param addr the endpoint address
   * @param type the endpoint type (e.g. USB_EP_CONTROL)
   * @param pmaAddress the address in packet memory for this endpoint
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::openTxEndpoint(volatile uint16_t *reg,uint8_t addr,uint16_t type,uint16_t pmaAddress) const {

    // set up the endpoint type and address

    *reg=(*reg & USB_EP_T_MASK) | type;
    *reg=(*reg & USB_EPREG_MASK) | addr | USB_EP_CTR_TX;

    // set up the TX address in the BTABLE

    USBR_BDT[addr].tx.addr=pmaAddress;

    // if DTOG_TX is 1 then we need to write 1 to toggle it to zero

    if((*reg & USB_EP_DTOG_TX)!=0)
      *reg=(*reg & USB_EPREG_MASK) | USB_EP_CTR_TX | USB_EP_DTOG_TX;

    // NAK the TX endpoint (nothing to go yet)

    setTxEndpointStatus(reg,USB_EP_TX_NAK);
  }


  /**
   * Open the device endpoints
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::openDeviceEndpoints() const {
    openTxEndpoint(&USBR->EP1R,1,USB_EP_INTERRUPT,IN_ENDPOINT_PMA_ADDRESS);
    openRxEndpoint(&USBR->EP1R,1,USB_EP_INTERRUPT,OUT_ENDPOINT_PMA_ADDRESS,TConfiguration::OUT_ENDPOINT_MAX_PACKET_SIZE);
  }


  /**
   * Close the device endpoints
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::closeDeviceEndpoints() const {
    closeTxEndpoint(&USBR->EP1R);
    closeRxEndpoint(&USBR->EP1R);
  }


  /**
   * Close an RX endpoint
   * @param reg the endpoint register address
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::closeRxEndpoint(volatile uint16_t *reg) const {

    // reset DTOG_RX then disable the endpoint

    if((*reg & USB_EP_DTOG_RX)!=0)
      *reg=(*reg & USB_EPREG_MASK) | USB_EP_CTR_RX | USB_EP_CTR_TX | USB_EP_DTOG_RX;

    setRxEndpointStatus(reg,USB_EP_RX_DIS);
  }


  /**
   * Close a TX endpoint
   * @param reg the endpoint register address
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::closeTxEndpoint(volatile uint16_t *reg) const {

    // reset DTOG_TX then disable the endpoint

    if((*reg & USB_EP_DTOG_TX)!=0)
      *reg=(*reg & USB_EPREG_MASK) | USB_EP_CTR_RX | USB_EP_CTR_TX | USB_EP_DTOG_TX;

    setTxEndpointStatus(reg,USB_EP_TX_DIS);
  }


  /**
   * Modify EP RX status to desired value by twiddling the insane DTOG bits
   * @param reg The current endpoint register value
   * @param status The new status
   * @return The correct modified register value
   */

  template<class TConfiguration,class... Features>
  inline uint16_t UsbCustomHid<TConfiguration,Features...>::setRxStatus(uint16_t reg,uint16_t status) const {

    // if current reg bit is not equal to the desired reg bit then
    // set 1 in the reg to toggle it

    if((USB_EPRX_DTOG1 & status)!=0)
      reg^=USB_EPRX_DTOG1;

    if((USB_EPRX_DTOG2 & status)!=0)
      reg^=USB_EPRX_DTOG2;

    return reg;
  }


  /**
   * Modify EP TX status to desired value
   * @param reg The current endpoint register value
   * @param status The new status
   * @return The correct modified register value
   */

  template<class TConfiguration,class... Features>
  inline uint16_t UsbCustomHid<TConfiguration,Features...>::setTxStatus(uint16_t reg,uint16_t status) const {

    // if current reg bit is not equal to the desired reg bit then
    // set 1 in the reg to toggle it

    if((USB_EPTX_DTOG1 & status)!=0)
      reg^=USB_EPTX_DTOG1;

    if((USB_EPTX_DTOG2 & status)!=0)
      reg^=USB_EPTX_DTOG2;

    return reg;
  }


  /**
   * Stall TX and RX control endpoints
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::controlEndpointError() const {
    setRxEndpointStatus(&USBR->EP0R,USB_EP_RX_STALL);
    setTxEndpointStatus(&USBR->EP0R,USB_EP_TX_STALL);
  }


  /**
   * Set a new RX endpoint status. Note that the CTR_RX/TX bits are not actually being
   * set because writing 1 does nothing.
   * @param epreg The address of the endpoint register
   * @param status The new status
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::setRxEndpointStatus(volatile uint16_t *epreg,uint16_t status) const {

    uint16_t value;

    value=*epreg & USB_EPRX_DTOGMASK;
    *epreg=setRxStatus(value,status) | USB_EP_CTR_RX | USB_EP_CTR_TX;
  }


  /**
   * Set a new TX endpoint status. Note that the CTR_RX/TX bits are not actually being
   * set because writing 1 does nothing.
   * @param epreg The address of the endpoint register
   * @param status The new status
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::setTxEndpointStatus(volatile uint16_t *epreg,uint16_t status) const {

    uint16_t value;

    value=*epreg & USB_EPTX_DTOGMASK;
    *epreg=setTxStatus(value,status) | USB_EP_CTR_RX | USB_EP_CTR_TX;
  }


  /**
   * Get the status of a RX endpoint
   * @param epreg The address of the endpoint register
   * @return The RX endpoint status
   */

  template<class TConfiguration,class... Features>
  inline uint16_t UsbCustomHid<TConfiguration,Features...>::getRxEndpointStatus(volatile uint16_t *epreg) const  {
    return *epreg & USB_EPRX_STAT;
  }


  /**
   * Get the status of a TX endpoint
   * @param epreg The address of the endpoint register
   * @return The TX endpoint status
   */

  template<class TConfiguration,class... Features>
  inline uint16_t UsbCustomHid<TConfiguration,Features...>::getTxEndpointStatus(volatile uint16_t *epreg) const {
    return *epreg & USB_EPTX_STAT;
  }


  /**
   * Handle the control OUT transfer
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleControlOutTransfer() {

    if((USBR->EP0R & USB_EP_SETUP)!=0)
      handleControlSetupOutTransfer();
    else if((USBR->EP0R & USB_EP_CTR_RX)!=0)
      handleControlDataOutTransfer();
  }


  /**
   * Handle a control setup OUT transfer
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleControlSetupOutTransfer() {

    uint32_t setup[12];

    // get the length from the BTABLE and initialise control state

    _setupDataLength=USBR_BDT[0].rx.getCount();
    _controlEndpointState=ControlEndpointStateType::SETUP;

    // copy the received data

    copyReceivedData(setup,BTABLE_BASE+0x18,_setupDataLength);

    // clear the 'correct transfer for reception' bit for this endpoint

    USBR->EP0R=USBR->EP0R & (~USB_EP_CTR_RX) & USB_EPREG_MASK;

    // get the header from the setup packet

    UsbSetupHeader header(setup);
    switch(header.bmRequest & 0x1f) {

      case USB_REQ_RECIPIENT_DEVICE:
        handleDeviceRequest(header);
        break;

      case USB_REQ_RECIPIENT_INTERFACE:
        handleInterfaceRequest(header);
        break;

      case USB_REQ_RECIPIENT_ENDPOINT:
        handleEndpointRequest(header);
        break;

      default:
        setRxEndpointStatus(&USBR->EP0R,USB_EP_RX_STALL);
        break;
    }
  }


  /**
   * Handle a standard device request
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleDeviceRequest(const UsbSetupHeader& header) {

    switch(header.bRequest) {

      case USB_REQ_GET_DESCRIPTOR:
        handleGetDescriptor(header);
        break;

      case USB_REQ_SET_ADDRESS:
        handleSetAddress(header);
        break;

      case USB_REQ_SET_CONFIGURATION:
        handleSetConfig(header);
        break;

      case USB_REQ_GET_CONFIGURATION:
        handleGetConfig(header);
        break;

      case USB_REQ_GET_STATUS:
        handleGetStatus();
        break;

      case USB_REQ_SET_FEATURE:
        handleSetFeature(header);
        break;

      case USB_REQ_CLEAR_FEATURE:
        handleClearFeature(header);
        break;

      default:
        controlEndpointError();
        break;
    }
  }


  /**
   * Get a descriptor
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleGetDescriptor(const UsbSetupHeader& header) {

    uint8_t *dataToSend;
    uint16_t length;

    length=0;

    switch(header.wValue >> 8) {

      case USB_DESC_TYPE_DEVICE:
        dataToSend=const_cast<uint8_t *>(_deviceDescriptor);
        length=sizeof(_deviceDescriptor);
        break;

      case USB_DESC_TYPE_CONFIGURATION:
        dataToSend=const_cast<uint8_t *>(_configurationDescriptor);
        length=sizeof(_configurationDescriptor);
        break;

      case USB_DESC_TYPE_STRING:
        handleGetDescriptorString(header,dataToSend,length);
        break;

      default:
        controlEndpointError();
        return;
    }

    // send the data to the host

    if(length && header.wLength)
      sendControlData(dataToSend,length<=header.wLength ? length : header.wLength);
  }


  /**
   * Handle a request for a descriptor string
   * @param header The setup header received from the host
   * @param dataToSend out pointer to the data to send to the host
   * @param length out reference to the amount of bytes to send to the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleGetDescriptorString(const UsbSetupHeader& header,uint8_t*& dataToSend,uint16_t& length) {

    switch(header.wValue & 0xff) {

      case USBD_IDX_LANGID_STR:
        dataToSend=const_cast<uint8_t *>(_langIdDescriptor);
        length=sizeof(_langIdDescriptor);
        break;

      case USBD_IDX_MFC_STR:
        dataToSend=const_cast<uint8_t *>(TConfiguration::ManufacturerString);
        length=sizeof(TConfiguration::ManufacturerString);
        break;

      case USBD_IDX_PRODUCT_STR:
        dataToSend=const_cast<uint8_t *>(TConfiguration::ProductString);
        length=sizeof(TConfiguration::ProductString);
        break;

      case USBD_IDX_SERIAL_STR:
        dataToSend=const_cast<uint8_t *>(TConfiguration::SerialString);
        length=sizeof(TConfiguration::SerialString);
        break;

      case USBD_IDX_CONFIG_STR:
        dataToSend=const_cast<uint8_t *>(TConfiguration::ConfigurationString);
        length=sizeof(TConfiguration::ConfigurationString);
        break;

      case USBD_IDX_INTERFACE_STR:
        dataToSend=const_cast<uint8_t *>(TConfiguration::InterfaceString);
        length=sizeof(TConfiguration::InterfaceString);
        break;

      default:
        controlEndpointError();
        return;
    }
  }


  /**
   * Set the USB address
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleSetAddress(const UsbSetupHeader& header) {

    uint8_t address;

    if(header.wIndex==0 && header.wLength==0) {

      if(_deviceState==DeviceStateType::CONFIGURED)
        controlEndpointError();
      else {

        address=header.wValue & 0x7F;
        setAddress(address);
        sendControlZeroLengthPacket();

        setDeviceState(address ? DeviceStateType::ADDRESSED : DeviceStateType::DEFAULT);
      }
    }
    else
      controlEndpointError();
  }


  /**
   * Set a configuration
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleSetConfig(const UsbSetupHeader& header) {

    if((_configurationIndex=header.wValue)>1)    // just one configuration here
      controlEndpointError();
    else {

      switch(_deviceState) {
        case DeviceStateType::ADDRESSED:
          if(_configurationIndex) {
            openDeviceEndpoints();
            sendControlZeroLengthPacket();
            setDeviceState(DeviceStateType::CONFIGURED);
          }
          else
            sendControlZeroLengthPacket();
          break;

        case DeviceStateType::CONFIGURED:
          if(_configurationIndex==0) {
            closeDeviceEndpoints();
            sendControlZeroLengthPacket();
            setDeviceState(DeviceStateType::ADDRESSED);
          }
          else
            sendControlZeroLengthPacket();
          break;

        default:
          controlEndpointError();
          break;
      }
    }
  }


  /**
   * Get device configuration number request
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleGetConfig(const UsbSetupHeader& header) {

    if(header.wLength!=1)
      controlEndpointError();
    else {

      switch(_deviceState) {

        case DeviceStateType::ADDRESSED:
          _configurationIndex=0;
        case DeviceStateType::CONFIGURED:
          sendControlData(&_configurationIndex,1);
          break;

        default:
          controlEndpointError();
          break;
      }
    }
  }


  /**
   * Get the status
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleGetStatus() {

    static uint8_t status;

    if(_deviceState==DeviceStateType::ADDRESSED || _deviceState==DeviceStateType::CONFIGURED) {

      status=0;

      if(std::is_base_of<UsbSelfPoweredFeature,UsbCustomHid<TConfiguration,Features...>>::value)
        status|=USB_CONFIG_SELF_POWERED;

      if(_remoteWakeup)
        status|=USB_CONFIG_REMOTE_WAKEUP;

      sendControlData(&status,1);
    }
  }


  /**
   * Set a feature
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleSetFeature(const UsbSetupHeader& header) {

    if(header.wValue==USB_FEATURE_REMOTE_WAKEUP) {
      _remoteWakeup=true;
      sendControlZeroLengthPacket();    // ACK
    }
  }


  /**
   * Clear a feature
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleClearFeature(const UsbSetupHeader& header) {

    if(_deviceState==DeviceStateType::ADDRESSED || _deviceState==DeviceStateType::CONFIGURED) {
      if(header.wValue==USB_FEATURE_REMOTE_WAKEUP) {
        _remoteWakeup=false;
        sendControlZeroLengthPacket();    // ACK
      }
    }
    else
      controlEndpointError();
  }


  /**
   * Send some data to the host on the control IN endpoint
   * @param address of data to send
   * @param length number of bytes to send
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::sendControlData(const void *data,uint16_t length) {
    _controlEndpointState=ControlEndpointStateType::DATA_IN;
    sendData(0,CONTROL_IN_PMA_ADDRESS,CONTROL_MAX_PACKET_SIZE,data,length);
  }


  /**
   * Send a zero length control packet
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::sendControlZeroLengthPacket() {
    _controlEndpointState=ControlEndpointStateType::STATUS_IN;
    sendData(0,CONTROL_IN_PMA_ADDRESS,CONTROL_MAX_PACKET_SIZE,nullptr,0);
  }


  /**
   * Send some data to the host
   * @param endpointIndex the index of the endpoint
   * @param pmaInAddress The offset address in packet memory
   * @param maxPacketSize The maximum packet size
   * @param data Pointer to the data to send
   * @param length The number of bytes to send
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::sendData(
                               uint8_t endpointIndex,
                               uint16_t pmaInAddress,
                               uint16_t maxPacketSize,
                               const void *data,
                               uint16_t length) {


    // set up the IN endpoint structure for the transmission

    _inEndpointData[endpointIndex].total=length;
    _inEndpointData[endpointIndex].remaining=length;
    _inEndpointData[endpointIndex].ptr=reinterpret_cast<const uint8_t *>(data);
    _inEndpointData[endpointIndex].pmaAddress=pmaInAddress;
    _inEndpointData[endpointIndex].maxPacketSize=maxPacketSize;

    // continue from the beginning

    continueSendData(endpointIndex);
  }


  /**
   * Continue sending the next packet
   * @param endpointIndex the index of the endpoint
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::continueSendData(uint8_t endpointIndex) {

    uint32_t i,n;
    uint16_t *pdwVal,word;
    uint16_t length;
    const uint8_t *dataBytes;
    UsbInEndpointData& ep(_inEndpointData[endpointIndex]);

    // cut down the length if this will be a multi-packet transfer

    if((length=ep.remaining)>ep.maxPacketSize)
      length=ep.maxPacketSize;
    else
      length=ep.remaining;

    n=(length+1)/2;
    pdwVal=reinterpret_cast<uint16_t *>(BTABLE_BASE+ep.pmaAddress);
    dataBytes=ep.ptr;

    for(i=n;i!=0;i--) {
      word=dataBytes[0] | ((uint16_t)dataBytes[1] << 8);
      *pdwVal++=word;
      dataBytes+=2;
    }

    // update status

    ep.ptr+=length;
    ep.remaining-=length;

    // now that the PMA memory is prepared, set the length and tell the peripheral to send it

    USBR_BDT[endpointIndex].tx.count=length;
    setTxEndpointStatus(&USBR->EP0R+endpointIndex*2,USB_EP_TX_VALID);
  }


  /**
   * Handle a standard interface request
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleInterfaceRequest(const UsbSetupHeader& header) {

    if(_deviceState==DeviceStateType::CONFIGURED && (header.wIndex & 0xff)<=1)
      handleSetup(header);
    else
      controlEndpointError();
  }


  /**
   * Handle a setup request
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleSetup(const UsbSetupHeader& header) {

    switch(header.bmRequest & USB_REQ_TYPE_MASK) {

      case USB_REQ_TYPE_CLASS:
        handleClassSetup(header);
        break;

      case USB_REQ_TYPE_STANDARD:
        handleStandardSetup(header);
        break;
    }
  }


  /**
   * Class specific setup requests
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleClassSetup(const UsbSetupHeader& header) {

    switch(header.bRequest) {

      case CUSTOM_HID_REQ_SET_PROTOCOL:
        _protocol=header.wValue;
        sendControlZeroLengthPacket();
        break;

      case CUSTOM_HID_REQ_GET_PROTOCOL:
        sendControlData(&_protocol,1);
        break;

      case CUSTOM_HID_REQ_SET_IDLE:
        _idleState=header.wValue >> 8;
        sendControlZeroLengthPacket();
        break;

      case CUSTOM_HID_REQ_GET_IDLE:
        sendControlData(&_idleState,1);
        break;

      case CUSTOM_HID_REQ_SET_REPORT:
        _reportAvailable=true;            // incoming report
        _controlEndpointState=ControlEndpointStateType::DATA_OUT;
        USBR_BDT[0].rx.setRxCount(header.wLength);
        setRxEndpointStatus(&USBR->EP0R,USB_EP_RX_VALID);
        sendControlZeroLengthPacket();
        break;

      default:
        controlEndpointError();
        break;
    }
  }


  /**
   * Standard setup requests
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleStandardSetup(const UsbSetupHeader& header) {

    uint16_t len;
    uint8_t *pbuf;

    switch(header.bRequest) {

      case USB_REQ_GET_DESCRIPTOR:

        if(header.wValue >> 8==USB_DESC_TYPE_HID_REPORT) {
          pbuf=const_cast<uint8_t *>(_reportDescriptor);
          len=sizeof(_reportDescriptor);
        }
        else if(header.wValue >> 8==USB_DESC_TYPE_HID_DESCRIPTOR) {
          pbuf=const_cast<uint8_t *>(_hidDescriptor);
          len=sizeof(_hidDescriptor);
        }
        else {
          pbuf=nullptr;
          len=0;
        }

        sendControlData(pbuf,header.wLength<len ? header.wLength : len);
        break;

      case USB_REQ_SET_INTERFACE:
        _altSetting=header.wValue;
        sendControlZeroLengthPacket();
        break;

      case USB_REQ_GET_INTERFACE:
        sendControlData(&_altSetting,1);
        break;

      default:
        controlEndpointError();
        break;
    }
  }


  /**
   * Handle a standard endpoint request
   * @param header The setup header received from the host
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleEndpointRequest(const UsbSetupHeader& header) {

    uint8_t ep_addr;
    static uint16_t status;

    if((header.bmRequest & 0x60)==0x20)
      handleSetup(header);
    else {

      // get the endpoint address

      ep_addr=header.wIndex;

      switch(header.bRequest) {

        case USB_REQ_SET_FEATURE :

          switch(_deviceState) {

            case DeviceStateType::ADDRESSED:
              if((ep_addr & 0x7f)!=0)
                stallEndpoint(ep_addr);
              break;

            case DeviceStateType::CONFIGURED:
              if(header.wValue==USB_FEATURE_EP_HALT && (ep_addr & 0x7f)!=0)
                stallEndpoint(ep_addr);

              handleSetup(header);
              sendControlZeroLengthPacket();
              break;

            default:
              controlEndpointError();
              break;
          }
          break;

        case USB_REQ_CLEAR_FEATURE:

          switch (_deviceState) {

            case DeviceStateType::ADDRESSED:
              if((ep_addr & 0x7f)!=0)
                stallEndpoint(ep_addr);
              break;

            case DeviceStateType::CONFIGURED:
              if(header.wValue==USB_FEATURE_EP_HALT) {
                if((ep_addr & 0x7f)!=0) {
                  unstallEndpoint(ep_addr);
                  handleSetup(header);
                }
              }
              break;

            default:
              controlEndpointError();
              break;
          }
          break;

        case USB_REQ_GET_STATUS:

          switch (_deviceState) {

            case DeviceStateType::ADDRESSED:
              if((ep_addr & 0x7f)!=0)
                stallEndpoint(ep_addr);
              break;

            case DeviceStateType::CONFIGURED:
              status=isStalled(ep_addr) ? 1 : 0;
              sendControlData(&status,2);
              break;

            default:
              controlEndpointError();
              break;
          }
          break;
      }
    }
  }


  /**
   * Stall an endpoint
   * @param epAddr The address of the endpoint
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::stallEndpoint(uint8_t epAddr) const {

    uint8_t endpointIndex;

    endpointIndex=epAddr & 0x7f;

    if(endpointIndex==0)
      controlEndpointError();
    else {

      if((epAddr & 0x80)==0x80)
        setTxEndpointStatus(&USBR->EP0R+endpointIndex*2,USB_EP_TX_STALL);
      else
        setRxEndpointStatus(&USBR->EP0R+endpointIndex*2,USB_EP_RX_STALL);
    }
  }


  /**
   * Unstall an endpoint
   * @param epAddr The address of the endpoint
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::unstallEndpoint(uint8_t epAddr) const {

    uint8_t endpointIndex;

    endpointIndex=epAddr & 0x7f;

    if((epAddr & 0x80)==0x80 || endpointIndex==0)
      setTxEndpointStatus(&USBR->EP0R+endpointIndex*2,USB_EP_TX_STALL);
    else if((epAddr & 0x80)==0 || endpointIndex==0)
      setRxEndpointStatus(&USBR->EP0R+endpointIndex*2,USB_EP_RX_STALL);
  }


  /**
   * Return true if an endpoint is stalled
   * @param epAddr The address of the endpoint
   */

  template<class TConfiguration,class... Features>
  inline bool UsbCustomHid<TConfiguration,Features...>::isStalled(uint8_t epAddr) const {

    uint8_t endpointIndex;
    uint16_t status;

    endpointIndex=epAddr & 0x7f;
    if((epAddr & 0x80)==0x80 || endpointIndex==0) {
      status=getTxEndpointStatus(&USBR->EP0R+endpointIndex*2);
      return status==USB_EP_TX_STALL;
    }
    else {
      status=getRxEndpointStatus(&USBR->EP0R+endpointIndex*2);
      return status==USB_EP_RX_STALL;
    }
  }


  /**
   * Handle a control data OUT transfer
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleControlDataOutTransfer() {

    uint16_t count;

    // clear the 'correct transfer for reception' bit for this endpoint

    USBR->EP0R=USBR->EP0R & ~(USB_EP_CTR_RX) & USB_EPREG_MASK;

    if((count=USBR_BDT[0].rx.getCount())>0) {

      // data out from the host on the control endpoint

      UsbRxEventSender.raiseEvent(
        0,
        reinterpret_cast<uint16_t *>(BTABLE_BASE+CONTROL_OUT_PMA_ADDRESS),
        count);
    }

    USBR_BDT[0].rx.setRxCount(CONTROL_MAX_PACKET_SIZE);
    setRxEndpointStatus(&USBR->EP0R,USB_EP_RX_VALID);
  }


  /**
   * Copy data received from packet memory to user memory
   * @param dest the destination in user memory
   * @param src the source address in packet memory
   * @param byteCount the number of bytes to copy
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::copyReceivedData(void *dest,uint32_t src,uint16_t byteCount) const {

    uint32_t i;
    uint16_t *destptr,*srcptr;

    destptr=reinterpret_cast<uint16_t *>(dest);
    srcptr=reinterpret_cast<uint16_t *>(src);

    for(i=(byteCount+1)/2;i>0;i--)
      *destptr++=*srcptr++;
  }


  /**
   * Handle the control IN transfer
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleControlInTransfer() {

    // clear CTR_TX

    USBR->EP0R=USBR->EP0R & (~USB_EP_CTR_TX) & USB_EPREG_MASK;

    if(_controlEndpointState==ControlEndpointStateType::DATA_IN) {

      if(_inEndpointData[0].remaining) {

        // continue sending the next in the multi-packet transfer

        continueSendData(0);

        // prepare for premature end of transfer

        USBR_BDT[0].rx.setRxCount(0);
        setRxEndpointStatus(&USBR->EP0R,USB_EP_RX_VALID);
      }
      else {

        // if we're sending a multiple of max packet size then a zero length is required

        if((_inEndpointData[0].total % CONTROL_MAX_PACKET_SIZE)==0 &&
            _inEndpointData[0].total>CONTROL_MAX_PACKET_SIZE &&
            _inEndpointData[0].total<_setupDataLength) {

          // send zero length packet

          sendData(0,CONTROL_IN_PMA_ADDRESS,CONTROL_MAX_PACKET_SIZE,nullptr,0);

          // prepare for premature end of transfer

          USBR_BDT[0].rx.setRxCount(0);
          setRxEndpointStatus(&USBR->EP0R,USB_EP_RX_VALID);
        }
        else {
          _controlEndpointState=ControlEndpointStateType::STATUS_OUT;
          USBR_BDT[0].rx.setRxCount(0);
          setRxEndpointStatus(&USBR->EP0R,USB_EP_RX_VALID);
        }
      }
    }

    if(_address>0 && _inEndpointData[0].remaining==0) {
      USBR->DADDR=_address | USB_DADDR_EF;
      _address=0;
    }
  }


  /**
   * Interrupt handler
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::onInterrupt() {

    // correct endpoint transfer

    if((USBR->ISTR & USB_ISTR_CTR)!=0)
      onCorrectTransfer();

    // reset

    if((USBR->ISTR & USB_ISTR_RESET)!=0)
      onReset();

    // packet memory over/underflow

    if((USBR->ISTR & USB_ISTR_PMAOVR)!=0)
      onPacketMemoryOverUnderFlow();

    // generic error

    if((USBR->ISTR & USB_ISTR_ERR)!=0)
      onError();

    // wakeup

    if((USBR->ISTR & USB_ISTR_WKUP)!=0)
      onWakeup();

    // suspend

    if((USBR->ISTR & USB_ISTR_SUSP)!=0)
      onSuspend();

    // start of frame

    if((USBR->ISTR & USB_ISTR_SOF)!=0)
      onStartOfFrame();

    // expected start of frame

    if((USBR->ISTR & USB_ISTR_ESOF)!=0)
      onExpectedStartOfFrame();
  }


  /**
   * Correct transfer interrupt
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::onCorrectTransfer() {

    uint16_t irq;
    uint8_t endpointIndex;
    bool isOut;
    volatile uint16_t *reg;

    // USB_ISTR_CTR is read only and will be automatically cleared by hardware
    // when we've processed all endpoint results

    while(((irq=USBR->ISTR) & USB_ISTR_CTR)!=0) {

      endpointIndex=irq & USB_ISTR_EP_ID;
      isOut=(irq & USB_ISTR_DIR)!=0;

      if(endpointIndex==0) {

        // control endpoint events

        if(isOut)
          handleControlOutTransfer();
        else
          handleControlInTransfer();
      }
      else {

        // normal endpoint events

        reg=&USBR->EP0R+2*endpointIndex;

        if((*reg & USB_EP_CTR_RX)!=0)
          handleEndpointOutTransfer(reg,endpointIndex);

        if((*reg & USB_EP_CTR_TX)!=0)
          handleEndpointInTransfer(reg,endpointIndex);
      }
    }
  }


  /**
   * Handle endpoint TX complete
   * @param reg The endpoint register
   * @param endpointIndex The endpoint index
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleEndpointInTransfer(volatile uint16_t *reg,uint8_t endpointIndex) {

    // clear CTR_TX

    *reg=*reg & (~USB_EP_CTR_TX) & USB_EPREG_MASK;

    // if this endpoint is complete then notify the user

    if(_inEndpointData[endpointIndex].remaining==0)
      UsbTxCompleteEventSender.raiseEvent(endpointIndex,_inEndpointData[endpointIndex].total);
    else
      continueSendData(endpointIndex);      // send the next batch
  }


  /**
   * Handle endpoint RX complete
   * @param reg The endpoint register
   * @param endpointIndex The endpoint index
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::handleEndpointOutTransfer(volatile uint16_t *reg,uint8_t endpointIndex) {

    // clear CTR_RX

    *reg=*reg & (~USB_EP_CTR_RX) & USB_EPREG_MASK;

    // notify the user that some data has been received

    UsbRxEventSender.raiseEvent(
        endpointIndex,
        reinterpret_cast<uint16_t *>(BTABLE_BASE+OUT_ENDPOINT_PMA_ADDRESS),
        static_cast<uint16_t>(USBR_BDT[endpointIndex].rx.getCount()));

    // reset for the next inbound

    USBR_BDT[endpointIndex].rx.setRxCount(TConfiguration::OUT_ENDPOINT_MAX_PACKET_SIZE);

    // endpoint is valid for the next time

    setRxEndpointStatus(reg,USB_EP_RX_VALID);
  }


  /**
   * Reset interrupt
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::onReset() {

    // clear the interrupt flag

    USBR->ISTR &= ~USB_ISTR_RESET;

    // reset our address

    setAddress(0);

    // open the control endpoints

    openControlEndpoints();

    // notify

    UsbStatusEventSender.raiseEvent(UsbStatusType::RESET);
  }


  /**
   * Packet memory overflow/underrun interrupt
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::onPacketMemoryOverUnderFlow() const {

    USBR->ISTR &= ~USB_ISTR_PMAOVR;

    // notify

    UsbStatusEventSender.raiseEvent(UsbStatusType::PACKET_MEMORY_OVER_UNDERFLOW);
  }


  /**
   * Error interrupt
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::onError() const {

    USBR->ISTR &= ~USB_ISTR_ERR;

    // notify

    UsbStatusEventSender.raiseEvent(UsbStatusType::PACKET_MEMORY_OVER_UNDERFLOW);
  }


  /**
   * Wakeup interrupt
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::onWakeup() {

    // come out of low power mode

    USBR->CNTR &= ~USB_CNTR_LPMODE;

    // set the interrupt mask

    setInterruptMask();

    // clear interrupt flag

    USBR->ISTR &= ~USB_ISTR_WKUP;

    // notify

    setDeviceState(_savedState);
  }


  /**
   * Suspend interrupt
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::onSuspend() {

    // clear interrupt flag

    USBR->ISTR &= ~USB_ISTR_SUSP;

    // suspend and low power mode

    USBR->CNTR |= USB_CNTR_FSUSP;
    USBR->CNTR |= USB_CNTR_LPMODE;

    _savedState=_deviceState;
    setDeviceState(DeviceStateType::SUSPENDED);
  }


  /**
   * Start of frame interrupt
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::onStartOfFrame() const {

    USBR->ISTR &= ~USB_ISTR_SOF;

    // notify

    UsbStatusEventSender.raiseEvent(UsbStatusType::START_OF_FRAME);
  }


  /**
   * Expected start of frame interrupt
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::onExpectedStartOfFrame() const {

    USBR->ISTR &= ~USB_ISTR_ESOF;

    // notify

    UsbStatusEventSender.raiseEvent(UsbStatusType::UNEXPECTED_START_OF_FRAME);
  }


  /*
   * Set a new device state and notify the change
   */

  template<class TConfiguration,class... Features>
  inline void UsbCustomHid<TConfiguration,Features...>::setDeviceState(DeviceStateType newState) {

    UsbStatusType notify;

    if(_deviceState!=newState) {

      _deviceState=newState;

      switch(newState) {

        case DeviceStateType::DEFAULT:
          notify=UsbStatusType::STATE_DEFAULT;
          break;

        case DeviceStateType::ADDRESSED:
          notify=UsbStatusType::STATE_ADDRESSED;
          break;

        case DeviceStateType::CONFIGURED:
          notify=UsbStatusType::STATE_CONFIGURED;
          break;

        case DeviceStateType::SUSPENDED:
          notify=UsbStatusType::STATE_SUSPENDED;
          break;

        default:
          return;
      }

      UsbStatusEventSender.raiseEvent(notify);
    }
  }


  /**
   * Return the current device state
   * @return The device state
   */

  template<class TConfiguration,class... Features>
  inline typename UsbCustomHid<TConfiguration,Features...>::DeviceStateType UsbCustomHid<TConfiguration,Features...>::getDeviceState() const {
    return _deviceState;
  }
}
