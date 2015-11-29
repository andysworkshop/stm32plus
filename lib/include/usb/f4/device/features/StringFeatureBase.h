/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Intermediate base class to hold the string used for a USB feature. Converts the
     * supplied string into unicode ready for use by the protocol
     */

    template<class TDevice>
    class StringFeatureBase : public DeviceFeatureBase<TDevice> {

      protected:
        uint16_t _length;
        scoped_array<uint8_t> _descriptor;
        uint8_t _stringIndex;

      protected:
        StringFeatureBase(TDevice& device,uint8_t stringIndex);
        ~StringFeatureBase();

        bool initialise(const char *str);

        void onEvent(UsbEventDescriptor& event);
    };


    /**
     * Constructor
     * @param device A reference to the USB device class
     * @param stringIndex Index used by the protocol when it calls back for this string
     */

    template<class TDevice>
    inline StringFeatureBase<TDevice>::StringFeatureBase(TDevice& device,uint8_t stringIndex)
      : DeviceFeatureBase<TDevice>(device) {

      _stringIndex=stringIndex;

      // subscribe to device events

      device.UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&StringFeatureBase<TDevice>::onEvent)
        );
    }


    /**
     * Initialise. This will do a dumb conversion to unicode by assuming that the mapping
     * of 8-bit characters is represented by an equivalent [cc00] unicode character.
     *
     * @param str 8-bit ASCII version of the string. Does not need to remain in
     * scope after this constructor completes.
     */

    template<class TDevice>
    inline bool StringFeatureBase<TDevice>::initialise(const char *str) {

      uint16_t len;

      if(str==nullptr)
        _length=0;
      else {
        _length=strlen(str)*2+2;    // +1 for 16-bit descriptor header, +1 for \0 at end
        _descriptor.reset(new uint8_t[_length]);

        USBD_GetString((uint8_t *)str,&_descriptor[0],&len);
      }
      return true;
    }


    /**
     * Destructor
     */

    template<class TDevice>
    inline StringFeatureBase<TDevice>::~StringFeatureBase() {

      // unsubscribe from device events

      this->_device.UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&StringFeatureBase<TDevice>::onEvent)
        );
    }


    /**
     * Event callback
     * @param event The event object
     */

    template<class TDevice>
    inline void StringFeatureBase<TDevice>::onEvent(UsbEventDescriptor& event) {

      if(event.eventType==UsbEventDescriptor::EventType::DEVICE_GET_STRING_DESCRIPTOR) {

        // convert to the event type that we know it is

        DeviceGetDisplayStringDescriptorEvent & stringEvent(
            static_cast<DeviceGetDisplayStringDescriptorEvent&>(event));

        // if we handle this string index then set the return values as long
        // as someone hasn't got there first

        if(stringEvent.stringIndex==_stringIndex && stringEvent.descriptor==nullptr) {

          stringEvent.descriptor=_descriptor.get();
          stringEvent.length=_length;   // length in bytes incl. \0 terminator
        }
      }
    }
  }
}
