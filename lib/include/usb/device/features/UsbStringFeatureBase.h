/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Intermediate base class to hold the string used for a USB feature. Converts the
   * supplied string into unicode ready for use by the protocol
   */

  template<class TUsbDevice>
  class UsbStringFeatureBase : public UsbDeviceFeatureBase<TUsbDevice> {

    protected:
      uint16_t _length;
      scoped_array<uint16_t> _unicodeString;
      uint8_t _stringIndex;

    protected:
      UsbStringFeatureBase(TUsbDevice& device,const char *str,uint8_t stringIndex);
      ~UsbStringFeatureBase();

      void onEvent(UsbDeviceEventDescriptor& event);
  };


  /**
   * Constructor. This will do a dumb conversion to unicode by assuming that the mapping
   * of 8-bit characters is represented by an equivalent [cc00] unicode character.
   *
   * @param device A reference to the USB device class
   * @param str 8-bit ASCII version of the string. Does not need to remain in
   * scope after this constructor completes.
   * @param stringIndex Index used by the protocol when it calls back for this string
   */

  template<class TUsbDevice>
  inline UsbStringFeatureBase<TUsbDevice>::UsbStringFeatureBase(TUsbDevice& device,const char *str,uint8_t stringIndex)
    : UsbDeviceFeatureBase<TUsbDevice>(device),
      _length(strlen(str)+1),
      _unicodeString(new uint16_t[_length]),
      _stringIndex(stringIndex) {

    const char *src;
    uint16_t *dest;

    // convert the string to unicode

    src=str;
    dest=&_unicodeString[0];

    do {
      *dest++=*src;
    } while(*src++);

    // register this string index with the device descriptor

    device.getDeviceDescriptor().iManufacturer=stringIndex;

    // subscribe to device events

    device.UsbDeviceEventSender.insertSubscriber(
        UsbDeviceEventSourceSlot::bind(this,&UsbStringFeatureBase<TUsbDevice>::onEvent)
      );
  }


  /**
   * Destructor
   */

  template<class TUsbDevice>
  inline UsbStringFeatureBase<TUsbDevice>::~UsbStringFeatureBase() {

    // unsubscribe from device events

    this->_device.UsbDeviceEventSender.removeSubscriber(
        UsbDeviceEventSourceSlot::bind(this,&UsbStringFeatureBase<TUsbDevice>::onEvent)
      );
  }


  /**
   * Event callback
   * @param event The event object
   */

  template<class TUsbDevice>
  inline void UsbStringFeatureBase<TUsbDevice>::onEvent(UsbDeviceEventDescriptor& event) {

    if(event.eventType==UsbDeviceEventDescriptor::EventType::GET_STRING_DESCRIPTOR) {

      // convert to the event type that we know it is

      UsbDeviceGetDisplayStringDescriptorEvent & stringEvent(
          static_cast<UsbDeviceGetDisplayStringDescriptorEvent&>(event));

      // if we handle this string index then set the return values as long
      // as someone hasn't got there first

      if(stringEvent.stringIndex==_stringIndex && stringEvent.string==nullptr) {

        stringEvent.string=_unicodeString.get();
        stringEvent.length=_length*2;   // length in bytes incl. \0 terminator
      }
    }
  }
}
