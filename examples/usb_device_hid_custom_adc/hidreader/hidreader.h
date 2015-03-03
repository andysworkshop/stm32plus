/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Application class
 */

class CHidReaderApp : public CWinApp
{
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
