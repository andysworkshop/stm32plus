/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Class to search for our device and get its path
 */

class UsbEnumerate {

public:
  CString _path;

public:
  UsbEnumerate(uint16_t vid,uint16_t pid);

  const CString& getPath() const;
};


/*
 * Get the path to the device or empty string if not found
 */

inline const CString& UsbEnumerate::getPath() const {
  return _path;
}


/*
 * Search for the device and set the path
 */

inline UsbEnumerate::UsbEnumerate(uint16_t vid,uint16_t pid) {

  HDEVINFO                         hDevInfo;
  SP_DEVICE_INTERFACE_DATA         DevIntfData;
  PSP_DEVICE_INTERFACE_DETAIL_DATA DevIntfDetailData;
  SP_DEVINFO_DATA                  DevData;
  DWORD dwSize,dwMemberIdx;
  TCHAR devid[100];
  GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, {0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30} };

  wsprintf(devid,_T("vid_%04hx&pid_%04hx"),vid,pid);

  // We will try to get device information set for all USB devices that have a
  // device interface and are currently present on the system (plugged in).

  hDevInfo=SetupDiGetClassDevs(&InterfaceClassGuid,NULL,0,DIGCF_DEVICEINTERFACE|DIGCF_PRESENT);

  if(hDevInfo!=INVALID_HANDLE_VALUE) {

    // Prepare to enumerate all device interfaces for the device information
    // set that we retrieved with SetupDiGetClassDevs(..)
    DevIntfData.cbSize=sizeof(SP_DEVICE_INTERFACE_DATA);
    dwMemberIdx=0;

    // Next, we will keep calling this SetupDiEnumDeviceInterfaces(..) until this
    // function causes GetLastError() to return  ERROR_NO_MORE_ITEMS. With each
    // call the dwMemberIdx value needs to be incremented to retrieve the next
    // device interface information.

    SetupDiEnumDeviceInterfaces(hDevInfo,NULL,&InterfaceClassGuid,dwMemberIdx,&DevIntfData);

    while(GetLastError()!=ERROR_NO_MORE_ITEMS) {

      // As a last step we will need to get some more details for each
      // of device interface information we are able to retrieve. This
      // device interface detail gives us the information we need to identify
      // the device (VID/PID), and decide if it's useful to us. It will also
      // provide a DEVINFO_DATA structure which we can use to know the serial
      // port name for a virtual com port.

      DevData.cbSize=sizeof(DevData);

      // Get the required buffer size. Call SetupDiGetDeviceInterfaceDetail with
      // a NULL DevIntfDetailData pointer, a DevIntfDetailDataSize
      // of zero, and a valid RequiredSize variable. In response to such a call,
      // this function returns the required buffer size at dwSize.

      SetupDiGetDeviceInterfaceDetail(hDevInfo,&DevIntfData,NULL,0,&dwSize,NULL);

      // Allocate memory for the DeviceInterfaceDetail struct

      DevIntfDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
      DevIntfDetailData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

      if(SetupDiGetDeviceInterfaceDetail(hDevInfo,&DevIntfData,DevIntfDetailData,dwSize,&dwSize,&DevData)) {
        // Finally we can start checking if we've found a useable device,
        // by inspecting the DevIntfDetailData->DevicePath variable.
        // The DevicePath looks something like this:
        //
        // \\?\usb#vid_04d8&pid_0033#5&19f2438f&0&2#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
        //
        // As you can see it contains the VID/PID for the device, so we can check
        // for the right VID/PID with string handling routines.

        if(_tcsstr((TCHAR*)DevIntfDetailData->DevicePath,devid)!=NULL) {

          _path=DevIntfDetailData->DevicePath;
          HeapFree(GetProcessHeap(),0,DevIntfDetailData);
          break;
        }
      }

      HeapFree(GetProcessHeap(),0,DevIntfDetailData);

      // Continue looping
      SetupDiEnumDeviceInterfaces(hDevInfo,NULL,&InterfaceClassGuid,++dwMemberIdx,&DevIntfData);
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
  }
}
