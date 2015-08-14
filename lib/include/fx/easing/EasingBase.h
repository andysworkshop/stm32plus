/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * Base class for easing functions
     */

    template<typename TDataType>
    class EasingBaseT {

      protected:
        TDataType _change;
        TDataType _duration;

      public:
        EasingBaseT();
        virtual ~EasingBaseT() {}

        /**
         * Ease a transition in
         * @param[in] time The time to do the transition.
         */

        virtual TDataType easeIn(TDataType time) const=0;

        /**
         * Ease a transition out.
         * @param[in] time The time to do the transition.
         */

        virtual TDataType easeOut(TDataType time) const=0;

        /**
         * Ease a transition in and out.
         * @param[in] time The time to do the transition.
         */

        virtual TDataType easeInOut(TDataType time) const=0;

        void setDuration(TDataType duration);
        void setTotalChangeInPosition(TDataType totalChangeInPosition);

        TDataType getTotalChangeInPosition() const;
        TDataType getDuration() const;
    };

    /**
     * Compatibility typedef
     */

    typedef EasingBaseT<float> EasingBase;


    /**
     * Default constructor
     */

    template<typename TDataType>
    inline EasingBaseT<TDataType>::EasingBaseT() {
      _change=0;
    }


    /**
     * Set the duration
     * @param[in] duration The duration
     */

    template<typename TDataType>
    inline void EasingBaseT<TDataType>::setDuration(TDataType duration) {
      _duration=duration;
    }


    /**
     * Set the total change in position
     * @param[in] totalChangeInPosition The total change in position.
     */

    template<typename TDataType>
    inline void EasingBaseT<TDataType>::setTotalChangeInPosition(TDataType totalChangeInPosition) {
      _change=totalChangeInPosition;
    }


    /**
     * Get the total change in position
     * @return the total change in position
     */

    template<typename TDataType>
    inline TDataType EasingBaseT<TDataType>::getTotalChangeInPosition() const {
      return _change;
    }


    /**
     * Get the duration
     * @return the duration
     */

    template<typename TDataType>
    inline TDataType EasingBaseT<TDataType>::getDuration() const {
      return _duration;
    }
  }
}
