///
/// Wink Signals
/// Copyright (C) 2012 Anax Creations. All rights reserved.
///
///
/// This software is provided 'as-is', without any express or implied warranty.
/// In no event will the authors be held liable for any damages arising from the
/// use of this software.
///
/// Permission is hereby granted, free of charge, to any person
/// obtaining a copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// 1. The origin of this software must not be misrepresented;
///    you must not claim that you wrote the original software.
///    If you use this software in a product, an acknowledgment
///    in the product documentation would be appreciated but is not required.
///
/// 2. Altered source versions must be plainly marked as such,
///    and must not be misrepresented as being the original software.
///
/// 3. The above copyright notice and this permission notice shall be included in
///    all copies or substantial portions of the Software.
///

/**
 * In accordance with the above license I need to point out that this file is heavily modified
 * to not require the STL. We use a simple linked list instead.
 */

#pragma once


namespace wink {
  template<class Slot>
  struct signal {

    protected:

      typedef Slot slot_type;

      slot_type _firstSlot;
#ifdef EXT_SLIST     
      __gnu_cxx::slist<slot_type> _slots;
#else
      std::slist<slot_type> _slots;
#endif
    public:

      /// Connects a slot to the signal
      /// \param slot The slot you wish to connect
      /// \see bind To bind a slot to a function

      void insertSubscriber(const slot_type& slot) {

        if(_slots.size()==0)
          _firstSlot=slot;

        _slots.push_front(slot);
      }

      /// Disconnects a slot from the signal
      /// \param slot The slot you wish to disconnect
      /// \see bind To bind a slot to a function

      bool removeSubscriber(const slot_type& slot) {

        for(auto it=_slots.begin();it!=_slots.end();it++) {
          if(*it==slot) {
            _slots.erase(it);
            return true;
          }
        }
        return false;
      }

      /// Emits the events you wish to send to the call-backs
      /// \param args The arguments to emit to the slots connected to the signal
      template <class ...Args>
      void raiseEvent(Args&&... args) const {

        if(_slots.size()==1)
          _firstSlot(args...);
        else {
          for(auto it=_slots.begin();it!=_slots.end();it++)
            (*it)(args...);
        }
      }
  };
}
