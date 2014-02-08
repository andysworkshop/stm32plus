/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


extern "C" {

  void MEM_DataCopy(void *,const void *,size_t);
}


/**
 * Hard-fault issues have been reported with MEM_DataCopy that I have not yet been able
 * to reproduce. For that reason it's now an opt-in optimisation
 */

#if defined(USE_MEMDATACOPY)
#undef memcpy                   // just in case
#define memcpy(a,b,c) MEM_DataCopy(a,b,c)
#endif
