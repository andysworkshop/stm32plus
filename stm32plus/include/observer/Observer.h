/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  class Observable;

  /**
   * @brief Base class for observers.
   *
   * Classes that are consumers of events should derive from this
   * base and implement the two required methods.
   */

  class Observer {

    public:

      virtual ~Observer() {}

      /**
       * Called when an event has been raised, probably from an interrupt so consumers should take
       * care to return quickly.
       *
       * @param[in] sender A reference to the sender of the event.
       *
       * @param[in] event
       *   The type of event being raised.
       *
       * @param[in] context
       *   The context for the event. The type of this pointer is dependent on the event_ parameter.
       *   Implementors of this method will have to cast context_ to what it really is.
       */

      virtual void onNotify(Observable& sender,ObservableEvent::E event,void *context)=0;

      /**
       * Notification that the Observable object has been destroyed. The implementing class must not
       * attempt to call remove() on its ObservableLink object because that object is no longer valid.
       */

      virtual void onSubjectDestroyed() {}
  };
}
