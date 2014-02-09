/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

ThumbManager::ThumbManager(LcdManager::LcdAccess& gl,FileSystemManager& fsManager,TouchManager& touchManager,uint32_t imageCount)
  : _gl(gl),
    _fsManager(fsManager),
    _touchManager(touchManager) {

  int16_t x,y,i;
  char buffer[30];

  _imageCount=imageCount;

  // store the image count

  _first=0;
  _last=_imageCount>THUMBS_PER_PAGE ? THUMBS_PER_PAGE-1 : _imageCount-1;

  // create the hot spots

  x=26;
  y=64;

  for(i=0;i<THUMBS_PER_PAGE/2;i++) {
    _hotSpots[i].X=x;
    _hotSpots[i].Y=y;
    _hotSpots[i].Width=THUMB_WIDTH;
    _hotSpots[i].Height=THUMB_HEIGHT;

    x+=48;
  }

  x=26;
  y=64+62;

  for(i=THUMBS_PER_PAGE/2;i<THUMBS_PER_PAGE;i++) {
    _hotSpots[i].X=x;
    _hotSpots[i].Y=y;
    _hotSpots[i].Width=THUMB_WIDTH;
    _hotSpots[i].Height=THUMB_HEIGHT;

    x+=48;
  }

  // open the progress files

  FileSystem& fs=_fsManager.getFileSystem();
  fs.openFile("/pframe/controls/progressBar.262",_progressBarFile);
  fs.openFile("/pframe/controls/progressKnob.262",_progressKnobFile);

  // open the time icons

  _timeMap[0]=5;
  _timeMap[1]=10;
  _timeMap[2]=15;
  _timeMap[3]=20;
  _timeMap[4]=30;
  _timeMap[5]=40;
  _timeMap[6]=60;

  for(i=0;i<7;i++) {

    strcpy(buffer,"/pframe/controls/");
    StringUtil::itoa(_timeMap[i],buffer+17,10);
    strcat(buffer,".262");

    fs.openFile(buffer,_timeIcons[i]);
  }
}


/*
 * Draw all the thumbnails
 */

void ThumbManager::redraw() {

  drawBackground();
  drawThumbs();
  drawScrollbar();
  drawTime();
  drawControls();
}


/*
 * Draw the time indicator
 */

void ThumbManager::drawTime() {

  FileInputStream fis(*_timeIcons[_currentTimeIndex]);
  fis.reset();

  _gl.drawBitmap(Rectangle(26+(32+4)*3,224,32,32),fis);
}


/*
 * Draw the additional controls
 */

void ThumbManager::drawControls() {

  drawBitmapFile("/pframe/controls/stop.262",26,224,32,32);
  drawBitmapFile("/pframe/controls/play.262",26+32+4,224,32,32);
  drawBitmapFile("/pframe/controls/updown.262",26+(32+4)*4,224,15,32);
}


/*
 * Draw a bitmap file
 */

void ThumbManager::drawBitmapFile(const char *filename,int16_t x,int16_t y,int16_t w,int16_t h) const {

  File *file;

  if(!_fsManager.getFileSystem().openFile(filename,file))
    return;

  FileInputStream fis(*file);
  _gl.drawBitmap(Rectangle(x,y,w,h),fis);
}


/*
 * Draw the scrollbar
 */

void ThumbManager::drawScrollbar() {

  // prepare input streams

  _progressBarFile->seek(0,File::SeekStart);
  FileInputStream progressBarStream(*_progressBarFile);

  _progressKnobFile->seek(0,File::SeekStart);
  FileInputStream progressKnobStream(*_progressKnobFile);

  // draw container

  _scrollbar.X=26;
  _scrollbar.Y=60+4+58+4+58+4;
  _scrollbar.Width=SCROLLBAR_WIDTH;
  _scrollbar.Height=SCROLLBAR_HEIGHT;

  _gl.drawBitmap(Rectangle(_scrollbar.X,_scrollbar.Y,_scrollbar.Width,_scrollbar.Height),progressBarStream);

  // use this rectangle for hit testing so move the valid x-range past the curved ends

  _scrollbar.X+=5;
  _scrollbar.Width-=10;

  // cannot scroll

  if(_imageCount<=THUMBS_PER_PAGE)
    return;

  // draw the thumb

  _thumbx=26+5+((168*_first)/(_imageCount-THUMBS_PER_PAGE));
  _gl.drawBitmap(Rectangle(_thumbx,60+4+58+4+58+4,SCROLLBAR_KNOB_WIDTH,SCROLLBAR_KNOB_HEIGHT),progressKnobStream);
}


/*
 * Draw the background
 */

void ThumbManager::drawBackground() const {

  Rectangle rc;

  rc.X=20;
  rc.Y=58;
  rc.Width=202;
  rc.Height=152;

  _gl.setForeground(0x384349);
  _gl.fillRectangle(rc);

  rc.Y=220;
  rc.Height=40;
  _gl.fillRectangle(rc);
}


/*
 * Draw all thumbs
 */

void ThumbManager::drawThumbs() const {

  int32_t i;

  for(i=_first;i<=_last;i++)
    drawThumb(i);
}


/*
 * Draw the thumbnail given by the index
 */

void ThumbManager::drawThumb(uint32_t thumbIndex) const {

  File *file;
  int16_t x,y,row,col;

  // open the thumb

  if(!openThumb(thumbIndex,file))
    return;

  // get a stream on to it

  FileInputStream istream(*file);

  // draw it

  row=thumbIndex-_first<4 ? 0 : 1;
  col=(thumbIndex-_first) & 3;

  x=(col*49)+26;
  y=(row*62)+64;

  _gl.drawBitmap(Rectangle(x,y,THUMB_WIDTH,THUMB_HEIGHT),istream);

  // done

  delete file;
}


/*
 * Open a thumb file
 */

bool ThumbManager::openThumb(uint32_t thumbIndex,File *& file) const {

  char buffer[30];

  strcpy(buffer,"/pframe/thumb/");
  StringUtil::itoa(thumbIndex,buffer+14,10);
  strcat(buffer,".262");

  return _fsManager.getFileSystem().openFile(buffer,file);
}


/*
 * Run the user interface. Return true if move to next image and nextImage will be set.
 * Return false if clicked outside interface
 */

bool ThumbManager::run(uint32_t& nextImage,uint32_t& autoScrollMillis,bool& autoScrollEnabled) {

  Point p;
  int16_t i;

  // remember where we are in the sequence

  for(i=0;i<7;i++) {
    if(_timeMap[i]==autoScrollMillis/1000) {
      _currentTimeIndex=i;
      break;
    }
  }

  // redraw all

  redraw();

  // wait for touches

  for(;;) {

    _touchManager.waitForPenUp();

    while(!_touchManager.clicked());

    // get the co-ordinates

    if(!_touchManager.getTouchScreen().getCoordinates(p))
      continue;

    if(clickedThumb(p,nextImage))
      return true;

    if(clickedKnob(p))
      continue;

    if(clickedScrollbar(p))
      continue;

    if(clickedUpDown(p)) {
      autoScrollMillis=static_cast<uint32_t>(_timeMap[_currentTimeIndex])*1000;
      continue;
    }

    if(clickedStop(p)) {
      _touchManager.waitForPenUp();
      autoScrollEnabled=false;
      return false;
    }

    if(clickedPlay(p)) {
      _touchManager.waitForPenUp();
      autoScrollEnabled=true;
      return false;
    }

    if(clickedOutside(p))
      return false;
  }
}


/*
 * Check if stop clicked
 */

bool ThumbManager::clickedStop(const Point& p) {

  return p.X>=26 && p.X<26+32 && p.Y>=224 && p.Y<=224+32;
}


/*
 * Check if play clicked
 */

bool ThumbManager::clickedPlay(const Point& p) {

  return p.X>=26+32+4 && p.X<26+32+4+32 && p.Y>=224 && p.Y<=224+32;
}


/*
 * Check if clicked up/down button
 */

bool ThumbManager::clickedUpDown(const Point& p) {

  if(p.X<26+(32+4)*4 || p.X>15+26+(32+4)*4 || p.Y<224 || p.Y>224+32)
    return false;

  if(p.Y<224+16)
    _currentTimeIndex++;
  else
    _currentTimeIndex--;

  if(_currentTimeIndex<0)
    _currentTimeIndex=0;
  else if(_currentTimeIndex>6)
    _currentTimeIndex=6;

  drawTime();
  _touchManager.waitForPenUp();

  return true;
}


/*
 * Have we clicked the knob?
 */

bool ThumbManager::clickedKnob(const Point& p) {

  int16_t xoffset,newThumbx;
  int32_t newfirst;
  Point newPoint;

  // scrolling disabled?

  if(_imageCount<THUMBS_PER_PAGE)
    return false;

  // must be in the knob

  if(p.X<_thumbx-4 || p.X>=_thumbx+SCROLLBAR_KNOB_WIDTH+4 || p.Y<_scrollbar.Y || p.Y>_scrollbar.Y+SCROLLBAR_HEIGHT)
    return false;

  // to avoid an initial jump, remember the offset from the left of the knob

  xoffset=p.X-_thumbx;

  // carry on while the pen is down

  while(_touchManager.isPenDown()) {

    // get touch co-ordinates

    if(!_touchManager.getTouchScreen().getCoordinates(newPoint))
      break;

    newThumbx=newPoint.X-xoffset;
    if(newThumbx<26+5)
      newThumbx=26+5;
    else if(newThumbx>26+5+168)
      newThumbx=26+5+168;

    // calc the new first position and validate it

    newfirst=((newThumbx-26-5)*(_imageCount-THUMBS_PER_PAGE))/168;

    if(newfirst<0)
      newfirst=0;
    else if(newfirst>_imageCount-THUMBS_PER_PAGE)
      newfirst=_imageCount-THUMBS_PER_PAGE;

    // if it's new then move

    if(newfirst!=_first) {
      _first=newfirst;
      _last=_first+THUMBS_PER_PAGE-1;

      drawScrollbar();
      drawThumbs();
    }
  }

  _touchManager.waitForPenUp();
  return true;
}


/*
 * check if clicked scrollbar and action it
 */

bool ThumbManager::clickedScrollbar(const Point& p) {

  if(_imageCount<THUMBS_PER_PAGE || !_scrollbar.containsPoint(p))
    return false;

  // left of the knob?

  if(p.X<_thumbx)
    _first-=THUMBS_PER_PAGE;
  else if(p.X>_thumbx+SCROLLBAR_KNOB_WIDTH)
    _first+=THUMBS_PER_PAGE;

  // range check

  if(_first<0)
    _first=0;
  else if(_first>_imageCount-THUMBS_PER_PAGE)
    _first=_imageCount-THUMBS_PER_PAGE;

  _last=_first+THUMBS_PER_PAGE-1;

  // redraw the thumbs and scrollbar

  drawThumbs();
  drawScrollbar();

  _touchManager.waitForPenUp();
  return true;
}


/*
 * Have we clicked outside the area
 */

bool ThumbManager::clickedOutside(const Point& p) const {

  return p.X<20 || p.X>220 || p.Y<60 || p.Y>260;
}


/*
 * Have we clicked a thumb?
 */

bool ThumbManager::clickedThumb(const Point& p,uint32_t& nextImage) const {

  int32_t i;

  for(i=_first;i<=_last;i++) {
    if(_hotSpots[i-_first].containsPoint(p)) {
      nextImage=i;
      return true;
    }
  }

  return false;
}
