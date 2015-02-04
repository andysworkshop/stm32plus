/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/Rectangle.h"
#include "display/graphic/PanelConfiguration.h"
#include "display/graphic/FrameBuffer.h"
#include "display/graphic/oled/ssd1306/SSD1306Orientation.h"
#include "timing/MillisecondTimer.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic SSD1306 template. The user can specialise based on the desired
     * orientation and access mode.
     */

    template<Orientation TOrientation,class TAccessMode>
    class SSD1306 : public SSD1306Orientation<TOrientation,TAccessMode> {

      public:
        enum {
          SHORT_SIDE = 64,
          LONG_SIDE = 128,

          DEFAULT_CONTRAST = 0xc8
        };

        typedef uint32_t tCOLOUR;

        /**
         * Unpacked colour representation. That's just one byte here.
         */

        struct UnpackedColour {
          uint8_t value;        // monochrome display
        };

      protected:
        TAccessMode& _accessMode;
        FrameBuffer<uint8_t> _frameBuffer;
        uint8_t _plotMasks[8];
        uint8_t _contrast;

      public:
        SSD1306(TAccessMode& accessMode);

        void initialise() const;

        void unpackColour(tCOLOUR src,UnpackedColour& dest) const;
        void unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const;

        void writePixel(const UnpackedColour& cr);
        void fillPixels(uint32_t numPixels,const UnpackedColour& cr);

        void sleep() const;
        void wake() const;
        void beginWriting() const;

        void setContrast(uint8_t contrast) const;
        void setDisplayOnOff(uint8_t onOff) const;
        void setDisplayClock(uint8_t clock) const;
        void setMultiplexRatio(uint8_t ratio) const;
        void setDisplayOffset(uint8_t offset) const;
        void setStartLine(uint8_t startLine) const;
        void setChargePump(uint8_t flag) const;
        void setAddressingMode(uint8_t mode) const;
        void setCommonRemap(uint8_t remap) const;
        void setCommonConfig(uint8_t config) const;
        void setPrechargePeriod(uint8_t period) const;
        void setVComh(uint8_t vcomh) const;
        void setEntireDisplay(uint8_t flag) const;
        void setInverseDisplay(uint8_t inverse) const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all.
     */

    template<Orientation TOrientation,class TAccessMode>
    inline SSD1306<TOrientation,TAccessMode>::SSD1306(TAccessMode& accessMode)
      : SSD1306Orientation<TOrientation,TAccessMode>(accessMode),
        _accessMode(accessMode),
        _frameBuffer(Size(128,64),1024),
        _contrast(DEFAULT_CONTRAST) {

      int8_t i;

      for(i=0;i<8;i++)
        _plotMasks[i]=~(1 << i);
    }


    /**
     * Initialise the LCD. Do the reset sequence.
     */

    template<Orientation TOrientation,class TAccessMode>
    inline void SSD1306<TOrientation,TAccessMode>::initialise() const {

      // the reset sequence must happen after Vdd has become stable. We'll give it 10ms.

      MillisecondTimer::delay(10);

      // reset the device

      this->_accessMode.reset();

      // initialise the panel

      setDisplayOnOff(0);             // Display Off (0x00/0x01)
      setDisplayClock(0x80);          // Set Clock as 100 Frames/Sec
      setMultiplexRatio(0x3F);        // 1/64 Duty (0x0F~0x3F)
      setDisplayOffset(0);            // Shift Mapping RAM Counter (0x00~0x3F)
      setStartLine(0);                // Set Mapping RAM Display Start Line (0x00~0x3F)
      setChargePump(0x04);            // Enable Embedded DC/DC Converter (0x00/0x04)
      setCommonRemap(0x8);            // Set COM/Row Scan Direction (0x00/0x08)
      setCommonConfig(0x10);          // Set Sequential Configuration (0x00/0x10)
      setAddressingMode(0);

      // orientation-specific stuff

      this->setOrientation();

      // continue

      setContrast(_contrast);         // Set SEG Output Current
      setPrechargePeriod(0xF1);       // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
      setVComh(0x40);                 // Set VCOM Deselect Level
      setEntireDisplay(0);            // Disable Entire Display On (0x00/0x01)
      setInverseDisplay(0);           // Disable Inverse Display On (0x00/0x01)

      // turn the display on

      setDisplayOnOff(1);             // Display On (0x00/0x01)
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,class TAccessMode>
    inline void SSD1306<TOrientation,TAccessMode>::sleep() const {
      setDisplayOnOff(0);
      setEntireDisplay(1);
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,class TAccessMode>
    inline void SSD1306<TOrientation,TAccessMode>::wake() const {
      setEntireDisplay(0);
      setDisplayOnOff(1);
    }


    /**
     * Not relevant because we go indirect via the framebuffer
     */

    template<Orientation TOrientation,class TAccessMode>
    inline void SSD1306<TOrientation,TAccessMode>::beginWriting() const {
    }


    /**
     * Display on/off
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setDisplayOnOff(uint8_t onOff) const {
      this->_accessMode.writeCommand(0xAE | onOff);
    }


    /**
     * display clock
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setDisplayClock(uint8_t clock) const {
      this->_accessMode.writeCommand(0xD5);     // Set Display Clock Divide Ratio / Oscillator Frequency
      this->_accessMode.writeCommand(clock);
    }


    /**
     * multiplex ratio
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setMultiplexRatio(uint8_t ratio) const {
      this->_accessMode.writeCommand(0xA8);     // Set Multiplex Ratio
      this->_accessMode.writeCommand(ratio);      // Default => 0x3F (1/64 Duty)
    }


    /**
     * display offset
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setDisplayOffset(uint8_t offset) const {
      this->_accessMode.writeCommand(0xD3);         // Set Display Offset
      this->_accessMode.writeCommand(offset);       //   Default => 0x00
    }


    /**
     * starting line
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setStartLine(uint8_t startLine) const {
      this->_accessMode.writeCommand(0x40 | startLine);   // Set Display Start Line
    }


    /*
     * set the charge pump
     * 0x10 (0x00) => Disable Charge Pump
     * 0x14 (0x04) => Enable Charge Pump
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setChargePump(uint8_t flag) const {
      this->_accessMode.writeCommand(0x8D);     // Set Charge Pump
      this->_accessMode.writeCommand(0x10 | flag);      //   Default => 0x10
    }


    /**
     * Set the addressing mode
     * 0x00 => Horizontal Addressing Mode
     * 0x01 => Vertical Addressing Mode
     * 0x02 => Page Addressing Mode
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setAddressingMode(uint8_t mode) const {
      this->_accessMode.writeCommand(0x20);     // Set Memory Addressing Mode
      this->_accessMode.writeCommand(mode);     // Default => 0x02
    }


    /**
     * Set the common remap
     * Default => 0xC0
     * 0xC0 (0x00) => Scan from COM0 to 63
     * 0xC8 (0x08) => Scan from COM63 to 0
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setCommonRemap(uint8_t remap) const {
      this->_accessMode.writeCommand(0xC0 | remap);     // Set COM Output Scan Direction
    }


    /**
     * Set the common config
     * Alternative COM Pin Configuration
     *  Disable COM Left/Right Re-Map
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setCommonConfig(uint8_t config) const {
      this->_accessMode.writeCommand(0xDA);               // Set COM Pins Hardware Configuration
      this->_accessMode.writeCommand(0x02 | config);      //   Default => 0x12 (0x10)
    }


    /**
     * Set the contrast
     * @param[in] contrast The contrast value: higher = more contrast.
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setContrast(uint8_t contrast) const {
      this->_accessMode.writeCommand(0x81);         // Set Contrast Control
      this->_accessMode.writeCommand(contrast);     //   Default => 0x7F
    }


    /**
     * Set the precharge period
     * D[3:0] => Phase 1 Period in 1~15 Display Clocks
     * D[7:4] => Phase 2 Period in 1~15 Display Clocks
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setPrechargePeriod(uint8_t period) const {
      this->_accessMode.writeCommand(0xD9);     // Set Pre-Charge Period
      this->_accessMode.writeCommand(period);     //   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
    }


    /**
     * Set VCOMH
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setVComh(uint8_t vcomh) const {
      this->_accessMode.writeCommand(0xDB);     // Set VCOMH Deselect Level
      this->_accessMode.writeCommand(vcomh);    //   Default => 0x20 (0.77*VCC)
    }


    /**
     * Set the entire display flag
     * Default => 0xA4
     * 0xA4 (0x00) => Normal Display
     * 0xA5 (0x01) => Entire Display On
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setEntireDisplay(uint8_t flag) const {
      this->_accessMode.writeCommand(0xA4 | flag);      // Set Entire Display On / Off
    }


    /**
     * Set the inverse display flag
     * Default => 0xA6
     * 0xA6 (0x00) => Normal Display
     * 0xA7 (0x01) => Inverse Display On
     */

    template<Orientation TOrientation,class TAccessMode>
    void SSD1306<TOrientation,TAccessMode>::setInverseDisplay(uint8_t inverse) const {
      this->_accessMode.writeCommand(0xA6 | inverse);     // Set Inverse Display On/Off
    }


    /**
     * Unpack the colour from RRGGBB to one byte monochrome
     * @param src
     * @param dest
     */

    template<Orientation TOrientation,class TAccessMode>
    inline void SSD1306<TOrientation,TAccessMode>::unpackColour(tCOLOUR src,UnpackedColour& dest) const {

      unpackColour((src >> 16) & 0xff,(src >> 8) & 0xff,src & 0xff,dest);
    }


    /**
     * Unpack colour by its components to monochrome. If the input colour is <50% brightness then the result
     * is black, otherwise it's white.
     * @param red
     * @param green
     * @param blue
     * @param dest
     */

    template<Orientation TOrientation,class TAccessMode>
    inline void SSD1306<TOrientation,TAccessMode>::unpackColour(uint8_t red,uint8_t green,uint8_t blue,UnpackedColour& dest) const {

      uint8_t brightness;

      brightness=std::max<uint8_t>(red,std::max<uint8_t>(green,blue));
      dest.value=brightness>127;
    }


    /**
     * Write a single pixel to the current output position.
     * @param cr The pixel to write
     */

    template<Orientation TOrientation,class TAccessMode>
    inline void SSD1306<TOrientation,TAccessMode>::writePixel(const UnpackedColour& cr) {

      uint8_t value;
      uint16_t byteIndex;
      uint32_t bitIndex;

      byteIndex=((this->_cursorPos.Y/8)*128)+this->_cursorPos.X;
      bitIndex=(byteIndex*8)+(this->_cursorPos.Y % 8);

      if(cr.value)
        _frameBuffer.getBitbandAddress()[bitIndex]=1;
      else
        _frameBuffer.getBitbandAddress()[bitIndex]=0;

      value=_frameBuffer[byteIndex];
      this->_accessMode.writeData(&value,1);

      // update cursor position

      if(++this->_cursorPos.X==this->_window.X+this->_window.Width) {
        this->_cursorPos.X=this->_window.X;
        this->_cursorPos.Y++;
      }
    }


    /**
     * Fill a block of pixels with the same colour. This operation will issue the
     * beginWriting() command for you.
     * @param numPixels how many
     * @param cr The unpacked colour to write
     */

    template<Orientation TOrientation,class TAccessMode>
    inline void SSD1306<TOrientation,TAccessMode>::fillPixels(uint32_t numPixels,const UnpackedColour& cr) {

      uint8_t value,bitpos;
      uint8_t *ptr;
      uint16_t index,valuesToWrite,lastIndex;

      // optimisation for clearing the display

      if(numPixels==128*64) {
        memset(&_frameBuffer[0],cr.value ? 0xff : 0,1024);
        this->_accessMode.writeData(&_frameBuffer[0],1024);
      }
      else {

        // do each pixel separately

        ptr=nullptr;

        while(numPixels--) {

          index=((this->_cursorPos.Y/8)*128)+this->_cursorPos.X;
          bitpos=this->_cursorPos.Y % 8;

          value=_frameBuffer[index];
          value=(value & _plotMasks[bitpos]) | (cr.value << bitpos);

          _frameBuffer[index]=value;

          if(ptr==nullptr) {
            ptr=&_frameBuffer[index];
            valuesToWrite=1;
            lastIndex=index;
          }
          else {
            if(lastIndex!=index) {
              lastIndex=index;
              valuesToWrite++;
            }
          }

          // update cursor position

          if(++this->_cursorPos.X==this->_window.X+this->_window.Width) {
            this->_cursorPos.X=this->_window.X;
            this->_cursorPos.Y++;
          }
        }

        // write the buffer (optimise this to just write the affected section)

        this->_accessMode.writeData(ptr,valuesToWrite);
      }
    }
  }
}
