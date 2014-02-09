/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   *
   * A scoped_ptr<T> is like a T*, except that the destructor of scoped_ptr<T>
   * automatically deletes the pointer it holds (if any).
   * That is, scoped_ptr<T> owns the T object that it points to.
   * Like a T*, a scoped_ptr<T> may hold either nullptr or a pointer to a T object.
   * Also like T*, scoped_ptr<T> is thread-compatible, and once you
   * dereference it, you get the threadsafety guarantees of T.
   *
   * The size of a scoped_ptr is small:
   * sizeof(scoped_ptr<C>) == sizeof(C*)
   *
   * This class comes from the Chromium project: http://www.chromium.org/
   * The Chromium license and the rights of the original authors are
   * preserved in full. The stm32plus license does not apply to this file.
   */

  template<class C>
  class scoped_ptr {
    public:

      // The element type
      typedef C element_type;

      // Constructor.  Defaults to intializing with nullptr.
      // There is no way to create an uninitialized scoped_ptr.
      // The input parameter must be allocated with new.
      explicit scoped_ptr(C* p=nullptr) :
          ptr_(p) {
      }

      // Destructor.  If there is a C object, delete it.
      // We don't need to test ptr_ == nullptr because C++ does that for us.
      ~scoped_ptr() {
        enum {
          type_must_be_complete=sizeof(C)
        };
        delete ptr_;
      }

      // Reset.  Deletes the current owned object, if any.
      // Then takes ownership of a new object, if given.
      // this->reset(this->get()) works.
      void reset(C* p=nullptr) {
        if(p != ptr_) {
          enum {
            type_must_be_complete=sizeof(C)
          };
          delete ptr_;
          ptr_=p;
        }
      }

      // Accessors to get the owned object.
      C& operator*() const {
        return *ptr_;
      }
      C* operator->() const {
        return ptr_;
      }
      C* get() const {
        return ptr_;
      }

      C*& address() {
        return ptr_;
      }

      // Comparison operators.
      // These return whether two scoped_ptr refer to the same object, not just to
      // two different but equal objects.
      bool operator==(C* p) const {
        return ptr_ == p;
      }
      bool operator!=(C* p) const {
        return ptr_ != p;
      }

      // Swap two scoped pointers.
      void swap(scoped_ptr& p2) {
        C* tmp=ptr_;
        ptr_=p2.ptr_;
        p2.ptr_=tmp;
      }

      // Release a pointer.
      // The return value is the current pointer held by this object.
      // If this object holds a nullptr pointer, the return value is nullptr.
      // After this operation, this object will hold a nullptr pointer,
      // and will not own the object any more.
      C* release() {
        C* retVal=ptr_;
        ptr_=nullptr;
        return retVal;
      }

    private:
      C* ptr_;

      // Forbid comparison of scoped_ptr types.  If C2 != C, it totally doesn't
      // make sense, and if C2 == C, it still doesn't make sense because you should
      // never have the same object owned by two different scoped_ptrs.
      template<class C2> bool operator==(scoped_ptr<C2> const& p2) const;
      template<class C2> bool operator!=(scoped_ptr<C2> const& p2) const;

      // Disallow evil constructors
      scoped_ptr(const scoped_ptr&);
      void operator=(const scoped_ptr&);
  };

  // Free functions
  template<class C>
  void swap(scoped_ptr<C>& p1,scoped_ptr<C>& p2) {
    p1.swap(p2);
  }

  template<class C>
  bool operator==(C* p1,const scoped_ptr<C>& p2) {
    return p1 == p2.get();
  }

  template<class C>
  bool operator!=(C* p1,const scoped_ptr<C>& p2) {
    return p1 != p2.get();
  }
}
