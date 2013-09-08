#pragma once

// ARMV6 , ARMV7 Atomic primitives
// Based on knowledge
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0489b/Cihbghef.html
//
// Autor Artur Bac 2010 EBASoft
//
// Under BSD License
//
// version 1.0.0
// Written for Bada & GCC C++ but will also work on any other platform with Cortex CPU like IPhone, Android
// ARMv6 introduces a new mechanism, known as "exclusives",
// using the LDREX and STREX instructions.
// Direct use of these instructions is not
// recommended for new code (see "GCC atomic primitives" for a better alternative).
// However, to assist with understanding existing code, a quick overview follows:
//      LDREX   r0, [r1]
//            // do something with r0
//           // no other load or store operations can occur between a LDREX and its corresponding STREX
//       STREX   r2, r0, [r1]
//            // now r2 = 0 if the new value was stored
//            // r2 = 1 if the store was abandoned
//
//Because the STREX is allowed to "fail", it is no longer necessary to lock out other bus activity,
// or stop other threads from executing (whether on the same CPU or on another CPU in a multiprocessor system);
// atomic use in a thread can preempt a pending atomic in another thread.
// For this reason, these primitives are usually also placed in a loop.
//Note that extra load and store instruction (of any kind) between LDREX and STREX can cause the STREX always to fail in some implementations,
//which is why you shouldn't access memory between the LDREX and the corresponding STREX,
//except in cases where it's sensible to abandon the operation such as context switches or exceptions.

//These builtins perform an atomic compare and swap.
//That is, if the current value of *ptr is oldval, then write newval into *ptr.
//The val version returns the contents of *ptr before the operation.

namespace stm32plus {


  /**
   * compare-and-swap internal definitions
   */

  namespace __atomic_internal {

    template<typename T, typename U, size_t sz>
    struct sync_bool_compare_and_swap_internal {
    };

    template<typename T, typename U>
    struct sync_bool_compare_and_swap_internal<T, U, sizeof(long)> {
      bool operator()(T *ptr, U oldval, U newval) const {
        register int result;
        asm volatile (
            "ldrex    r0, [%1]         \n\t" /*exclusive load of ptr */
            "cmp      r0,  %2          \n\t" /*compare the oldval ==  *ptr */
            "ite eq\n\t"
            "strexeq  %0,  %3, [%1]\n\t" /*store if eq, strex+eq*/
            "clrexne"
            : "=&r" (result)
            : "r"(ptr), "r"(oldval),"r"(newval)
            : "r0"
        );
        return result == 0;
      }
    };

    template<typename T, typename U>
    struct sync_bool_compare_and_swap_internal<T, U, sizeof(short)> {
      bool operator()(T *ptr, U oldval, U newval) const {
        register int result;
        asm volatile (
            "ldrexh   r1, [%1]         \n\t" /*exclusive load of ptr*/
            "cmp      r1,  %2          \n\t"/*compare the low reg oldval == low *ptr*/
            "ite eq\n\t"
            "strexheq %0,  %3, [%1]\n\t" /*store if eq, strex+eq*/
            "clrexne"
            : "=&r" (result)
            : "r"(ptr), "r"(oldval),"r"(newval)
            : "r1"
        );
        return result == 0;
      }
    };

    template<typename T, typename U>
    struct sync_bool_compare_and_swap_internal<T, U, sizeof(char)> {
      bool operator()(T *ptr, U oldval, U newval) const {
        register int result;
        asm volatile (
            "ldrexb   r1, [%1]         \n\t" /*exclusive load of ptr*/
            "cmp      r1,  %2          \n\t"/*compare the low reg oldval == low *ptr*/
            "ite eq\n\t"
            "strexbeq %0,  %3, [%1]" /*store if eq, strex+eq*/
            "clrexne"
            : "=&r" (result)
            : "r"(ptr), "r"(oldval),"r"(newval)
            : "r1"
        );
        return result == 0;
      }
    };
  }

  /*
   * Compare-and-swap API
   */

  template<typename T, typename U>
  bool sync_bool_compare_and_swap(T *ptr, U oldval, U newval) {
    return __atomic_internal::sync_bool_compare_and_swap_internal<T, U, sizeof(T)>()(ptr, oldval, newval);
  }


  /*
   * Test-and-set internal definitions
   */

  namespace __atomic_internal {

    template<typename T, typename U, size_t sz>
    struct sync_lock_test_and_set {
    };


    template<typename T, typename U>
    struct sync_lock_test_and_set<T, U, sizeof(long)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm (
            "1: ldrex   %0,  [%1]   \n\t"
            "strex   r4,   %2, [%1]   \n\t"
            "cmp     r4,   #0   \n\t"
            "bne     1b     \n\t "
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r4"
        );
        return result;
      }
    };

    template<typename T, typename U>
    struct sync_lock_test_and_set<T, U, sizeof(short)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm (
            "1: ldrexh   %0,  [%1]    \n\t"
            "strexh   r4,   %2, [%1]    \n\t"
            "cmp     r4,   #0   \n\t"
            "bne     1b     \n\t "
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r4"
        );
        return result;
      }
    };


    template<typename T, typename U>
    struct sync_lock_test_and_set<T, U, sizeof(char)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm (
            "1: ldrexb   %0,  [%1]\n\t"
            "strexb   r4,   %2, [%1]\n\t"
            "cmp     r4,   #0\n\t"
            "bne     1b \n\t "
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r4"
        );
        return result;
      }
    };
  }

  /*
   * Test-and-set API
   */

  template<typename T, typename U>
  inline T sync_lock_test_and_set(T *ptr, U value) {
    return __atomic_internal::sync_lock_test_and_set<T, U, sizeof(T)>()(ptr,value);
  }


  /*
   * fetch-and-add internal definitions
   */

  namespace __atomic_internal {

    template<typename T, typename U, size_t sz>
    struct sync_fetch_and_add {
    };

    template<typename T, typename U>
    struct sync_fetch_and_add<T, U, sizeof(long)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1: ldrex   %0,  [%1] \n\t"
            "add     r1,   %0,  %2  \n\t"
            "strex   r2,   r1, [%1] \n\t"
            "cmp     r2,   #0 \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1","r2"
        );
        return result;
      }
    };

    template<typename T, typename U>
    struct sync_fetch_and_add<T, U, sizeof(short)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1: ldrexh  %0,  [%1] \n\t"
            "add     r1,   %0,  %2  \n\t"
            "strexh  r2,   r1, [%1] \n\t"
            "cmp     r2,   #0 \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1","r2"
        );
        return result;
      }
    };

    template<typename T, typename U>
    struct sync_fetch_and_add<T, U, sizeof(char)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1: ldrexb  %0,  [%1] \n\t"
            "add     r1,   %0,  %2  \n\t"
            "strexb  r2,   r1, [%1] \n\t"
            "cmp     r2,   #0 \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1","r2"
        );
        return result;
      }
    };
  }

  /*
   * Fetch-and-add API
   */

  template<typename T, typename U>
  inline T sync_fetch_and_add(T *ptr, U value) {
    return __atomic_internal::sync_fetch_and_add<T, U, sizeof(T)>()(ptr, value);
  }


  /*
   * add-and-fetch internal definitions
   */


  namespace __atomic_internal {

    template<typename T, typename U, size_t sz>
    struct sync_add_and_fetch {
    };

    template<typename T, typename U>
    struct sync_add_and_fetch<T, U, sizeof(long)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1:   ldrex   %0,  [%1]     \n\t"
            "add     %0,   %0,  %2    \n\t"
            "strex   r1,   %0, [%1] \n\t"
            "cmp     r1,   #0     \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1"
        );
        return result;
      }
    };

    template<typename T, typename U>
    struct sync_add_and_fetch<T, U, sizeof(short)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1:   ldrexh  %0,  [%1]     \n\t"
            "add     %0,   %0,  %2    \n\t"
            "strexh  r1,   %0, [%1] \n\t"
            "cmp     r1,   #0     \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1"
        );
        return result;
      }
    };

    template<typename T, typename U>
    struct sync_add_and_fetch<T, U, sizeof(char)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1:   ldrexb  %0,  [%1]     \n\t"
            "add     %0,   %0,  %2    \n\t"
            "strexb  r1,   %0, [%1] \n\t"
            "cmp     r1,   #0     \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1"
        );
        return result;
      }
    };
  }

  /*
   * add-and-fetch API
   */

  template<typename T, typename U>
  inline T sync_add_and_fetch(T *ptr, U value) {
    return __atomic_internal::sync_add_and_fetch<T, U, sizeof(T)>()(ptr, value);
  }


  /*
   * Fetch and sub internal definitions
   */

  namespace __atomic_internal {

    template<typename T, typename U, size_t sz>
    struct sync_fetch_and_sub {
    };

    template<typename T, typename U>
    struct sync_fetch_and_sub<T, U, sizeof(long)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1: ldrex   %0,  [%1] \n\t"
            "sub     r1,   %0,  %2  \n\t"
            "strex   r2,   r1, [%1] \n\t"
            "cmp     r2,   #0 \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1","r2"
        );
        return result;
      }
    };

    template<typename T, typename U>
    struct sync_fetch_and_sub<T, U, sizeof(short)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1: ldrexh  %0,  [%1] \n\t"
            "sub     r1,   %0,  %2  \n\t"
            "strexh  r2,   r1, [%1] \n\t"
            "cmp     r2,   #0 \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1","r2"
        );
        return result;
      }
    };

    template<typename T, typename U>
    struct sync_fetch_and_sub<T, U, sizeof(char)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1: ldrexb  %0,  [%1] \n\t"
            "sub     r1,   %0,  %2  \n\t"
            "strexb  r2,   r1, [%1] \n\t"
            "cmp     r2,   #0 \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1","r2"
        );
        return result;
      }
    };
  }

  /*
   * fetch-and-sub API
   */

  template<typename T, typename U>
  inline T sync_fetch_and_sub(T *ptr, U value) {
    return __atomic_internal::sync_fetch_and_sub<T, U, sizeof(T)>()(ptr, value);
  }


  /*
   * sub-and-fetch internal definitions
   */

  namespace __atomic_internal {

    template<typename T, typename U, size_t sz>
    struct sync_sub_and_fetch {
    };

    template<typename T, typename U>
    struct sync_sub_and_fetch<T, U, sizeof(long)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1:   ldrex   %0,  [%1]     \n\t"
            "sub     %0,   %0,  %2    \n\t"
            "strex   r1,   %0, [%1] \n\t"
            "cmp     r1,   #0     \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1"
        );
        return result;
      }
    };

    template<typename T, typename U>
    struct sync_sub_and_fetch<T, U, sizeof(short)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1:   ldrexh  %0,  [%1]     \n\t"
            "sub     %0,   %0,  %2    \n\t"
            "strexh  r1,   %0, [%1] \n\t"
            "cmp     r1,   #0     \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1"
        );
        return result;
      }
    };

    template<typename T, typename U>
    struct sync_sub_and_fetch<T, U, sizeof(char)> {
      T operator()(T *ptr, U value) const {
        register T result;
        asm volatile (
            "1:   ldrexb  %0,  [%1]     \n\t"
            "sub     %0,   %0,  %2    \n\t"
            "strexb  r1,   %0, [%1] \n\t"
            "cmp     r1,   #0     \n\t"
            "bne     1b"
            : "=&r" (result)
            : "r"(ptr), "r"(value)
            : "r1"
        );
        return result;
      }
    };
  }

  /*
   * increment/decrement API
   */

  template<typename T, typename U>
  inline T sync_sub_and_fetch(T *ptr, U value) {
    return __atomic_internal::sync_sub_and_fetch<T, U, sizeof(T)>()(ptr, value);
  }

  template<typename T>
  inline T sync_increment_and_fetch(T *ptr) {
    return __atomic_internal::sync_add_and_fetch<T, int, sizeof(T)>()(ptr, 1);
  }

  template<typename T>
  inline T sync_decrement_and_fetch(T *ptr) {
    return __atomic_internal::sync_sub_and_fetch<T, int, sizeof(T)>()(ptr, 1);
  }

  template<typename T>
  inline T sync_fetch_and_increment(T *ptr) {
    return __atomic_internal::sync_fetch_and_add<T, int, sizeof(T)>()(ptr, 1);
  }

  template<typename T>
  inline T sync_fetch_and_decrement(T *ptr) {
    return __atomic_internal::sync_fetch_and_sub<T, int, sizeof(T)>()(ptr, 1);
  }
}
