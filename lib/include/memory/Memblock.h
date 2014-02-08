/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


/**
 * @brief Template class to manage a block of data on the heap
 * @tparam T The type being managed here.
 *
 * Assignment makes a copy and does not transfer ownership of the original. Therefore a=b
 * will make a new copy of b. Memory is copied from b to a using memcpy so obviously this
 * only works with simple types and structures.
 */

  template<typename T>
  class Memblock {

    private:
      T *_data;
      uint32_t _size;

    public:

      /**
       * Default constructor
       */

      Memblock() {
        _data=nullptr;
        _size=0;
      }


      /**
       * Construct with space for the given number of types.
       * @param[in] numTypes The number of types to store here.
       */

      Memblock(uint32_t numTypes) {
        allocate(numTypes);
      }


      /**
       * Copy constructor.
       * @param[in] src The memory block to copy from
       */

      Memblock(const Memblock<T>& src) {
        assign(src);
      }


      /**
       * Destructor, de-allocate memory
       */

      ~Memblock() {
        cleanup();
      }


      /**
       * Get a pointer to the data block.
       * @return A pointer.
       */

      T *getData() const {
        return _data;
      }


      /**
       * Get the number of types being stored here.
       * @return The number of types.
       */

      uint32_t getSize() const {
        return _size;
      }


      /**
       * Cast operator
       * @return A pointer to the data.
       */

      operator T* () {
        return _data;
      }


      /**
       * Assignment operator.
       * @param[in] src The block to assign from.
       * @return A reference to this.
       */

      Memblock<T>& operator=(const Memblock<T>& src) {
        cleanup();
        assign(src);

        return *this;
      }


      /**
       * Array access operator.
       * @param[in] pos The index in the block to return.
       * @return A reference to the item at the index.
       */

      T& operator[](int pos) const {
        return _data[pos];
      }


      /**
       * Reallocate to handle new data. Only increases are supported.
       * @param[in] newSize The size to increase to.
       */

      void reallocate(uint32_t newSize) {
        T *newData;

        if(_data==nullptr)
          allocate(newSize);
        else {
          newData=new T[newSize];
          memcpy(newData,_data,_size*sizeof(T));
          delete [] _data;
          _data=newData;
          _size=newSize;
        }
      }

      /**
       * Free existing data and start with a fresh buffer of the given size
       * @param[in] resetSize The size to start again with
       */

      void reset(uint32_t resetSize) {
        cleanup();
        allocate(resetSize);
      }

    private:

      void allocate(uint32_t numTypes) {
        _data=new T[numTypes];
        _size=numTypes;
      }

      void cleanup() {
        if(_data!=nullptr) {
          delete [] _data;
          _data=nullptr;
          _size=0;
        }
      }

      void assign(const Memblock<T>& src) {
        _data=new T[src.getSize()];
        _size=src.getSize();
        memcpy(_data,const_cast<Memblock<T>&>(src),_size*sizeof(T));
      }
  };


  /// Typedef for the common case of a block of bytes
  typedef Memblock<uint8_t> ByteMemblock;
}
