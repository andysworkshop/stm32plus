/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * @brief A point co-ordinate structure
     */

    struct Point {

      /// X co-ordinate
      int16_t X;

      /// Y co-ordinate
      int16_t Y;

      /**
       * Return a (0,0) point.
       */

      static const Point Origin;

      Point() {}

      /**
       * Constructor with parameters
       * @param[in] x The x-co-ord
       * @param[in] y The y-co-ord
       */

      Point(int16_t x,int16_t y) {
        X=x;
        Y=y;
      }


      /**
       * Copy constructor
       */

      Point(const Point& p) {
        assign(p);
      }


      /*
       * Assignment operator
       */

      Point& operator=(const Point& p) {
        assign(p);
        return *this;
      }


      /*
       * Assign src to this
       */

      void assign(const Point& p) {
        X=p.X;
        Y=p.Y;
      }


      /*
       * Equality operators
       */

      bool operator==(const Point& rhs) const {
        return rhs.X==X && rhs.Y==Y;
      }

      bool operator!=(const Point& rhs) const {
        return rhs.X!=X || rhs.Y!=Y;
      }
    };
  }
}
