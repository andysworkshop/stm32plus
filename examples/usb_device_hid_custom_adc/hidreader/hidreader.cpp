/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"
#include "hidreader.h"
#include "hidReaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*
 * Message map
 */

BEGIN_MESSAGE_MAP(CHidReaderApp, CWinApp)
END_MESSAGE_MAP()

// The one and only CHidReaderApp object

CHidReaderApp theApp;


/*
 * Init instance
 */

BOOL CHidReaderApp::InitInstance() {

	CWinApp::InitInstance();

	CHidReaderDlg dlg;
	m_pMainWnd=&dlg;
  dlg.DoModal();

  return FALSE;
}

