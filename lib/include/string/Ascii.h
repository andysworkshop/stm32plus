/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/**
 * @file Ascii.h
 * Wrapper class for converting types to ASCII and offering an operator const char *() to get
 * at the results. Intended for inline use with implicit conversion.
 */

namespace stm32plus {
  namespace StringUtil {

    /**
     * ASCII converter class
     */

    class Ascii {

      protected:
        scoped_array<char> _buffer;

      public:
        Ascii(long l);

        operator const char *() const;
    };


    /**
     * Constructor with long
     * @param l The long to convert
     */

    inline Ascii::Ascii(long l)
      : _buffer(new char[20]) {

      StringUtil::modp_uitoa10(l,_buffer.get());
    }


    /**
     * Cast conversion
     */

    inline Ascii::operator const char *() const {
      return _buffer.get();
    }
  }
}
