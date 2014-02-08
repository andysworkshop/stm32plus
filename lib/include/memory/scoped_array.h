/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {

  /**
   * scoped_array<C> is like scoped_ptr<C>, except that the caller must allocate
   * with new [] and the destructor deletes objects with delete [].
   *
   * As with scoped_ptr<C>, a scoped_array<C> either points to an object
   * or is nullptr.  A scoped_array<C> owns the object that it points to.
   * scoped_array<T> is thread-compatible, and once you index into it,
   * the returned objects have only the threadsafety guarantees of T.
   *
   * Size: sizeof(scoped_array<C>) == sizeof(C*)
   * template <class C>
   *
   * This class comes from the Chromium project: http://www.chromium.org/
   * The Chromium license and the rights of the original authors are
   * preserved in full. The stm32plus license does not apply to this file.
   */

  template <class C>
  class scoped_array {

    public:

      // The element type
      typedef C element_type;

      // Constructor.  Defaults to intializing with nullptr.
      // There is no way to create an uninitialized scoped_array.
      // The input parameter must be allocated with new [].
      explicit scoped_array(C* p=nullptr) :
          array_(p) {
      }

      // Destructor.  If there is a C object, delete it.
      // We don't need to test ptr_ == nullptr because C++ does that for us.
      ~scoped_array() {
        enum {
          type_must_be_complete=sizeof(C)
        };
        delete[] array_;
      }

      // Reset.  Deletes the current owned object, if any.
      // Then takes ownership of a new object, if given.
      // this->reset(this->get()) works.
      void reset(C* p=nullptr) {
        if(p != array_) {
          enum {
            type_must_be_complete=sizeof(C)
          };
          delete[] array_;
          array_=p;
        }
      }

      // Get one element of the current object.
      C& operator[](std::ptrdiff_t i) const {
        return array_[i];
      }

      // Get a pointer to the zeroth element of the current object.
      // If there is no current object, return nullptr.
      C* get() const {
        return array_;
      }

      // Comparison operators.
      // These return whether two scoped_array refer to the same object, not just to
      // two different but equal objects.
      bool operator==(C* p) const {
        return array_ == p;
      }
      bool operator!=(C* p) const {
        return array_ != p;
      }

      // Swap two scoped arrays.
      void swap(scoped_array& p2) {
        C* tmp=array_;
        array_=p2.array_;
        p2.array_=tmp;
      }

      // Release an array.
      // The return value is the current pointer held by this object.
      // If this object holds a nullptr pointer, the return value is nullptr.
      // After this operation, this object will hold a nullptr pointer,
      // and will not own the object any more.
      C* release() {
        C* retVal=array_;
        array_=nullptr;
        return retVal;
      }

    private:
      C* array_;

      // Forbid comparison of different scoped_array types.
      template<class C2> bool operator==(scoped_array<C2> const& p2) const;
      template<class C2> bool operator!=(scoped_array<C2> const& p2) const;

      // Disallow evil constructors
      scoped_array(const scoped_array&);
      void operator=(const scoped_array&);
  };


  // Free functions
  template<class C>
  void swap(scoped_array<C>& p1,scoped_array<C>& p2) {
    p1.swap(p2);
  }

  template<class C>
  bool operator==(C* p1,const scoped_array<C>& p2) {
    return p1 == p2.get();
  }

  template<class C>
  bool operator!=(C* p1,const scoped_array<C>& p2) {
    return p1 != p2.get();
  }
}
