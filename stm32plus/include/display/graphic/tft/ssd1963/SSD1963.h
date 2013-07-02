/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#include "display/graphic/tft/ssd1963/SSD1963Colour.h"
#include "display/graphic/tft/ssd1963/SSD1963Orientation.h"
#include "display/graphic/tft/ssd1963/SSD1963Gamma.h"
#include "display/graphic/tft/ssd1963/commands/AllCommands.h"


namespace stm32plus {
  namespace display {

    /**
     * Generic SSD1963 template. The user can specialise based on the desired colour
     * depth, orientation and access mode.
     * @tparam TOrientation An appropriate specialisation of SSD1963Orientation
     * @tparam TColourDepth An appropriate specialisation of SSD1963Colour
     * @tparam TAccessMode The type of access mode to use for this setup
     * @tparam TPanelTraits Identifying information about this panel, such as its dimensions
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    class SSD1963 : public SSD1963Colour<TColourDepth,TAccessMode,TPanelTraits>,
                    public SSD1963Orientation<TOrientation,TAccessMode,TPanelTraits> {

      protected:

        TAccessMode& _accessMode;

      public:
        SSD1963(TAccessMode& accessMode);

        void initialise() const;

        void applyGamma(SSD1963Gamma& gamma) const;
        void sleep() const;
        void wake() const;
        void beginWriting() const;
        void setScrollArea(uint16_t y,uint16_t height) const;
    };


    /**
     * Constructor. Pass the access mode reference up the hierarchy where it'll get stored in the
     * common base class for use by all. Store the dimensions
     * @param accessMode The access mode class to use for this instantiation
     * @param shortSide The short side of the display in pixels
     * @param longSide The long side of the display in pixels
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline SSD1963<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::SSD1963(TAccessMode& accessMode)
      : SSD1963Colour<TColourDepth,TAccessMode,TPanelTraits>(accessMode),
        SSD1963Orientation<TOrientation,TAccessMode,TPanelTraits>(accessMode),
        _accessMode(accessMode) {
    }


    /**
     * Initialise the LCD. Do the reset sequence.
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void SSD1963<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::initialise() const {

      const unsigned char FSMC_Bank = 0;

      // slow down the device for the register setup

      FSMC_Bank1->BTCR[FSMC_Bank+1] = FSMC_BTR1_ADDSET_0 | FSMC_BTR1_DATAST_2 | FSMC_BTR1_BUSTURN_0 ;
      FSMC_Bank1->BTCR[FSMC_Bank] = FSMC_BCR1_MWID_0 | FSMC_BCR1_WREN | FSMC_BCR1_MBKEN;

      // do a software reset

      this->_accessMode.writeCommand(ssd1963::SOFTWARE_RESET);
      MillisecondTimer::delay(1);

      // reset the device

      this->_accessMode.reset();

      // set the PLL multipler. the pixel clock is sourced from the PLL.
      // PLL frequency = (external oscillator frequency * N) / M
      // where 250Mhz < (external oscillator frequency * N) < 800Mhz

      this->_accessMode.writeCommand(ssd1963::SET_PLL_MN);
      this->_accessMode.writeData(TPanelTraits::PLL_M);
      this->_accessMode.writeData(TPanelTraits::PLL_N);
      this->_accessMode.writeData(4);

      // enable the PLL

      this->_accessMode.writeCommand(ssd1963::SET_PLL);
      this->_accessMode.writeData(1);
      MillisecondTimer::delay(2);
      this->_accessMode.writeCommand(ssd1963::SET_PLL);
      this->_accessMode.writeData(3);
      MillisecondTimer::delay(2);

      // do a software reset

      this->_accessMode.writeCommand(ssd1963::SOFTWARE_RESET);
      MillisecondTimer::delay(1);

      // set the pixel clock

      this->_accessMode.writeCommand(ssd1963::SET_LSHIFT_FREQUENCY);
      this->_accessMode.writeData(TPanelTraits::PIXEL_CLOCK >> 16);
      this->_accessMode.writeData((TPanelTraits::PIXEL_CLOCK >> 8) & 0xff);
      this->_accessMode.writeData(TPanelTraits::PIXEL_CLOCK & 0xff);

      // set up the LCD mode (colour depth, dithering)

      this->_accessMode.writeCommand(ssd1963::SET_LCD_MODE);
      this->_accessMode.writeData(this->getLcdModeBits());                // 0=18 bit, 1=24 bit
      this->_accessMode.writeData(0);                                     // TFT mode
      this->_accessMode.writeData((TPanelTraits::LONG_SIDE-1) >> 8);      // horizontal size
      this->_accessMode.writeData((TPanelTraits::LONG_SIDE-1) & 0xff);
      this->_accessMode.writeData((TPanelTraits::SHORT_SIDE-1) >> 8);     // vertical size
      this->_accessMode.writeData((TPanelTraits::SHORT_SIDE-1) & 0xff);
      this->_accessMode.writeData(0);                                     // RGB transfer order

      // horizontal timing

      this->_accessMode.writeCommand(ssd1963::SET_HORIZONTAL_PERIOD);
      this->_accessMode.writeData((TPanelTraits::H_BACK_PORCH+TPanelTraits::H_PERIOD+TPanelTraits::H_FRONT_PORCH) >> 8);
      this->_accessMode.writeData((TPanelTraits::H_BACK_PORCH+TPanelTraits::H_PERIOD+TPanelTraits::H_FRONT_PORCH) & 0xff);
      this->_accessMode.writeData(TPanelTraits::H_BACK_PORCH >> 8);
      this->_accessMode.writeData(TPanelTraits::H_BACK_PORCH & 0xff);
      this->_accessMode.writeData(TPanelTraits::H_SYNC_PULSE_WIDTH-1);
      this->_accessMode.writeData(TPanelTraits::H_SYNC_POSITION >> 8);
      this->_accessMode.writeData(TPanelTraits::H_SYNC_POSITION & 0xff);
      this->_accessMode.writeData(0);

      // vertical timing

      this->_accessMode.writeCommand(ssd1963::SET_VERTICAL_PERIOD);
      this->_accessMode.writeData((TPanelTraits::V_BACK_PORCH+TPanelTraits::V_PERIOD+TPanelTraits::V_FRONT_PORCH) >> 8);
      this->_accessMode.writeData((TPanelTraits::H_BACK_PORCH+TPanelTraits::V_PERIOD+TPanelTraits::V_FRONT_PORCH) & 0xff);
      this->_accessMode.writeData(TPanelTraits::V_BACK_PORCH >> 8);
      this->_accessMode.writeData(TPanelTraits::V_BACK_PORCH & 0xff);
      this->_accessMode.writeData(TPanelTraits::V_SYNC_PULSE_WIDTH-1);
      this->_accessMode.writeData(TPanelTraits::V_SYNC_POSITION >> 8);
      this->_accessMode.writeData(TPanelTraits::V_SYNC_POSITION & 0xff);
      this->_accessMode.writeData(0);

      // enable packed 12-bit mode (2 16-bit FSMC transfers per pixel). This requires
      // that DB0..DB11 are connected. Any other pins exposed by the interface
      // (i.e. DB12..23) are unused.

      this->_accessMode.writeCommand(ssd1963::SET_PIXEL_DATA_INTERFACE);
      this->_accessMode.writeData(1);
      MillisecondTimer::delay(5);

      // start up the display

      this->_accessMode.writeCommand(ssd1963::SLEEP_OUT);
      MillisecondTimer::delay(10);
      this->_accessMode.writeCommand(ssd1963::DISPLAY_INVERSION_OFF);
      this->_accessMode.writeCommand(ssd1963::IDLE_MODE_OFF);
      this->_accessMode.writeCommand(ssd1963::NORMAL_DISPLAY_MODE_ON);

      // set the panel orientation

      this->_accessMode.writeCommand(ssd1963::SET_ADDRESS_MODE,this->getAddressMode());

      // reset the scroll position

      this->setScrollPosition(0);

      // wait the required number of ms. before we can turn the display on

      MillisecondTimer::delay(125);
      this->_accessMode.writeCommand(ssd1963::DISPLAY_ON);

      // the user needs to speed up the FSMC now that this has completed
    }


    /**
     * Apply the panel gamma settings
     * @param gamma The gamma class containing the one value
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void SSD1963<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::applyGamma(SSD1963Gamma& gamma) const {

      this->_accessMode.writeCommand(ssd1963::GAMMA_SET,gamma[0]);
    }


    /**
     * Send the panel to sleep
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void SSD1963<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::sleep() const {

      this->_accessMode.writeCommand(ssd1963::DISPLAY_OFF);
      this->_accessMode.writeCommand(ssd1963::SLEEP_IN);
      MillisecondTimer::delay(5);
    }


    /**
     * Wake the panel up
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void SSD1963<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::wake() const {

      this->_accessMode.writeCommand(ssd1963::SLEEP_OUT);
      MillisecondTimer::delay(120);
      this->_accessMode.writeCommand(ssd1963::DISPLAY_ON);
      MillisecondTimer::delay(5);
    }


    /**
     * Issue the command that allows graphics ram writing to commence
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void SSD1963<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::beginWriting() const {
      this->_accessMode.writeCommand(ssd1963::MEMORY_WRITE);
    }


    /**
     * Set the scroll area to a full-width rectangle region
     * @param y The y-co-ord of the region
     * @param height The height of the region
     */

    template<Orientation TOrientation,ColourDepth TColourDepth,class TAccessMode,class TPanelTraits>
    inline void SSD1963<TOrientation,TColourDepth,TAccessMode,TPanelTraits>::setScrollArea(uint16_t y,uint16_t height) const {
      uint16_t bfa;

      bfa=TPanelTraits::LONG_SIDE-height-y;

      this->_accessMode.writeCommand(ssd1963::VERTICAL_SCROLLING_DEFINITION);
      this->_accessMode.writeData(y >> 8);
      this->_accessMode.writeData(y & 0xff);
      this->_accessMode.writeData(height >> 8);
      this->_accessMode.writeData(height & 0xff);
      this->_accessMode.writeData(bfa >> 8);
      this->_accessMode.writeData(bfa & 0xff);
    }
  }
}
