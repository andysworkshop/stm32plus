/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Thumbnail manager class
 */

class ThumbManager {

  protected:
    LcdManager::LcdAccess& _gl;
    FileSystemManager& _fsManager;
    TouchManager& _touchManager;
    Rectangle _hotSpots[8];
    Rectangle _scrollbar;
    File *_progressBarFile;
    File *_progressKnobFile;
    File *_timeIcons[7];
    uint8_t _timeMap[7];

    int16_t _currentTimeIndex;
    int16_t _thumbx;
    int32_t _first;
    int32_t _last;
    int32_t _imageCount;

  protected:
    void drawBackground() const;
    void drawThumb(uint32_t thumbIndex) const;
    void redraw();
    void drawThumbs() const;
    bool openThumb(uint32_t thumbIndex,File *& file) const;
    bool clickedThumb(const Point& p,uint32_t& nextImage) const;
    bool clickedScrollbar(const Point& p);
    bool clickedOutside(const Point& p) const;
    void drawScrollbar();
    bool clickedKnob(const Point& p);
    void drawBitmapFile(const char *filename,int16_t x,int16_t y,int16_t w,int16_t h) const;
    void drawControls();
    void drawTime();
    bool clickedUpDown(const Point& p);
    bool clickedStop(const Point& p);
    bool clickedPlay(const Point& p);

    static const int32_t THUMBS_PER_PAGE = 8;

    static const int16_t THUMB_WIDTH = 44;
    static const int16_t THUMB_HEIGHT = 58;

    static const int16_t SCROLLBAR_WIDTH = 188;
    static const int16_t SCROLLBAR_HEIGHT = 18;

    static const int16_t SCROLLBAR_KNOB_WIDTH = 8;
    static const int16_t SCROLLBAR_KNOB_HEIGHT = 18;

  public:
    ThumbManager(LcdManager::LcdAccess& gl,FileSystemManager& fsManager,TouchManager& touchManager,uint32_t imageCount);

    bool run(uint32_t& nextImage,uint32_t& autoScrollMillis,bool& autoScrollEnabled);
};


