/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor that announces a new domain suffix
     */

    struct DomainNameAnnouncementEvent : NetEventDescriptor {

      /**
       * Your local domain suffix (\0 terminated string)
       */

      const char *domainName;


      /**
       * Constructor
       */

      DomainNameAnnouncementEvent(const char *domain)
        : NetEventDescriptor(NetEventType::DOMAIN_NAME_ANNOUNCEMENT),
          domainName(domain) {
      }
    };
  }
}
