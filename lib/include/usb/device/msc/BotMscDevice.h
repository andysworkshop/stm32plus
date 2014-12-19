/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Endpoints used by this device. The Bulk Only Transport has one bulk IN and one OUT
     * endpoint.
     */

    template<class TDevice> using BotMscDeviceInEndpoint=BulkInEndpointFeature<1,TDevice>;
    template<class TDevice> using BotMscDeviceOutEndpoint=BulkOutEndpointFeature<2,TDevice>;


    /**
     * Declare the structure that gets sent back when the host asks for the whole
     * configuration descriptor
     */

    struct BotMscDeviceConfigurationDescriptor {
      ConfigurationDescriptor configuration;
      InterfaceDescriptor itf;
      EndpointDescriptor outEndpoint;
      EndpointDescriptor inEndpoint;
    } __attribute__((packed));


    /**
     * Derivation of MscDevice to provide support for the Bulk Only Transport (BOT)
     *
     *   1x Configuration descriptor
     *   2x Interfaces
     *   1x Inbound bulk data endpoint
     *   1x Outbound bulk data endpoint
     */

     template<class TPhy,template <class> class... Features>
     class BotMscDevice : public MscDevice<TPhy,
                                           BotMscDeviceConfigurationDescriptor,
                                           BotMscDeviceInEndpoint,
                                           BotMscDeviceOutEndpoint,
                                           Features...> {

       public:

         typedef MscDevice<TPhy,
                           BotMscDeviceConfigurationDescriptor,
                           BotMscDeviceInEndpoint,
                           BotMscDeviceOutEndpoint,
                           Features...> MscDeviceBase;

         /**
          * Customisable parameters for this MSC BOT device
          */

         struct Parameters : MscDeviceBase::Parameters {

           uint16_t msc_bot_max_packet_size;       // default is 64 bytes

           Parameters() {
             msc_bot_max_packet_size=0x40;
           }
         };

       protected:

         enum {
           IN_EP_ADDRESS = EndpointDescriptor::IN | 1,     // endpoint address
           OUT_EP_ADDRESS = EndpointDescriptor::OUT | 2,   // out endpoint address
         };

      protected:
        void onEvent(UsbEventDescriptor& event);

      public:
        BotMscDevice();
        ~BotMscDevice();

        bool initialise(Parameters& params);
    };


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline BotMscDevice<TPhy,Features...>::BotMscDevice() {

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&BotMscDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Destructor
     */

    template<class TPhy,template <class> class... Features>
    inline BotMscDevice<TPhy,Features...>::~BotMscDevice() {

      // unsubscribe from USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&BotMscDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param param The parameters structure
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool BotMscDevice<TPhy,Features...>::initialise(Parameters& params) {

      USBD_StatusTypeDef status;

      // initialise upwards

      if(!MscDeviceBase::initialise(params))
        return false;

      // set up the configuration descriptor (see constructor for defaults)

      this->_configurationDescriptor.configuration.wTotalLength=sizeof(this->_configurationDescriptor);
      this->_configurationDescriptor.configuration.bNumInterfaces=1;
      this->_configurationDescriptor.configuration.bmAttributes=0x80 | ConfigurationDescriptor::SELF_POWERED;

      // if ConfigurationTextFeature is in the hierarchy then we've got a configuration string (compile-time check)

      if(std::is_base_of<ConfigurationTextFeature<Device<TPhy>>,BotMscDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.configuration.iConfiguration=USBD_IDX_CONFIG_STR;

      // set up the interface descriptor

      this->_configurationDescriptor.itf.bInterfaceNumber=0;
      this->_configurationDescriptor.itf.bNumEndpoints=2;
      this->_configurationDescriptor.itf.bInterfaceClass=static_cast<uint8_t>(DeviceClass::MASS_STORAGE);
      this->_configurationDescriptor.itf.bInterfaceSubClass=static_cast<uint8_t>(MscSubClass::SCSI_TRANSPARENT);
      this->_configurationDescriptor.itf.bInterfaceProtocol=static_cast<uint8_t>(MscProtocol::BOT);

      // if InterfaceTextFeature is in the hierarchy then we've got an interface string (compile-time check)

      if(std::is_base_of<InterfaceTextFeature<Device<TPhy>>,BotMscDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.itf.iInterface=USBD_IDX_INTERFACE_STR;

      // set up the OUT endpoint descriptor

      this->_configurationDescriptor.outEndpoint.bEndpointAddress=OUT_EP_ADDRESS;
      this->_configurationDescriptor.outEndpoint.bmAttributes=EndpointDescriptor::BULK;
      this->_configurationDescriptor.outEndpoint.wMaxPacketSize=params.msc_bot_max_packet_size;         // LED reports are 1 byte

      // set up the IN endpoint descriptor

      this->_configurationDescriptor.inEndpoint.bEndpointAddress=IN_EP_ADDRESS;
      this->_configurationDescriptor.inEndpoint.bmAttributes=EndpointDescriptor::BULK;
      this->_configurationDescriptor.inEndpoint.wMaxPacketSize=params.msc_bot_max_packet_size;

      // set up the qualifier descriptor (see constructor for defaults)

      this->_qualifierDescriptor.bMaxPacketSize0=0x40;
      this->_qualifierDescriptor.bNumConfigurations=1;

      // start the device

      if((status=USBD_Start(&this->_deviceHandle))!=USBD_OK)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,this->E_START,status);

      // OK

      return true;
    }


    /**
     * Event handler for device events
     * @param event The event descriptor
     */

    template<class TPhy,template <class> class... Features>
    __attribute__((noinline)) inline void BotMscDevice<TPhy,Features...>::onEvent(UsbEventDescriptor& event) {
    }
  }
}
