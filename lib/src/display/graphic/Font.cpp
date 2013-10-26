/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/display/tft.h"


namespace stm32plus {
  namespace display {

    /**
     * Constructor
     */

    FontBase::FontBase(FontType type,uint8_t firstChar,uint8_t characterCount,uint8_t height,int8_t spacing,const struct FontChar *characters)
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

    void FontBase::getCharacter(uint8_t character,const FontChar*& fc) const {

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
  }
}
