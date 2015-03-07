/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include "resource.h"


/*
 * MFC dialog class
 */

class CHidReaderDlg : public CDialogEx {

  protected:
    HANDLE _deviceHandle;
    CWinThread *_pThread;
    CEvent _dataEvent;
    CEvent _exitEvent;

    OVERLAPPED _overlapped;
    uint8_t _report[7];

    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

  protected:
   
    enum {
      WM_USER_REPORT = WM_USER+1,
      WM_USER_ERROR  = WM_USER+2
    };

    void postResult();
    void postError(DWORD code);
    void setTime();

    afx_msg LRESULT OnReport(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnError(WPARAM wParam,LPARAM lParam);

  public:
	  CHidReaderDlg();
    ~CHidReaderDlg();

    void threadProc();

	  enum { IDD = IDD_HIDREADER_DIALOG };
    afx_msg void OnClose();
};
