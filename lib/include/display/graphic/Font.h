/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Implementation of a font compiled into memory
     */

    class FontBase {

      public:

        /**
         * The font type (bitmap of pixels or LZG compressed bitmap)
         */

        enum FontType {
          FONT_BITMAP,//!< FONT_BITMAP
          FONT_LZG    //!< FONT_LZG
        } _fontType;

      private:
        uint8_t _characterCount;
        uint8_t _height;
        int8_t _characterSpacing;
        uint8_t _firstCharacter;
        const struct FontChar * _characters;

      public:
        FontBase(FontType type,uint8_t firstChar,uint8_t characterCount,uint8_t height,int8_t spacing,const struct FontChar *characters);

        void getCharacter(uint8_t character,const FontChar*& fc) const;
        uint16_t getId() const;
        uint8_t getHeight() const;
        int8_t getCharacterSpacing() const;
        FontType getType() const;
    };


    /**
     * Get the font height.
     */

    inline uint8_t FontBase::getHeight() const {
      return _height;
    }


    /**
     * Get the character spacing
     */

    inline int8_t FontBase::getCharacterSpacing() const {
      return _characterSpacing;
    }


    /**
     * stub types to allow method overloading in the GraphicsLibrary class. this first one is for
     * bitmap fonts - the original font format
     */

    class Font : public FontBase {
      public:
        Font(uint8_t firstChar,uint8_t characterCount,uint8_t height,int8_t spacing,const struct FontChar *characters)
          : FontBase(FONT_BITMAP,firstChar,characterCount,height,spacing,characters) {
        }
    };


    /**
     * and this one is for the Lzg TrueType fonts
     */

    class LzgFont : public FontBase {
      public:
        LzgFont(uint8_t firstChar,uint8_t characterCount,uint8_t height,int8_t spacing,const struct FontChar *characters)
          : FontBase(FONT_LZG,firstChar,characterCount,height,spacing,characters) {
        }
    };
  }
}
