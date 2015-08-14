/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * @brief Back easing function. Goes back on itself.
     */

    template<class TDataType>
    class BackEaseT : public EasingBaseT<TDataType> {

      protected:
        TDataType _overshoot;

      public:
        BackEaseT();
        BackEaseT(TDataType overshoot);
        virtual ~BackEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;

        void setOvershoot(TDataType overshoot);
        TDataType getOvershoot() const;
    };


    /**
     * Constructor - sets a default value for the overshoot
     */

    template<class TDataType>
    inline BackEaseT<TDataType>::BackEaseT() {
      _overshoot=1.70158;
    }


    /**
     * Constructor with parameter
     * @param overshoot
     */

    template<class TDataType>
    inline BackEaseT<TDataType>::BackEaseT(TDataType overshoot)
      : _overshoot(overshoot) {
    }


    /**
     * Set the overshoot value
     * @param overshoot the new value
     */

    template<class TDataType>
    inline void BackEaseT<TDataType>::setOvershoot(TDataType overshoot) {
      _overshoot=overshoot;
    }


    /**
     * Get the overshoot value
     * @return the value
     */

    template<class TDataType>
    inline TDataType BackEaseT<TDataType>::getOvershoot() const {
      return _overshoot;
    }


    /**
     * Starts the motion by backtracking, then reversing direction and moving toward the target
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType BackEaseT<TDataType>::easeIn(TDataType time) const {
      time/=this->_duration;
      return this->_change * time * time * ((_overshoot + 1) * time - _overshoot);
    }


    /**
     * Starts the motion by moving towards the target, overshooting
     * it slightly, and then reversing direction back toward the target
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType BackEaseT<TDataType>::easeOut(TDataType time) const {
      time=time / this->_duration - 1;
      return this->_change * (time * time * ((_overshoot + 1) * time + _overshoot) + 1);
    }


    /**
     * Combines the motion of the easeIn and easeOut methods to
     * start the motion by backtracking, then reversing direction
     * and moving toward target, overshooting target slightly,
     * reversing direction again, and then moving back toward the target
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType BackEaseT<TDataType>::easeInOut(TDataType time) const {
      TDataType overshoot;

      overshoot=_overshoot * 1.525;
      time/=this->_duration / 2;

      if(time < 1)
        return this->_change / 2 * (time * time * ((overshoot + 1) * time - overshoot));

      time-=2;
      return this->_change / 2 * (time * time * ((overshoot + 1) * time + overshoot) + 2);
    }
  }
}
