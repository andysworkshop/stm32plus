/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Constructor
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline GraphicsLibrary<TDevice,TDeviceAccessMode>::GraphicsLibrary(TDeviceAccessMode& accessMode)
      : TDevice(accessMode) {

      _fontFilledBackground=true;

      // initialise the panel

      this->initialise();
    }


    /**
     * set the foreground
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::setForeground(tCOLOUR cr) {
      this->unpackColour(cr,_foreground);
    }

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::setForeground(uint8_t r,uint8_t g,uint8_t b) {
      this->unpackColour(r,g,b,_foreground);
    }

    /**
     * set the background
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::setBackground(tCOLOUR cr) {
      this->unpackColour(cr,_background);
    }

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::setBackground(uint8_t r,uint8_t g,uint8_t b) {
      this->unpackColour(r,g,b,_background);
    }

    /**
     * Get a full-screen rectangle
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline Rectangle GraphicsLibrary<TDevice,TDeviceAccessMode>::getFullScreenRectangle() const {
      return Rectangle(0,0,this->getWidth(),this->getHeight());
    }

    /**
     * Get the panel maximum x-coord
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline int16_t GraphicsLibrary<TDevice,TDeviceAccessMode>::getXmax() const {
      return this->getWidth()-1;
    }

    /**
     * Get the panel maximum y-coord
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline int16_t GraphicsLibrary<TDevice,TDeviceAccessMode>::getYmax() const {
      return this->getHeight()-1;
    }

    /**
     * clear screen to the background colour
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::clearScreen() {

      this->moveTo(
          Rectangle(
              0,
              0,
              this->getWidth(),
              this->getHeight()
          )
      );

      this->fillPixels((uint32_t)this->getWidth()*(uint32_t)this->getHeight(),_background);
    }


    /**
     * Convenience helper to move to a point extending to the
     * end of the display
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::moveToPoint(const Point& pt) const {
      this->moveTo(Rectangle(pt.X,pt.Y,this->getWidth()-pt.X,this->getHeight()-pt.Y));
    }
  }
}
