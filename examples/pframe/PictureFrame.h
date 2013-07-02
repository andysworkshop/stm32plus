/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


/*
 * Main application class
 */

class PictureFrame {

  protected:
    LcdManager *_lcdManager;
    FileSystemManager *_fsManager;
    TouchManager *_touchManager;
    BitmapManager *_bitmapManager;
    ImageManager *_imageManager;
    ThumbManager *_thumbManager;
    uint32_t _autoScrollMillis;
    bool _autoScrollEnabled;

  protected:
    bool initialise();
    void processEvents();
    bool processClick();

  public:
    void run();
};

