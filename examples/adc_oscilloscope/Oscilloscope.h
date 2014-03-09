/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * This is the main application class.
 */

class Oscilloscope {

  public:
    volatile bool _halfTime;
    volatile bool _fullTime;

  protected:
    scoped_ptr<Display> _display;
    scoped_ptr<Adc> _adc;

    scoped_array<uint8_t> _channel1;
    scoped_array<uint8_t> _channel2;

  protected:
    void createAdc();
    void createBuffers();

  public:
    void run();
};
