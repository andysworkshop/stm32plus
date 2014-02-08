/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/string.h"


namespace stm32plus {

  /**
   * Constructor.
   * @param[in] original The original string. This string is not modified.
   * @param[in] separators The token separators.
   */

  TokenisedString::TokenisedString(const char *original,const char *separators) {

    char *ptr,*current;
    int i;

  // count tokens

    _numTokens=0;
    _first=0;
    _last=-1;
    _tokens=nullptr;

    if(original==nullptr)
      return;

    ptr=strdup(original);
    current=strtok(ptr,separators);

    while(current!=nullptr) {
      _numTokens++;
      current=strtok(nullptr,separators);
    }
    free(ptr);

  // anything?

    if(_numTokens==0)
      return;

  // allocate

    _tokens=new char *[_numTokens];

  // parse again

    ptr=strdup(original);
    current=strtok(ptr,separators);
    i=0;

    while(current!=nullptr) {
      _tokens[i++]=strdup(current);
      current=strtok(nullptr,separators);
    }
    free(ptr);

    _last=_numTokens-1;
  }


  /**
   * Destructor. Free memory used by tokens.
   */

  TokenisedString::~TokenisedString() {

    int i;

    if(_numTokens>0) {
      for(i=0;i<_numTokens;i++)
        free(_tokens[i]);

      delete [] _tokens;
    }
  }

  /**
   * Get the number of tokens, taking into account the range if set.
   * @return The number of tokens.
   */

  int TokenisedString::getNumTokens() const {
    return (_last-_first)+1;
  }


  /**
   * Get a token, taking into account the range if set.
   * @param[in] pos The token index to get.
   * @return An internal pointer to the token.
   */

  const char *TokenisedString::operator[](int pos) const {
    return _tokens[_first+pos];
  }


  /**
   * Return the last entry.
   * @return The last token.
   */

  const char *TokenisedString::last() const {
    return _tokens[_last];
  }


  /**
   * Limit the range of tokens returned so that the caller can only see a subset of all the tokens.
   * @param[in] first The first token in the range.
   * @param[in] last The last token in the range.
   */

  void TokenisedString::setRange(int first,int last) {
    _first=first;
    _last=last;
  }


  /**
   * Clear the range parameters.
   */

  void TokenisedString::resetRange() {
    _first=0;
    _last=_numTokens-1;
  }
}
