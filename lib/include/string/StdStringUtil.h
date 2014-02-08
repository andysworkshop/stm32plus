/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/**
 * @file
 * Various string utilities. We provide integer to ASCII and optimised to-ASCII conversions for
 * double-precision and uint32_t values.
 */

namespace stm32plus {
  namespace StdStringUtil {

    static const char *defaultTrimChars="\n\r\t ";

    /**
     * Remove white space (\n\r\t ) from the start of a string
     * @param str the string to left-trim
     */

    inline void trimLeft(std::string& str,const char *trimChars=defaultTrimChars) {

      std::string::size_type pos;

      if((pos=str.find_first_not_of(trimChars))==std::string::npos)
        str.clear();
      else if(pos!=0)
        str=str.substr(pos);
    }


    /**
     * Remove white space (\n\r\t ) from the end of a string
     * @param str the string to right-trim
     */

    inline void trimRight(std::string& str,const char *trimChars=defaultTrimChars) {

      std::string::size_type pos;

      if((pos=str.find_last_not_of(trimChars))==std::string::npos)
        str.clear();
      else if(pos!=str.size()-1)
        str=str.substr(0,pos+1);
    }


    /**
     * Remove white space (\n\r\t ) from both ends of a string
     * @param str the string to fully trim
     */

    inline void trim(std::string& str,const char *trimChars=defaultTrimChars) {
      trimLeft(str,trimChars);
      trimRight(str,trimChars);
    }


    /**
     * Tokenise a string based on supplied separators
     * @param cmdText
     */

    inline void tokenise(const std::string& cmdText,const char *separators,std::vector<std::string>& words) {

      std::string::size_type first,last;

      // tokenise the command

      for(first=cmdText.find_first_not_of(separators);first!=std::string::npos && last!=std::string::npos;) {

        if((last=cmdText.find_first_of(separators,first))==std::string::npos)
          break;

        words.push_back(cmdText.substr(first,last-first));
        first=cmdText.find_first_not_of(separators,last);
      }

      if(first!=std::string::npos)
        words.push_back(cmdText.substr(first,cmdText.length()-first));
    }
  }
}
