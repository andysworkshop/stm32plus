/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Endpoint used by this device
     */

    template<class TDevice> using MouseHidDeviceEndpoint1=InterruptInEndpointFeature<1,TDevice>;

    /**
     * Derivation of HidDevice to handle a HID mouse. This device type declares the following:
     *   1x Configuration descriptor
     *   1x Interface
     *   1x Inbound interrupt endpoint
     */

    template<class TPhy,template <class> class... Features>
    class MouseHidDevice : public HidDevice<TPhy,MouseHidDeviceEndpoint1,Features...> {

      protected:

        /**
         * Declare the structure that gets sent back when the host asks for the whole
         * configuration descriptor
         */

        struct DeviceDescriptor {
          ConfigurationDescriptor configuration;
          InterfaceDescriptor interface;
          HidClassDescriptor hid;
          EndpointDescriptor endpoint;
        } __attribute__((packed));

        DeviceDescriptor _mouseDescriptor;

      public:

        /**
         * Customisable parameters for this HID device
         */

        struct Parameters : HidDevice<TPhy,MouseHidDeviceEndpoint1,Features...>::Parameters {

          uint8_t hid_mouse_poll_interval;      // default is 10

          Parameters() {
            hid_mouse_poll_interval=HID_POLLING_INTERVAL;
          }
        };

      public:
        bool initialise(Parameters& params);
    };


    /**
     * Initialise the class
     * @param param The parameters structure
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    bool MouseHidDevice<TPhy,Features...>::initialise(Parameters& params) {

      // set up the configuration descriptor (see constructor for defaults)

      _mouseDescriptor.configuration.wTotalLength=sizeof(DeviceDescriptor);
      _mouseDescriptor.configuration.bNumInterfaces=1;

      // set up the interface descriptor (see constructor for defaults)

      _mouseDescriptor.interface.bInterfaceNumber=0;
      _mouseDescriptor.interface.bNumEndpoints=1;
      _mouseDescriptor.interface.bInterfaceClass=static_cast<uint8_t>(DeviceClass::HID);
      _mouseDescriptor.interface.bInterfaceSubClass=static_cast<uint8_t>(HidSubClass::BOOT);
      _mouseDescriptor.interface.bInterfaceProtocol=static_cast<uint8_t>(HidProtocol::MOUSE);

      // set up the hid class descriptor (see constructor for defaults)

      _mouseDescriptor.hid.bNumDescriptors=1;
      _mouseDescriptor.hid.bDescriptorType=HID_REPORT_DESC;
      _mouseDescriptor.hid.wItemLength=sizeof(MouseReportDescriptor);

      // set up the endpoint descriptor

      _mouseDescriptor.endpoint.bEndpointAddress=EndpointDescriptor::IN | 1;
      _mouseDescriptor.endpoint.bmAttributes=EndpointDescriptor::INTERRUPT;
      _mouseDescriptor.endpoint.wMaxPacketSize=4;                           // mouse reports are 4 bytes
      _mouseDescriptor.endpoint.bInterval=params.hid_mouse_poll_interval;   // default is 10 frames

      return HidDevice<TPhy,MouseHidDeviceEndpoint1,Features...>::initialise(params);
    }
  }
}
