/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"
#include "config/display/touch.h"


namespace stm32plus {
  namespace display {


    /**
     * Constructor, set up the parameters for the calibration
     * @param[in] gui Implementation of the GuiCallback interface
     * @param[in] ts The touch screen to interact with.
     */

    ThreePointTouchScreenCalibrator::ThreePointTouchScreenCalibrator(TouchScreen& ts,GuiCallback& gui)
      : _gui(gui),
        _ts(ts) {
    }


  /**
   * Interactively calibrate the touch screen with the user. This routine will present a UI to the user for calibration
   * and block until it's done. The resulting calibration class will be returned in the calibrationResults parameter and
   * can be serialised to an output device for restoration in future sessions.
   *
   * @param[out] calibrationResults The results of the calibration. Caller is responsible for deleting the pointer.
   * @return false if it fails
   */

    bool ThreePointTouchScreenCalibrator::calibrate(TouchScreenCalibration*& calibrationResults) {

      Point p1Panel,p2Panel,p3Panel,p1Touch,p2Touch,p3Touch;

      // register as an event subscriber with the touch screen

      _clicked=true;

      _ts.TouchScreenReadyEventSender.insertSubscriber(TouchScreenReadyEventSourceSlot::bind(this,&ThreePointTouchScreenCalibrator::onTouchScreenReady));

      // point 1 is at 25%,50%, 2 is at 75%,25% and 3 is at 75%,75%

      getUserInput(25,50,p1Panel,p1Touch);
      getUserInput(75,25,p2Panel,p2Touch);
      getUserInput(75,75,p3Panel,p3Touch);

      // deregister our interest in clicks

      _ts.TouchScreenReadyEventSender.removeSubscriber(TouchScreenReadyEventSourceSlot::bind(this,&ThreePointTouchScreenCalibrator::onTouchScreenReady));

      // given the three points, expected and raw, we can do the mapping

      calibrationResults=new ThreePointTouchScreenCalibration(p1Panel,p2Panel,p3Panel,p1Touch,p2Touch,p3Touch);
      return true;
    }


    /*
     * Display a point for the user to click on and wait for that to happen
     */

    void ThreePointTouchScreenCalibrator::getUserInput(
        int xpercent,
        int ypercent,
        Point& panelPoint,
        Point& touchPoint) {

      Point p;
      int32_t xsum,ysum;
      int i;
      Size panelSize;

      // convert the percent co-ords into actual panel locations

      panelSize=_gui.getPanelSize();

      panelPoint.X=(panelSize.Width*xpercent)/100;
      panelPoint.Y=(panelSize.Height*ypercent)/100;

      for(int i=0;i<50000;i++);

      // display the hit-point for the user

      _gui.displayPrompt("Tap and hold the red point");
      _gui.displayHitPoint(panelPoint);

      // wait for a click to happen via the callback in the observer

      for(_clicked=false;!_clicked;);

      // get sampling 50 points

      xsum=0;
      ysum=0;

      for(i=0;i<50;i++) {
        _ts.getCoordinates(p);

        xsum+=p.X;
        ysum+=p.Y;
      }

      // remove the hit point

      _gui.displayPrompt("Stop pressing the screen");

      // wait to allow for debouncing

      MillisecondTimer::delay(3000);

      touchPoint.X=xsum/50;
      touchPoint.Y=ysum/50;
    }


    /*
     * Observer notification that something has happened
     */

    void ThreePointTouchScreenCalibrator::onTouchScreenReady() {
      _clicked=true;
    }
  }
}
