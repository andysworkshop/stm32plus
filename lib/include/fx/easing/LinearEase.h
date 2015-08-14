/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace fx {

    /**
     * @brief Linear ease. Really a dummy as it doesn't ease at all.
     */

    template<class TDataType>
    class LinearEaseT : public EasingBaseT<TDataType> {

      public:
        virtual ~LinearEaseT() {}

        virtual TDataType easeIn(TDataType time) const override;
        virtual TDataType easeOut(TDataType time) const override;
        virtual TDataType easeInOut(TDataType time) const override;
    };


    /**
     * Compatibility typedef
     */

    typedef LinearEaseT<float> LinearEase;


    /**
     * no acceleration
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType LinearEaseT<TDataType>::easeIn(TDataType time) const {
      return this->_change * time / this->_duration;
    }


    /**
     * no acceleration
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType LinearEaseT<TDataType>::easeOut(TDataType time) const {
      return easeIn(time);
    }


    /**
     * no acceleration
     * @param time the current animation time
     * @return the position at the time
     */

    template<class TDataType>
    inline TDataType LinearEaseT<TDataType>::easeInOut(TDataType time) const {
      return easeIn(time);
    }
  }
}
