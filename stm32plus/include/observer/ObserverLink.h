/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  class Observer;

  /**
   * @brief The linked list node that holds an observer.
   *
   * The observers are held in a linked list. This class manages the link and the observer held in this link.
   */

  class ObserverLink {

    protected:
      Observable& _base;
      Observer& _observer;

      ObserverLink* _next;
      ObserverLink* _previous;

    public:
      ObserverLink(Observable& base_,Observer& observer);

      void destroy();
      void remove();
      void setNext(ObserverLink* newNext);
      void setPrevious(ObserverLink* newPrevious);
      ObserverLink* getNext();
      Observer& getObserver();
  };
}
