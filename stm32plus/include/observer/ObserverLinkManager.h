/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * @brief class to manage the lifecycle of an ObserverLink.
   *
   * Deregisters when destructed as long as the observed class has not been destroyed
   */

  class ObserverLinkManager  {
    protected:
      ObserverLink *_link;

    public:

      /**
       * Default constructor, set the managed link to nullptr
       */

      ObserverLinkManager() {
        _link=nullptr;
      }

      /**
       * Destructor, deregister
       */

      ~ObserverLinkManager() {
        if(_link)
          _link->remove();
      }

      /**
       * Set the managed link
       * @param[in] link The link to manage
       */

      void setLink(ObserverLink *link) {
        _link=link;
      }


      /**
       * Subject has been destroyed, clear the managed link
       */

      void onSubjectDestroyed() {
        _link=nullptr;
      }
  };
}
