/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

using namespace stm32plus;


template<class TPinPackage>
class SpiEmulator {

  protected:
    GpioPinRef _nss;
    GpioPinRef _sclk;
    GpioPinRef _mosi;
    GpioPinRef _miso;

  public:
    SpiEmulator();

    void setNss(bool state) {
      _nss.setState(state);
    }

    bool send(const uint8_t *buffer,uint32_t size) {
      for(uint32_t i=0;i<size;i++) {

        uint8_t b=buffer[i];
        for(uint8_t j=0;j<8;j++) {

          if((b & 0x80)==0)
            _mosi.reset();
          else
            _mosi.set();

          _sclk.reset();
          _sclk.set();

          b<<=1;
        }
      }
      return true;
    }

    bool receive(uint8_t *buffer,uint32_t size) {

      for(uint32_t i=0;i<size;i++) {

        uint8_t b=0;

        for(uint8_t j=0;j<8;j++) {

          b<<=1;

          _sclk.reset();
          _sclk.set();

          if(_miso.read())
            b|=1;
        }

        buffer[i]=b;
      }
      return true;
    }
};


template<class TPinPackage>
inline SpiEmulator<TPinPackage>::SpiEmulator()
  : _nss((GPIO_TypeDef *)TPinPackage::Port_NSS,TPinPackage::Pin_NSS),
    _sclk((GPIO_TypeDef *)TPinPackage::Port_SCK,TPinPackage::Pin_SCK),
    _mosi((GPIO_TypeDef *)TPinPackage::Port_MOSI,TPinPackage::Pin_MOSI),
    _miso((GPIO_TypeDef *)TPinPackage::Port_MISO,TPinPackage::Pin_MISO) {

  // initialise NSS

  GpioPinInitialiser::initialise(
      (GPIO_TypeDef *)TPinPackage::Port_NSS,
      TPinPackage::Pin_NSS,
      Gpio::OUTPUT);

  // initialise SCLK

  GpioPinInitialiser::initialise(
      (GPIO_TypeDef *)TPinPackage::Port_SCK,
      TPinPackage::Pin_SCK,
      Gpio::OUTPUT);

  // initialise MOSI

  GpioPinInitialiser::initialise(
      (GPIO_TypeDef *)TPinPackage::Port_MOSI,
      TPinPackage::Pin_MOSI,
      Gpio::OUTPUT);

  // initialise MISO

   GpioPinInitialiser::initialise(
       (GPIO_TypeDef *)TPinPackage::Port_MISO,
       TPinPackage::Pin_MISO,
       Gpio::INPUT);

   // default states

   _nss.set();
   _sclk.set();
}
