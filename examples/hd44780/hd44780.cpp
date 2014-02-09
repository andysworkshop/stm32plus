/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/display/character.h"


using namespace stm32plus;
using namespace stm32plus::display;


/**
 *
 * HD44780 20x4 character LCD demo. For this demonstration
 * we will attach the character LCD to the following pins:
 *
 * PC[0] => RS
 * PC[1] => ENABLE
 * PC[2] => D0
 * PC[3] => D1
 * PC[4] => D2
 * PC[5] => D3
 *
 * This demonstration attachs a text terminal class to the
 * LCD for convenient text output. We then go into an
 * infinite loop writing out the famous Lorem Ipsum
 * passage, scrolling the display each time the output
 * reaches the end of the display.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */


/**
 * The sample text
 */

static const char *sampleText[]={
  "Lorem","ipsum","dolor","sit","amet,","consectetur","adipisicing","elit,","sed","do","eiusmod","tempor","incididunt","ut","labore","et","dolore","magna","aliqua.","Ut","enim","ad","minim","veniam,","quis","nostrud","exercitation","ullamco","laboris","nisi","ut","aliquip","ex","ea","commodo","consequat.","Duis","aute","irure","dolor","in","reprehenderit","in","voluptate","velit","esse","cillum","dolore","eu","fugiat","nulla","pariatur.","Excepteur","sint","occaecat","cupidatat","non","proident,","sunt","in","culpa","qui","officia","deserunt","mollit","anim","id","est","laborum",NULL
};


class HD44780Test {

  public:

    void run() {

      int i,written,len;

      /*
       * Initialise the 6 required pins for output
       */

      GpioC<DefaultDigitalOutputFeature<0,1,2,3,4,5> > pc;

      /*
       * Initialise the 20x4 display
       */

      HD44780 lcd(pc[0],pc[1],pc[2],pc[3],pc[4],pc[5],20,4);

      /*
       * Attach a terminal to the display so we can easily demonstrate the
       * text output function
       */

      CharacterLcdTerminal<HD44780> terminal(lcd);

      /*
       * Write out the sample text
       */

      terminal.clear();

      for(i=written=0;;) {

        // if the current word plus trailing space would wrap, start a new line

        len=strlen(sampleText[i])+1;
        if(written+len>20) {
          terminal << '\n';
          written=0;
        }

        // write out the current word with the following space

        terminal << sampleText[i] << ' ';
        written+=len;

        // if there is no next word then start again

        if(sampleText[++i]==NULL)
          i=0;

        // delay for a 500ms before the next word

        MillisecondTimer::delay(500);
      }
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  HD44780Test test;
  test.run();

  // not reached
  return 0;
}
