/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Touch screen manager
 */

class TouchManager : public Initialiser,
                     public ThreePointTouchScreenCalibrator::GuiCallback {

  protected:
    FileSystemManager& _fsManager;

    TouchScreen *_touchScreen;
    Spi *_spi;
    ExtiPeripheralBase *_exti;
    GpioPinRef _penIrqPin;
    PassThroughTouchScreenPostProcessor _passThroughPostProcessor;
    PassThroughTouchScreenCalibration _passThroughCalibration;
    TouchScreenCalibration *_calibration;
    bool _needsCalibration;

    volatile bool _clicked;

  protected:
    bool loadCalibration();
    bool saveCalibration();

  public:
    TouchManager(LcdManager& lcdManager,FileSystemManager& fsManager);
    virtual ~TouchManager() {}

    TouchScreen& getTouchScreen() const {
      return *_touchScreen;
    }

    bool needsCalibration() const {
      return _needsCalibration;
    }

    void waitForPenUp() {
      while(isPenDown());
      MillisecondTimer::delay(100);
      _clicked=false;
    }

    bool isPenDown() const {
      return !_penIrqPin.read();
    }

    bool clicked() const {
      return _clicked;
    }

    bool calibrate();

    // overrides from Initialiser

    virtual bool initialise();

    // overrides from ThreePointTouchScreenCalibrator::GuiCallback

    virtual void displayPrompt(const char *text);
    virtual void displayHitPoint(const Point& pt);
    virtual Size getPanelSize();

    // touch screen ready event

    void onTouchScreenReady();
};
