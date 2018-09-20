// DInputUtil.cpp
// ����:��ʼ����������DInput,������Ϸ����ע����е�DInput��Ϣ
// �汾:1.2003.10.06
// ����:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __DINPUTUTIL_H__
#define __DINPUTUTIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DiGameCntrlPropSheet.h"

/////////////////////////////////////////////////////////////////////////////
HRESULT CreateDIDevice2(short JoystickID, HWND hWnd, LPDIRECTINPUT pDirectInput, LPDIRECTINPUTJOYCONFIG pDIJoyConfig, LPDIRECTINPUTDEVICE2* ppDIDevice2);
HRESULT OpenConfigKey2K(GUID GuidInstance, PHKEY pConfigKey);
HRESULT OpenConfigKeyXP(GUID GuidInstance, PHKEY pConfigKey);
HRESULT OpenConfigKeyVISTA(GUID GuidInstance, PHKEY pConfigKey);
HRESULT SetConfig98(short JoystickID, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag);
HRESULT SetConfigME(short JoystickID, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag);
HRESULT SetConfig2K(GUID GuidInstance, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag);
HRESULT SetConfigXP(GUID GuidInstance, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag);
HRESULT SetConfigVISTA(GUID GuidInstance, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag);

//���ں���
HRESULT InitDInput(HWND hWnd, CDIGameCntrlPropSheet* pDIGameCtrlPropSheet);
HRESULT ReInitDInput(HWND hWnd, CDIGameCntrlPropSheet* pDIGameCtrlPropSheet);
HRESULT SetConfig(LPDIJOYCONFIG pDIJoyConfig, DWORD Flag, GUID GuidInstance, short JoystickID);
HANDLE GetDeviceHandle(LPDIRECTINPUTDEVICE pDID);
DWORD GetDxVersion();
/////////////////////////////////////////////////////////////////////////////
#endif // __DINPUTUTIL_H__
