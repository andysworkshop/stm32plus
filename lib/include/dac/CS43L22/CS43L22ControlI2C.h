/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Template class for the CS43L22 I2C controller. Inherits all the I2C functionality
   * from the peripheral it's parameterised with
   */

  template<class TI2CPeripheral>
  class CS43L22ControlI2C : public TI2CPeripheral {

    public:

      enum {
        I2C_BUS_ADDRESS = 0x94
      };

      enum Command {
        ID                        = 1,
        POWER_CTL1                = 2,
        POWER_CTL2                = 4,
        CLOCKING_CTL              = 5,
        INTERFACE_CTL1            = 6,
        INTERFACE_CTL2            = 7,
        PASSTHROUGH_A             = 8,
        PASSTHROUGH_B             = 9,
        ANALOG_ZC_SR              = 0xa,
        PASSTHROUGH_GANG_CTL      = 0xc,
        PLAYBACK_CTL1             = 0xd,
        MISC_CTL                  = 0xe,
        PLAYBACK_CTL2             = 0xf,
        PASSTHROUGH_A_VOL         = 0x14,
        PASSTHROUGH_B_VOL         = 0x15,
        PCMA_VOL                  = 0x1a,
        PCMB_VOL                  = 0x1b,
        BEEP_FREQ_ON_TIME         = 0x1c,
        BEEP_VOL_OFF_TIME         = 0x1d,
        BEEP_TONE_CFG             = 0x1e,
        TONE_CTL                  = 0x1f,
        MASTER_A_VOL              = 0x20,
        MASTER_B_VOL              = 0x21,
        HEADPHONE_A_VOL           = 0x22,
        HEADPHONE_B_VOL           = 0x23,
        SPEAKER_A_VOL             = 0x24,
        SPEAKER_B_VOL             = 0x25,
        CHANNEL_MIXER_AND_SWAP    = 0x26,
        LIMIT_CTL1_THRESHOLDS     = 0x27,
        LIMIT_CTL2_RELEASE_RATE   = 0x28,
        LIMITER_ATTACK_RATE       = 0x29,
        OVERFLOW_AND_CLOCK_STATUS = 0x2e,
        BATTERY_COMPENSATION      = 0x2f,
        VP_BATTERY_LEVEL          = 0x30,
        SPEAKER_STATUS            = 0x31,
        CHARGE_PUMP_FREQUENCY     = 0x34
      };

      enum Source {
        ANALOG_1 = 1,
        ANALOG_2 = 2,
        ANALOG_3 = 4,
        ANALOG_4 = 8
      };

    protected:
      uint8_t convertVolume(uint8_t rawVolume) const;

    public:
      CS43L22ControlI2C(const typename TI2CPeripheral::Parameters& params);

      // initialise the device

      bool initialise() const;

      // pass through selection

      bool enablePassThroughA(Source source) const;
      bool enablePassThroughB(Source source) const;

      // power control

      bool powerOn() const;
      bool powerOff() const;

      // speaker/headphone power selection

      bool speakersOn() const;
      bool speakersOff() const;

      bool headphonesOn() const;
      bool headphonesOff() const;

      // volume controls

      bool masterVolume(uint8_t volume) const;
      bool headphonesVolume(uint8_t volume) const;
      bool speakersVolume(uint8_t volume) const;
      bool passThroughAVolume(uint8_t volume) const;
      bool passThroughBVolume(uint8_t volume) const;

      // beep generation

      bool beepSingle() const;
  };


  /**
   * Constructor
   * @param params The parameters class containing the I2C initialisation parameters.
   */

  template<class TI2CPeripheral>
  inline CS43L22ControlI2C<TI2CPeripheral>::CS43L22ControlI2C(const typename TI2CPeripheral::Parameters& params)
    : TI2CPeripheral(params) {

    // set our slave address. we're always a slave so there has to be an I/O feature
    // somewhere in the feature hierarchy that has this method

    this->setSlaveAddress(I2C_BUS_ADDRESS);
  }


  /**
   * Send the device initialisation sequence
   * @return true if it worked
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::initialise() const {

    return

        // turn the device power off

        powerOff()  &&

        // all outputs off

        speakersOff() &&
        headphonesOff() &&

        // auto-detect speed, MCLK/2

        this->writeByte(CLOCKING_CTL,0x81) &&

        // slave Mode, I2S Data Format

        this->writeByte(INTERFACE_CTL1,0x4) &&

        // all passthroughs muted

        this->writeByte(MISC_CTL,4) &&

        // volume levels

        masterVolume(200) &&
        headphonesVolume(200) &&
        speakersVolume(200) &&

        // power on

        powerOn() &&

        this->writeByte(PCMA_VOL,0xa) &&
        this->writeByte(PCMB_VOL,0xa) &&

        // disable the analog soft ramp

        this->writeByte(ANALOG_ZC_SR,0) &&

        // disable the digital soft ramp

        this->writeByte(MISC_CTL, 0x04) &&

        // disable the limiter attack level
        this->writeByte(LIMIT_CTL1_THRESHOLDS,0x00) &&

        // set the frequency of the beep

        this->writeByte(BEEP_FREQ_ON_TIME,0x80);
  }


  /**
   * Configure passthrough A for an analog source
   * @param sources The analog source
   * @return true if it worked
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::enablePassThroughA(Source source) const {

    return this->setMask(PASSTHROUGH_A,(uint8_t)source,0xf) &&   // enable the bits in the passthrough register
           this->setBits(MISC_CTL,0x40) &&                       // set the passthrough A enable bit
           this->resetBits(MISC_CTL,0x10);                       // clear the passthrough A mute bit
  }


  /**
   * Configure passthrough B for an analog source
   * @param sources The analog source
   * @return true if it worked
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::enablePassThroughB(Source source) const {

    return this->setMask(PASSTHROUGH_B,(uint8_t)source,0xf) &&   // enable the bits in the passthrough register
           this->setBits(MISC_CTL,0x80) &&                       // set the passthrough B enable bit
           this->resetBits(MISC_CTL,0x20);                       // clear the passthrough B mute bit
  }


  /**
   * Send a single beep
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::beepSingle() const {

    // toggle BEEP0

    return this->resetBits(BEEP_TONE_CFG,0x40) &&
           this->setBits(BEEP_TONE_CFG,0x40);
  }


  /**
   * power up
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::powerOn() const {
    return this->writeByte(POWER_CTL1,0x9e);
  }


  /**
   * power down
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::powerOff() const {
    return this->writeByte(POWER_CTL1,1);
  }


  /**
   * enable speaker output
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::speakersOn() const {
    return this->setMask(POWER_CTL2,0xa,0xf);
  }


  /**
   * disable speaker output
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::speakersOff() const {
    return this->setMask(POWER_CTL2,0xf,0xf);
  }


  /**
   * enable headphone output
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::headphonesOn() const {
    return this->setMask(POWER_CTL2,0xa0,0xf0);
  }


  /**
   * disable headphone output
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::headphonesOff() const {
    return this->setMask(POWER_CTL2,0xf0,0xf0);
  }


  /**
   * Set the master volume level
   * @param volume the volume value
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::masterVolume(uint8_t volume) const {

    return this->writeByte(MASTER_A_VOL,convertVolume(volume)) &&
           this->writeByte(MASTER_B_VOL,convertVolume(volume));
  }


  /**
   * Set the headphones volume level
   * @param volume the volume value
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::headphonesVolume(uint8_t volume) const {

    return this->writeByte(HEADPHONE_A_VOL,convertVolume(volume)) &&
           this->writeByte(HEADPHONE_B_VOL,convertVolume(volume));
  }

  /**
   * Set the speakers volume level
   * @param volume the volume value
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::speakersVolume(uint8_t volume) const {

    return this->writeByte(SPEAKER_A_VOL,convertVolume(volume)) &&
           this->writeByte(SPEAKER_B_VOL,convertVolume(volume));
  }


  /**
   * Set the passthrough A volume
   * @param volume the volume value
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::passThroughAVolume(uint8_t volume) const {

    return this->writeByte(PASSTHROUGH_A_VOL,convertVolume(volume)) &&
           this->writeByte(PASSTHROUGH_A_VOL,convertVolume(volume));
  }


  /**
   * Set the passthrough B volume
   * @param volume the volume value
   * @return true if succeeded
   */

  template<class TI2CPeripheral>
  inline bool CS43L22ControlI2C<TI2CPeripheral>::passThroughBVolume(uint8_t volume) const {

    return this->writeByte(PASSTHROUGH_B_VOL,convertVolume(volume)) &&
           this->writeByte(PASSTHROUGH_B_VOL,convertVolume(volume));
  }


  /**
   * Scale the volume value just like the ST sample code does
   * @param rawVolume
   * @return The scaled volume value
   */

  template<class TI2CPeripheral>
  inline uint8_t CS43L22ControlI2C<TI2CPeripheral>::convertVolume(uint8_t rawVolume) const {

    // Magic undocumented constant. Thanks ST.
    return rawVolume>0xE6 ? rawVolume-0xE7 : rawVolume+0x19;
  }
}
