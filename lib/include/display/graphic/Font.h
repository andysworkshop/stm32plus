/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
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
        const struct FontChar * _characters;
        uint8_t _characterCount;
        uint8_t _firstCharacter;
        uint8_t _height;
        int8_t _characterSpacing;
        uint8_t _lastCharacter;

      public:
        FontBase(FontType type,uint8_t firstChar,uint8_t characterCount,uint8_t height,int8_t spacing,const struct FontChar *characters);

        void getCharacter(uint8_t character,const FontChar*& fc) const;
        uint16_t getId() const;
        uint8_t getHeight() const;
        int8_t getCharacterSpacing() const;
        FontType getType() const;
    };


    /**
     * Constructor
     */

    inline FontBase::FontBase(FontType type,uint8_t firstChar,uint8_t characterCount,uint8_t height,int8_t spacing,const struct FontChar *characters)
      : _fontType(type),
        _characters(characters),
        _characterCount(characterCount),
        _firstCharacter(firstChar),
        _height(height),
        _characterSpacing(spacing) {

      _lastCharacter=_characters[characterCount-1].Code;
    }


    /**
     * Get the font character definition address
     */

    inline void FontBase::getCharacter(uint8_t character,const FontChar*& fc) const {

      const FontChar *ptr;
      int16_t i;

      // the bulk of the characters are in sequential order, see if we can
      // index directly into the character array to find it

      ptr=nullptr;

      if(character>=_firstCharacter && character<_lastCharacter) {

        // the character is in range and indexable, is it in sequential place?

        if(character-_firstCharacter<_characterCount && _characters[character-_firstCharacter].Code==character)
          ptr=&_characters[character-_firstCharacter];
        else {

          // did't find it, search for it going backwards because the likelihood is that
          // it's towards the end of the array

          for(i=_characterCount-1;i>=0;i--) {

            if(_characters[i].Code==character) {
              ptr=&_characters[i];
              break;
            }
          }
        }
      }

      if(ptr==nullptr)
        ptr=&_characters[0];        // didn't found it, default to first char so user knows something is wrong

      // set up the return data

      fc=ptr;
    }


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
