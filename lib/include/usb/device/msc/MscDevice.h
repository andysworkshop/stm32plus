/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Intermediate base class for mass storage device implementations. Provides common
     * functionality for all mass storage device types.
     *
     * @tparam TPhy the PHY implementation
     * @tparam TConfigurationDescriptor A structure that holds the complete config descriptor
     * @tparam Features... The device feature classes
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    class MscDevice : public Device<TPhy>,
                      public ControlEndpointFeature<Device<TPhy>>,
                      public Features<Device<TPhy>>... {


      public:

        /*
         * Parameters for the MSC device
         */

        struct Parameters : Device<TPhy>::Parameters,
                            ControlEndpointFeature<Device<TPhy>>::Parameters,
                            Features<Device<TPhy>>::Parameters... {

          Parameters() {
          }
        };

      protected:
        TConfigurationDescriptor  _configurationDescriptor;

      protected:
        void onEvent(UsbEventDescriptor& event);
        void onGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event);

      public:
        MscDevice();
        ~MscDevice();

        bool initialise(Parameters& params);
    };


    /**
     * Constructor
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline MscDevice<TPhy,TConfigurationDescriptor,Features...>::MscDevice()
      : ControlEndpointFeature<Device<TPhy>>(static_cast<Device<TPhy>&>(*this)),
        Features<Device<TPhy>>(static_cast<Device<TPhy>&>(*this))... {

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&MscDevice<TPhy,TConfigurationDescriptor,Features...>::onEvent)
        );
    }


    /**
     * Destructor
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline MscDevice<TPhy,TConfigurationDescriptor,Features...>::~MscDevice() {

      // unsubscribe from USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&MscDevice<TPhy,TConfigurationDescriptor,Features...>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param params The parameters class
     * @return true if it worked
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline bool MscDevice<TPhy,TConfigurationDescriptor,Features...>::initialise(Parameters& params) {

      // initialise upwards

      if(!Device<TPhy>::initialise(params) ||
         !ControlEndpointFeature<Device<TPhy>>::initialise(params) ||
         !RecursiveBoolInitWithParams<MscDevice,Features<Device<TPhy>>...>::tinit(this,params))
        return false;

      // link UsbEventSource class into the SDK structure

      USBD_RegisterClass(&this->_deviceHandle,static_cast<UsbEventSource *>(this));
      return true;
    }


    /**
     * Event handler for device events
     * @param event The event descriptor
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    __attribute__((noinline)) inline void MscDevice<TPhy,TConfigurationDescriptor,Features...>::onEvent(UsbEventDescriptor& event) {

      // check for handled events

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::CLASS_GET_CONFIGURATION_DESCRIPTOR:
          onGetConfigurationDescriptor(static_cast<DeviceClassSdkGetConfigurationDescriptorEvent&>(event));
          break;

        default:
          break;
      }
    }


    /**
     * Get the configuration descriptor
     * @param event The event class to receive the descriptor and provide type of descriptor being requested
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline void MscDevice<TPhy,TConfigurationDescriptor,Features...>::onGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event) {

      if(event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::HIGH_SPEED ||
         event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::FULL_SPEED ||
         event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::OTHER_SPEED) {

        // set up the values in the event

        event.length=sizeof(_configurationDescriptor);
        event.descriptor=reinterpret_cast<uint8_t *>(&_configurationDescriptor);
      }
    }
  }
}
