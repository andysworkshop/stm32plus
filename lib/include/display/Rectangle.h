/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief A rectangle class
     */

    struct Rectangle {

      /// X co-ordinate
      int16_t X;

      /// Y co-ordinate
      int16_t Y;

      /// Width
      int16_t Width;

      /// Height
      int16_t Height;


      Rectangle() {}

      /**
       * Constructor. Set up the parameters.
       * @param[in] x The x-coord
       * @param[in] y The y-coord
       * @param[in] width The width (cannot be negative)
       * @param[in] height The height (cannot be negative)
       */

      Rectangle(int16_t x,int16_t y,int16_t width,int16_t height) {
        X=x;
        Y=y;
        Width=width;
        Height=height;
      }


      /**
       * Constructor with top-left co-ord and size
       * @param[in] p The top-left co-ord
       * @param[in] size The rectangle width and height
       */

      Rectangle(const Point& p,const Size& size) {
        X=p.X;
        Y=p.Y;
        Width=size.Width;
        Height=size.Height;
      }


      /**
       * Copy constructor
       */

      Rectangle(const Rectangle& src) {
        assign(src);
      }


      /**
       * Assignment operator
       */

      Rectangle& operator=(const Rectangle& src) {
        assign(src);
        return *this;
      }


      /**
       * Assign src to this
       */

      void assign(const Rectangle& src) {
        X=src.X;
        Y=src.Y;
        Width=src.Width;
        Height=src.Height;
      }


      /**
       * Equality operators
       */

      bool operator==(const Rectangle& rhs) const {
        return X==rhs.X && Y==rhs.Y && Width==rhs.Width && Height==rhs.Height;
      }

      bool operator!=(const Rectangle& rhs) const {
        return X!=rhs.X || Y!=rhs.Y || Width!=rhs.Width || Height!=rhs.Height;
      }

      /**
       * Get the size of the rectangle
       * @return the size structure
       */

      Size getSize() const {
        Size s;

        s.Width=Width;
        s.Height=Height;

        return s;
      }


      /**
       * Get the top-left corner
       * @return the top-left
       */

      Point getTopLeft() const {
        Point p;

        p.X=X;
        p.Y=Y;

        return p;
      }


      /**
       * Check if the given point is contained in this rectangle
       * @return true if is contained
       */

      bool containsPoint(const Point& p) const {
        return p.X>=X && p.X<=X+Width && p.Y>=Y && p.Y<=Y+Height;
      }
    };


    /**
     * Derivation of rectangle that accepts negative width/height and internally
     * converts them to positive values while keeping the rect at the same position
     */

    struct NormalizedRectangle : Rectangle {

      /**
       * Constructor
       * @param x
       * @param y
       * @param width Can be negative
       * @param height Can be negative
       */

      NormalizedRectangle(int16_t x,int16_t y,int16_t width,int16_t height)
        : Rectangle(x,y,width,height) {

        if(Height<0) {
          Y+=(height+1);
          Height=-Height;
        }

        if(Width<0) {
          X+=(Width+1);
          Width=-Width;
        }
      }
    };
  }
}
