/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"
#include "hidreader.h"
#include "hidReaderDlg.h"
#include "afxdialogex.h"
#include "UsbEnumerate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*
 * Constructor
 */

CHidReaderDlg::CHidReaderDlg()
  : CDialogEx(CHidReaderDlg::IDD,NULL),
    _dataEvent(FALSE,TRUE),
    _exitEvent(FALSE,TRUE) {
  
  _deviceHandle=INVALID_HANDLE_VALUE;
  _pThread=NULL;
}


/*
 * Destructor
 */

CHidReaderDlg::~CHidReaderDlg() {

  if(_deviceHandle!=INVALID_HANDLE_VALUE)
    CloseHandle(_deviceHandle);
}


/*
 * Message map
 */

BEGIN_MESSAGE_MAP(CHidReaderDlg,CDialogEx)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_CLOSE()
  ON_WM_NCRBUTTONDBLCLK()
  ON_MESSAGE(WM_USER_REPORT,OnReport)
  ON_MESSAGE(WM_USER_ERROR,OnError)
END_MESSAGE_MAP()


/*
 * Callback used to jump into the class
 */

UINT __cdecl stubThreadProc(LPVOID lpParameter) {
  ((CHidReaderDlg *)lpParameter)->threadProc();
  return 0;
}


/*
 * Dialog init 
 */

BOOL CHidReaderDlg::OnInitDialog() {

  CDialogEx::OnInitDialog();

  // find the connected device

  UsbEnumerate usb(0xDEAD,0x0004);

  if(usb.getPath().IsEmpty())
    MessageBox(_T("Cannot find USB device. Please ensure that it's switched on and connected to the PC"));
  else {
  
    // open the device

	  if((_deviceHandle=CreateFile(usb.getPath(),GENERIC_WRITE | GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL))==INVALID_HANDLE_VALUE)
      MessageBox(_T("The USB device has been located but we failed to open it for reading"));
    else {

      // start the monitoring thread

      _pThread=AfxBeginThread(&stubThreadProc,this,0,0,CREATE_SUSPENDED);
      _pThread->m_bAutoDelete=FALSE;
      _pThread->ResumeThread();
    }
  }

  return TRUE;
}


/*
 * This thread will continually monitor the handle
 */

void CHidReaderDlg::threadProc() {
  
  HANDLE h[2];
  DWORD retval;

  ZeroMemory(&_overlapped,sizeof(_overlapped));
  _overlapped.hEvent=_dataEvent;

  h[0]=_dataEvent;
  h[1]=_exitEvent;

  for(;;) {
  
    // quick check if need to exit

    if(WaitForSingleObject(_exitEvent,0)==WAIT_OBJECT_0)
      break;

    if(!ReadFile(_deviceHandle,_report,sizeof(_report),NULL,&_overlapped)) {

      if(GetLastError()==ERROR_IO_PENDING) {

        if((retval=WaitForMultipleObjects(sizeof(h)/sizeof(h[0]),h,FALSE,INFINITE))==WAIT_OBJECT_0)
          postResult();
        else if(retval==WAIT_OBJECT_0+1)
          break;
      }
      else {
        postError(GetLastError());
        break;
      }
    }
    else
      postResult();
  }

  CancelIo(_deviceHandle);
}


/*
 * Post message synchronously to show the result
 */

void CHidReaderDlg::postResult() {
  SendMessage(WM_USER_REPORT);
}


/*
 * Post message synchronously to show an error
 */

void CHidReaderDlg::postError(DWORD code) {
  SendMessage(WM_USER_ERROR,0,code);
}


/*
 * Closing
 */

void CHidReaderDlg::OnClose() {

  if(_pThread) {
  
    // set the exit event

    _exitEvent.SetEvent();

    // wait for a reasonable time

    if(WaitForSingleObject(_pThread,5000)==WAIT_TIMEOUT)
      MessageBox(_T("Failed to terminate monitoring thread"));

    delete _pThread;
  }

  // call the base class

  CDialogEx::OnClose();
}


/*
 * A new report is available
 */

LRESULT CHidReaderDlg::OnReport(WPARAM wParam,LPARAM lParam) {

  TCHAR buffer[100];
  uint16_t ch1,ch2,ch3,*ptr;

  // prepare a text report

  ptr=reinterpret_cast<uint16_t *>(&_report[1]);
  ch1=*ptr++;
  ch2=*ptr++;
  ch3=*ptr;

  wsprintf(buffer,_T("%d: ch1=%03X ch2=%03X ch3=%03X"),_report[0],ch1,ch2,ch3);

  // set to the control

  SetDlgItemText(IDC_VALUE,buffer);

  // set the time

  setTime();
  return 0;
}


/*
 * A new error occurred
 */

LRESULT CHidReaderDlg::OnError(WPARAM wParam,LPARAM lParam) {
  
  TCHAR buffer[100];

  wsprintf(buffer,_T("Error: %08lX"),lParam);
  SetDlgItemText(IDC_VALUE,buffer);

  return 0;
}


/*
 * Set the time to the control
 */

void CHidReaderDlg::setTime() {

  TCHAR buffer[100];
  SYSTEMTIME st;

  // prepare the time

  GetSystemTime(&st);
  GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT,0,&st,NULL,buffer,sizeof(buffer)/sizeof(buffer[0]));

  SetDlgItemText(IDC_TIME,buffer);
}
