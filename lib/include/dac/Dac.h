/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Base class for both DAC peripherals
   */

  class Dac {

    protected:
      uint32_t _channel;

    public:
      Dac(uint32_t channel);

    public:
      uint32_t getChannel() const;

      void enablePeripheral() const;
      void disablePeripheral() const;

      void triggerOn() const;
      void triggerOff() const;

      uint16_t getLastOutput() const;
  };


  /**
   * Constructor
   * @param channel DAC channel (DAC_Channel_1 / DAC_Channel_2)
   */

  inline Dac::Dac(uint32_t channel)
    : _channel(channel) {
  }


  /**
   * Get the channel number
   * @return The channel number (DAC_Channel_1 / DAC_Channel_2)
   */

  inline uint32_t Dac::getChannel() const {
    return _channel;
  }


  /**
   * Enable the peripheral
   */

  inline void Dac::enablePeripheral() const {
    DAC_Cmd(_channel,ENABLE);
  }


  /**
   * Disable the peripheral
   */

  inline void Dac::disablePeripheral() const {
    DAC_Cmd(_channel,DISABLE);
  }


  /**
   * Fire the software trigger
   */

  inline void Dac::triggerOn() const {
    DAC_SoftwareTriggerCmd(_channel,ENABLE);
  }


  /**
   * Disable the software trigger
   */

  inline void Dac::triggerOff() const {
    DAC_SoftwareTriggerCmd(_channel,DISABLE);
  }


  /**
   * Get the last data output to the channel
   * @return
   */

  inline uint16_t Dac::getLastOutput() const {
    return DAC_GetDataOutputValue(_channel);
  }
}
