/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  class ObserverLink;
  class Observer;

  /**
   * @brief base class for classes that raise events
   *
   * Observable base class. Classes that raise events derive from this and use the notify method to
   * tell the observers that something interesting has happened.
   */

  class Observable {

    protected:
      ObserverLink *_first;

    protected:
      ~Observable();

    public:
      Observable();

      ObserverLink *insertObserver(Observer& newObserver);

      ObserverLink *getFirstObserverLink();
      void setFirstObserverLink(ObserverLink* newFirst);
      void notifyObservers(ObservableEvent::E event,void *context=nullptr);
  };

}
