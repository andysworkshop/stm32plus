/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


/*
 * Constructor
 */

TouchManager::TouchManager(LcdManager& lcdManager,FileSystemManager& fsManager)
  : Initialiser(lcdManager),
    _fsManager(fsManager) {
}


/*
 * Initialise the touch screen
 */

bool TouchManager::initialise() {

  int16_t i,j;

  _term.writeString("Initialising touch screen.\n");

  // initialise hardware: the pen IRQ is on PB6

  GpioB<DefaultDigitalInputFeature<6> > pb;
  _penIrqPin=pb[6];

  _exti=new Exti6(EXTI_Mode_Interrupt,EXTI_Trigger_Falling,pb[6]);

  Spi1<>::Parameters params;
  params.spi_baudRatePrescaler=SPI_BaudRatePrescaler_256;
  params.spi_cpol=SPI_CPOL_Low;
  params.spi_cpha=SPI_CPHA_1Edge;

  _spi=new Spi1<>(params);

  // get calibration data if there is any

  if(loadCalibration())
    _needsCalibration=false;
  else {
    _calibration=new PassThroughTouchScreenCalibration;
    _needsCalibration=true;
  }

  // create the touch screen API

  _touchScreen=new ADS7843AsyncTouchScreen(
      *_calibration,
      _passThroughPostProcessor,
      *_spi,
      _penIrqPin,
      *_exti);

  // do the 5 second countdown for recalibration

  _clicked=false;

  _touchScreen->TouchScreenReadyEventSender.insertSubscriber(
      TouchScreenReadyEventSourceSlot::bind(this,&TouchManager::onTouchScreenReady)
    );

  if(!_needsCalibration) {
    for(i=5;i>=0;i--) {

      _term.clearLine();
      _term << "Tap screen to recalibrate: " << i;

      for(j=0;j<100;j++) {
        MillisecondTimer::delay(10);

        if(_clicked) {
          calibrate();
          return true;
        }
      }
    }
    _term << '\n';
  }

  return true;
}


/*
 * Calibrate the screen
 */

bool TouchManager::calibrate() {

  TouchScreenCalibration* newResults;

  // wait for the pen to come up

  while(_touchScreen->isTouched());

  // create the calibrator

  ThreePointTouchScreenCalibrator calibrator(*_touchScreen,*this);

  // important preparation for calibration: we must set the screen to pass through mode
  // so that the calibrator sees raw co-ordinates and not calibrated!

  _touchScreen->setCalibration(_passThroughCalibration);

  // calibrate the screen and get the new results

  if(!calibrator.calibrate(newResults))
    return false;

  _term.clearScreen();

  // set the new results

  if(_calibration!=NULL)
    delete _calibration;

  _calibration=newResults;

  // re-initialise the touch screen with the calibration data

  _touchScreen->setCalibration(*_calibration);
  _needsCalibration=false;

  // save the new results to the file system as /pframe/touchcal.bin

  return saveCalibration();
}


/*
 * Save the calibration data
 */

bool TouchManager::saveCalibration() {

  File *file;
  bool retval;

  _term.writeString("Saving calibration data.\n");

  // delete old file

  _fsManager.getFileSystem().deleteFile("/pframe/touchcal.bin");

  // open the file

  if(!_fsManager.getFileSystem().createFile("/pframe/touchcal.bin")) {
    _term.writeString("Unable to create calibration file.\n");
    return false;
  }
  if(!_fsManager.getFileSystem().openFile("/pframe/touchcal.bin",file)) {
    _term.writeString("Unable to open calibration file.\n");
    return false;
  }

  // attach the file to a stream and serialize the data into the file

  FileOutputStream ostream(*file);
  retval=_calibration->serialise(ostream);

  delete file;
  return retval;
}


/*
 * Load calibration data from /pframe/touchcal.bin
 */

bool TouchManager::loadCalibration() {

  File *file;
  bool retval;

  _term.writeString("Loading calibration data.\n");

  // open the file

  if(!_fsManager.getFileSystem().openFile("/pframe/touchcal.bin",file)) {
    _term.writeString("Calibration data not found.\n");
    return false;
  }

  // attach the file to a stream and deserialize the data into the class

  FileInputStream istream(*file);

  _calibration=new ThreePointTouchScreenCalibration;
  retval=_calibration->deserialise(istream);

  delete file;
  return retval;
}


/*
 * Display prompt for calibrator
 */

void TouchManager::displayPrompt(const char *text) {

  Size size;
  LcdManager::LcdAccess lcd=_lcdManager.getLcd();

  lcd.setBackground(ColourNames::BLACK);
  lcd.setForeground(ColourNames::WHITE);

  lcd.clearScreen();

  // show the prompt at the top center

  size=lcd.measureString(*(lcd.getStreamSelectedFont()),text);

  lcd << Point((lcd.getWidth()/2)-(size.Width/2),0) << text;
}


/*
 * Display a hit point
 */

void TouchManager::displayHitPoint(const Point& pt) {

  int16_t i,j,x,y;

  x=pt.X-1;
  y=pt.Y-1;

  _lcdManager.getLcd().setForeground(ColourNames::RED);

  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
      _lcdManager.getLcd().plotPoint(Point(x+j,y+i));
}


/*
 * Get the panel dimensions
 */

Size TouchManager::getPanelSize() {

  return Size(_lcdManager.getLcd().getWidth(),_lcdManager.getLcd().getHeight());
}


/*
 * Screen clicked
 */

void TouchManager::onTouchScreenReady() {
  _clicked=true;
}
