/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Recursive initialisation of feature classes defined using variadic templates.
     * This initialiser has parameters and NetworkUtilityObjects class and returns bool
     * Here's the forward definition
     */

    template<typename F,typename... T>
    struct RecursiveBoolInitWithParamsAndNetworkUtilityObjects;

    /**
     * Recursion termination condition, always return success
     */

    template<class F>
    struct RecursiveBoolInitWithParamsAndNetworkUtilityObjects<F> {
      static bool tinit(F *,typename F::Parameters&,NetworkUtilityObjects&) {
        return true;
      }
    };


    /**
     * General recursive initialiser
     */

    template<class F,class T,class... G>
    struct RecursiveBoolInitWithParamsAndNetworkUtilityObjects<F,T,G...> {
      static bool tinit(F *ptr,typename F::Parameters& p,NetworkUtilityObjects& netUtils) {

        if(!ptr->T::initialise(p,netUtils))
          return false;

        return RecursiveBoolInitWithParamsAndNetworkUtilityObjects<F,G...>::tinit(ptr,p,netUtils);
      }
    };


    /**
     * Recursive initialisation of physical layer feature classes defined using variadic
     * templates. This initialiser has parameters, NetworkUtilityObjects and a PHY class
     * and returns bool. Here's the forward definition
     */

    template<class F,class TPhy,class... T>
    struct RecursivePhysicalLayerInit;

    /**
     * Recursion termination condition, always return success
     */

    template<class F,class TPhy>
    struct RecursivePhysicalLayerInit<F,TPhy> {
      static bool tinit(F *,typename F::Parameters&,NetworkUtilityObjects&,TPhy&) {
        return true;
      }
    };


    /**
     * General recursive initialiser
     */

    template<class F,class TPhy,class T,class... G>
    struct RecursivePhysicalLayerInit<F,TPhy,T,G...> {
      static bool tinit(F *ptr,typename F::Parameters& p,TPhy& phy,NetworkUtilityObjects& netUtils) {

        if(!ptr->T::initialise(p,netUtils,phy))
          return false;

        return RecursivePhysicalLayerInit<F,TPhy,G...>::tinit(ptr,p,netUtils,phy);
      }
    };
  }
}
