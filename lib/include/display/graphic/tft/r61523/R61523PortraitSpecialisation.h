/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Specialisation of R61523Orientation for the panel in PORTRAIT mode.
     * @tparam TAccessMode the access mode implementation, e.g. Fsmc16BitAccessMode
     */

    template<class TAccessMode>
    class R61523Orientation<PORTRAIT,TAccessMode> {

      private:
        TAccessMode& _accessMode;

      protected:
        R61523Orientation(TAccessMode& accessMode);

        void setOrientation() const;

      public:
        constexpr int16_t getWidth() const;
        constexpr int16_t getHeight() const;

        void moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const;
        void moveTo(const Rectangle& rc) const;
        void moveX(int16_t xstart,int16_t xend) const;
        void moveY(int16_t ystart,int16_t yend) const;
    };


    /**
     * Constructor
     */

    template<class TAccessMode>
    inline R61523Orientation<PORTRAIT,TAccessMode>::R61523Orientation(TAccessMode& accessMode)
      : _accessMode(accessMode) {
    }


    /**
     * Execute the commands for setting the orientation to portrait
     */

    template<class TAccessMode>
    inline void R61523Orientation<PORTRAIT,TAccessMode>::setOrientation() const {
      _accessMode.writeCommand(r61523::SET_ADDRESS_MODE,0x40);
    }


    /**
     * Get the width in pixels
     * @return 360px
     */

    template<class TAccessMode>
    constexpr inline int16_t R61523Orientation<PORTRAIT,TAccessMode>::getWidth() const {
      return 360;
    }


    /**
     * Get the height in pixels
     * @return 640px
     */

    template<class TAccessMode>
    constexpr inline int16_t R61523Orientation<PORTRAIT,TAccessMode>::getHeight() const {
      return 640;
    }


    /**
     * Move the display output rectangle
     * @param rc The display output rectangle
     */

    template<class TAccessMode>
    inline void R61523Orientation<PORTRAIT,TAccessMode>::moveTo(const Rectangle& rc) const {
      moveTo(rc.X,rc.Y,rc.X+rc.Width-1,rc.Y+rc.Height-1);
    }


    /**
     * Move the display rectangle to the rectangle described by the co-ordinates
     * @param xstart starting X position
     * @param ystart starting Y position
     * @param xend ending X position
     * @param yend ending Y position
     */

    template<class TAccessMode>
    inline void R61523Orientation<PORTRAIT,TAccessMode>::moveTo(int16_t xstart,int16_t ystart,int16_t xend,int16_t yend) const {

      _accessMode.writeCommand(r61523::SET_COLUMN_ADDRESS,xstart >> 8);
      _accessMode.writeData(xstart & 0xff);
      _accessMode.writeData(xend >> 8);
      _accessMode.writeData(xend & 0xff);

      _accessMode.writeCommand(r61523::SET_PAGE_ADDRESS,ystart >> 8);
      _accessMode.writeData(ystart & 0xff);
      _accessMode.writeData(yend >> 8);
      _accessMode.writeData(yend & 0xff);
    }


    /**
     * Move the X position
     * @param xstart The new X start position
     * @param xend The new X end position
     */

    template<class TAccessMode>
    inline void R61523Orientation<PORTRAIT,TAccessMode>::moveX(int16_t xstart,int16_t xend) const {
      _accessMode.writeCommand(r61523::SET_COLUMN_ADDRESS,xstart >> 8);
      _accessMode.writeData(xstart & 0xff);
      _accessMode.writeData(xend >> 8);
      _accessMode.writeData(xend & 0xff);
    }


    /**
     * Move the Y position
     * @param ystart The new Y start position
     * @param yend The new Y end position
     */

    template<class TAccessMode>
    inline void R61523Orientation<PORTRAIT,TAccessMode>::moveY(int16_t ystart,int16_t yend) const {
      _accessMode.writeCommand(r61523::SET_PAGE_ADDRESS,ystart >> 8);
      _accessMode.writeData(ystart & 0xff);
      _accessMode.writeData(yend >> 8);
      _accessMode.writeData(yend & 0xff);
    }
  }
}

