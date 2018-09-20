// DInputUtil.cpp
// 功能:初始化或者重置DInput,设置游戏杆在注册表中的DInput信息
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#define DIRECTINPUT_VERSION 0x0700
#include "DInputUtil.h"
#include "GetOsType.h"

#include <Stdio.h>

extern CRITICAL_SECTION g_CriticalSection;
extern HINSTANCE g_hInst;
/////////////////////////////////////////////////////////////////////////////
HRESULT InitDInput(HWND hWnd, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet)
{
	EnterCriticalSection(&g_CriticalSection);
	HRESULT hResult;

	while(TRUE)
	{
		if((IsBadReadPtr((void*)pDIGameCntrlPropSheet, sizeof(CDIGameCntrlPropSheet))) || (IsBadWritePtr((void*)pDIGameCntrlPropSheet, sizeof(CDIGameCntrlPropSheet))))
		{
			hResult = E_POINTER;
			break;
		}
		LPDIRECTINPUTDEVICE2 pDIDevice2;
		LPDIRECTINPUTJOYCONFIG pDIJoyConfig;
		pDIGameCntrlPropSheet->GetDevice(&pDIDevice2);
		pDIGameCntrlPropSheet->GetJoyConfig(&pDIJoyConfig);
		LPDIRECTINPUT pDirectInput;

		if((NULL != pDIDevice2) && (NULL != pDIJoyConfig))
		{
			hResult = S_FALSE;
			break;
		}
		else
		{
			hResult = DirectInputCreate(g_hInst, DIRECTINPUT_VERSION, &pDirectInput, NULL);
			if(FAILED(hResult))
			{
				break;
			}
		}

		if(NULL == pDIJoyConfig)
		{
			hResult = pDirectInput->QueryInterface(IID_IDirectInputJoyConfig, (PVOID*)&pDIJoyConfig);
			if(SUCCEEDED(hResult))
			{
				pDIGameCntrlPropSheet->SetJoyConfig(pDIJoyConfig);
			}
			else
			{
				break;
			}
		}
		else
		{
			hResult = S_FALSE;
		}
		
		if(NULL == pDIDevice2)
		{
			if(NULL != pDIJoyConfig)
			{
				hResult = CreateDIDevice2(pDIGameCntrlPropSheet->GetID(), hWnd, pDirectInput, pDIJoyConfig, &pDIDevice2);
				if(SUCCEEDED(hResult))
				{
					DIPROPDWORD dpdx;
					dpdx.diph.dwSize = sizeof(DIPROPDWORD);
					dpdx.diph.dwHeaderSize = sizeof(DIPROPHEADER);
					dpdx.diph.dwObj = DIJOFS_X;
					dpdx.diph.dwHow = DIPH_BYOFFSET;
					dpdx.dwData = 500;
					pDIDevice2->SetProperty(DIPROP_DEADZONE, &dpdx.diph);

					DIPROPDWORD dpdy;
					dpdy.diph.dwSize = sizeof(DIPROPDWORD);
					dpdy.diph.dwHeaderSize = sizeof(DIPROPHEADER);
					dpdy.diph.dwObj = DIJOFS_Y;
					dpdy.diph.dwHow = DIPH_BYOFFSET;
					dpdy.dwData = 500;
					pDIDevice2->SetProperty(DIPROP_DEADZONE, &dpdy.diph);

					DIPROPDWORD dpdz;
					dpdz.diph.dwSize = sizeof(DIPROPDWORD);
					dpdz.diph.dwHeaderSize = sizeof(DIPROPHEADER);
					dpdz.diph.dwObj = DIJOFS_Z;
					dpdz.diph.dwHow = DIPH_BYOFFSET;
					dpdz.dwData = 500;
					pDIDevice2->SetProperty(DIPROP_DEADZONE, &dpdz.diph);

					DIPROPDWORD dpdr;
					dpdr.diph.dwSize = sizeof(DIPROPDWORD);
					dpdr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
					dpdr.diph.dwObj = DIJOFS_RZ;
					dpdr.diph.dwHow = DIPH_BYOFFSET;
					dpdr.dwData = 500;
					pDIDevice2->SetProperty(DIPROP_DEADZONE, &dpdr.diph);

					pDIGameCntrlPropSheet->SetDevice(pDIDevice2);
				}
				else
				{
					break;
				}
			}
			else
			{
				hResult = E_FAIL;
				break;
			}
		}
		else
		{
			hResult = S_FALSE;
		}
		if(NULL != pDirectInput)
		{
			pDirectInput->Release();
		}
		break;
	}

	LeaveCriticalSection(&g_CriticalSection);
	return hResult;
}

HRESULT ReInitDInput(HWND hWnd, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet)
{
	EnterCriticalSection(&g_CriticalSection);
	HRESULT hResult;

	while(TRUE)
	{
		if((IsBadReadPtr((void*)pDIGameCntrlPropSheet, sizeof(CDIGameCntrlPropSheet))) || (IsBadWritePtr((void*)pDIGameCntrlPropSheet, sizeof(CDIGameCntrlPropSheet))))
		{
			hResult = E_POINTER;
			break;
		}

		LPDIRECTINPUTDEVICE2 pDIDevice2;
		LPDIRECTINPUTJOYCONFIG pDIJoyConfig ;
		pDIGameCntrlPropSheet->GetDevice(&pDIDevice2);   
		pDIGameCntrlPropSheet->GetJoyConfig(&pDIJoyConfig);   
		LPDIRECTINPUT pDirectInput;

		hResult = DirectInputCreate(g_hInst, DIRECTINPUT_VERSION, &pDirectInput, NULL);
		if(FAILED(hResult))
		{
			break;
		}
		
		if(NULL != pDIJoyConfig)
		{
			pDIJoyConfig->Release();
			pDIJoyConfig = NULL;
		}

		if(NULL == pDIJoyConfig)
		{
			hResult = pDirectInput->QueryInterface(IID_IDirectInputJoyConfig, (PVOID*)&pDIJoyConfig);
			if(SUCCEEDED(hResult))
			{
				pDIGameCntrlPropSheet->SetJoyConfig(pDIJoyConfig);
			}
			else
			{
				break;
			}
		} 
		else 
		{
			hResult = S_FALSE;
		}

		if(NULL != pDIDevice2)
		{
			pDIDevice2->Unacquire();
			pDIDevice2->Release();
		}

		if(NULL != pDIJoyConfig)
		{
			hResult = CreateDIDevice2(pDIGameCntrlPropSheet->GetID(), hWnd, pDirectInput, pDIJoyConfig, &pDIDevice2);
			if(SUCCEEDED(hResult))
			{
				DIPROPDWORD dpdx;
				dpdx.diph.dwSize = sizeof(DIPROPDWORD);
				dpdx.diph.dwHeaderSize = sizeof(DIPROPHEADER);
				dpdx.diph.dwObj = DIJOFS_X;
				dpdx.diph.dwHow = DIPH_BYOFFSET;
				dpdx.dwData = 500;
				pDIDevice2->SetProperty(DIPROP_DEADZONE, &dpdx.diph);

				DIPROPDWORD dpdy;
				dpdy.diph.dwSize = sizeof(DIPROPDWORD);
				dpdy.diph.dwHeaderSize = sizeof(DIPROPHEADER);
				dpdy.diph.dwObj = DIJOFS_Y;
				dpdy.diph.dwHow = DIPH_BYOFFSET;
				dpdy.dwData = 500;
				pDIDevice2->SetProperty(DIPROP_DEADZONE, &dpdy.diph);

				DIPROPDWORD dpdz;
				dpdz.diph.dwSize = sizeof(DIPROPDWORD);
				dpdz.diph.dwHeaderSize = sizeof(DIPROPHEADER);
				dpdz.diph.dwObj = DIJOFS_Z;
				dpdz.diph.dwHow = DIPH_BYOFFSET;
				dpdz.dwData = 500;
				pDIDevice2->SetProperty(DIPROP_DEADZONE, &dpdz.diph);

				DIPROPDWORD dpdr;
				dpdr.diph.dwSize = sizeof(DIPROPDWORD);
				dpdr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
				dpdr.diph.dwObj = DIJOFS_RZ;
				dpdr.diph.dwHow = DIPH_BYOFFSET;
				dpdr.dwData = 500;
				pDIDevice2->SetProperty(DIPROP_DEADZONE, &dpdr.diph);

				pDIGameCntrlPropSheet->SetDevice(pDIDevice2);
			}
			else
			{
				break;
			}
		}
		else
		{
			hResult = E_FAIL;
			break;
		}

		if(NULL != pDirectInput)
		{
			pDirectInput->Release();
		}
		break;
	}

   LeaveCriticalSection(&g_CriticalSection);
   return hResult;
}

HRESULT CreateDIDevice2(short JoystickID, HWND hWnd, LPDIRECTINPUT pDirectInput, LPDIRECTINPUTJOYCONFIG pDIJoyConfig, LPDIRECTINPUTDEVICE2* ppDIDevice2)
{
	HRESULT hResult = E_NOTIMPL;
	LPDIRECTINPUTDEVICE pDIDevice2Temp  = NULL;
	DIJOYCONFIG DIJoyConfig;

	if((IsBadReadPtr((void*)pDirectInput, sizeof(IDirectInput))) || (IsBadWritePtr((void*)pDirectInput, sizeof(IDirectInput))))
	{
		return E_POINTER;
	}
	if((IsBadReadPtr((void*)pDIJoyConfig, sizeof(IDirectInputJoyConfig))) || (IsBadWritePtr((void*)pDIJoyConfig, sizeof(IDirectInputJoyConfig))))
	{
		return E_POINTER;
	}
	if((IsBadReadPtr((void*)ppDIDevice2, sizeof(LPDIRECTINPUTDEVICE2))) || (IsBadWritePtr((void*)ppDIDevice2, sizeof(LPDIRECTINPUTDEVICE2))))
	{
		return E_POINTER;
	}

	DIJoyConfig.dwSize = sizeof(DIJOYCONFIG);
	hResult = pDIJoyConfig->GetConfig(JoystickID, &DIJoyConfig, DIJC_GUIDINSTANCE);
	if(FAILED(hResult))
	{
		return hResult;
	}

	hResult = pDirectInput->CreateDevice(DIJoyConfig.guidInstance, &pDIDevice2Temp, NULL);
	if(FAILED(hResult))
	{
		return hResult;
	}

	hResult = pDIDevice2Temp->QueryInterface(IID_IDirectInputDevice2, (PVOID*)ppDIDevice2);
	pDIDevice2Temp->Release();
	if(FAILED(hResult))
	{
		return hResult;
	}

	hResult = (*ppDIDevice2)->SetDataFormat(&c_dfDIJoystick);
	if(FAILED(hResult))
	{
		return hResult;
	}

	hResult = (*ppDIDevice2)->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
	if(FAILED(hResult))
	{
		return hResult;
	}

	return hResult;
}

HRESULT SetConfig(LPDIJOYCONFIG pDIJoyConfig, DWORD Flag, GUID GuidInstance, short JoystickID)
{
	DWORD OsType = GetOsType();
	switch(OsType)
	{
	case WINDOWS_98:
		return SetConfig98(JoystickID, pDIJoyConfig, Flag);
	case WINDOWS_ME:
		return SetConfigME(JoystickID, pDIJoyConfig, Flag);
	case WINDOWS_2K:
	case WINDOWS_XP:
		if (GetDxVersion() >= 0x0801)
		{
			
			return SetConfigXP(GuidInstance, pDIJoyConfig, Flag);

		}
		else
		{
			return SetConfig2K(GuidInstance, pDIJoyConfig, Flag);
		}
		break;
	case WINDOWS_VISTA:
		return SetConfigVISTA(GuidInstance, pDIJoyConfig, Flag);

		break;
	default:
		break;
	}
	
	return E_FAIL;
}

HRESULT SetConfig98(short JoystickID, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag)
{
	HRESULT hResult = S_OK;
	
	HKEY ConfigKey;
	hResult = RegOpenKey(HKEY_LOCAL_MACHINE, _T("System\\CurrentControlSet\\Control\\MediaResources\\Joystick\\<FixedKey>\\CurrentJoystickSettings"), &ConfigKey);
	if(S_OK != hResult)
	{
		return hResult;
	}
	
	TCHAR ConfigKeyValue[256];
	_stprintf(ConfigKeyValue, _T("Joystick%dConfiguration"), JoystickID + 1);
	hResult = RegSetValueEx(ConfigKey, ConfigKeyValue, 0, REG_BINARY, (BYTE*)(&(pDIJoyConfig->hwc)), sizeof(pDIJoyConfig->hwc));
	RegCloseKey(ConfigKey);
	
	return hResult;
}

HRESULT SetConfigME(short JoystickID, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag)
{
	HRESULT hResult = S_OK;
	
	HKEY ConfigKey;
	hResult = RegOpenKey(HKEY_LOCAL_MACHINE, _T("System\\CurrentControlSet\\Control\\MediaResources\\Joystick\\<FixedKey>\\CurrentJoystickSettings"), &ConfigKey);
	if(S_OK != hResult)
	{
		return hResult;
	}
	
	TCHAR ConfigKeyValue[256];
	_stprintf(ConfigKeyValue, _T("Joystick%dConfiguration"), JoystickID + 1);
	hResult = RegSetValueEx(ConfigKey, ConfigKeyValue, 0, REG_BINARY, (BYTE*)(&(pDIJoyConfig->hwc)), sizeof(pDIJoyConfig->hwc));
	RegCloseKey(ConfigKey);
	
	return hResult;
}

HRESULT SetConfig2K(GUID GuidInstance, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag)
{
	HRESULT hResult = S_OK;
	
	HKEY ConfigKey = NULL;
	OpenConfigKey2K(GuidInstance, &ConfigKey);
	if(NULL == ConfigKey)
	{
		return E_FAIL;
	}

	if(DIJC_REGHWCONFIGTYPE == (Flag & DIJC_REGHWCONFIGTYPE))
	{
		HKEY AxesKey;
		hResult = RegOpenKey(ConfigKey, _T("#\\Device Parameters\\DirectX\\Type\\Axes"), &AxesKey);
		if(S_OK != hResult)
		{
			if(ERROR_FILE_NOT_FOUND == hResult)
			{
				hResult = RegCreateKey(ConfigKey, _T("#\\Device Parameters\\DirectX\\Type\\Axes"), &AxesKey);
			}
			else
			{
				RegCloseKey(ConfigKey);
				return hResult;
			}
		}

		HKEY OneAxesKey;
		TCHAR AxesName[4][2] = {_T("0"), _T("1"), _T("2"), _T("5")};
		DWORD Data[3] = {0, 0, 0};
		DWORD DataLength = sizeof(Data);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwX;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwX;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwX;
		hResult = RegOpenKey(AxesKey, AxesName[0], &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
			hResult = RegCreateKey(AxesKey, AxesName[0], &OneAxesKey);
		}
		hResult = RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwY;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwY;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwY;
		hResult = RegOpenKey(AxesKey, AxesName[1], &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
			hResult = RegCreateKey(AxesKey, AxesName[1], &OneAxesKey);
		}
		RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwZ;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwZ;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwZ;
		hResult = RegOpenKey(AxesKey, AxesName[2], &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
			hResult = RegCreateKey(AxesKey, AxesName[2], &OneAxesKey);
		}
		RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwR;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwR;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwR;
		hResult = RegOpenKey(AxesKey, AxesName[3], &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
			hResult = RegCreateKey(AxesKey, AxesName[3], &OneAxesKey);
		}
		RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		RegCloseKey(AxesKey);
	}
	
	RegCloseKey(ConfigKey);
	
	return hResult;
}

HRESULT OpenConfigKey2K(GUID GuidInstance, PHKEY pConfigKey)
{
	HRESULT hResult = S_OK;
	
	HKEY DeviceClassKey;
	hResult = RegOpenKey(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\DeviceClasses\\{4d1e55b2-f16f-11cf-88cb-001111000030}"), &DeviceClassKey);
	if(S_OK != hResult)
	{
		return hResult;
	}

	HKEY DeviceKey = NULL;
	TCHAR DeviceKeyName[2048];
	int i = 0;
	while(TRUE)
	{
		hResult = RegEnumKey(DeviceClassKey, i ++, DeviceKeyName, 2048);
		if(S_OK != hResult)
		{
			RegCloseKey(DeviceClassKey);
			return hResult;
		}
		if(NULL != DeviceKey)
		{
			RegCloseKey(DeviceKey);
		}
		hResult = RegOpenKey(DeviceClassKey, DeviceKeyName, &DeviceKey);
		if(S_OK != hResult)
		{
			RegCloseKey(DeviceClassKey);
			return hResult;
		}

		HKEY GuidKey;
		hResult = RegOpenKey(DeviceKey, _T("#\\Device Parameters\\DirectX"), &GuidKey);
		if(FAILED(hResult))
		{
			RegCloseKey(DeviceKey);
			RegCloseKey(DeviceClassKey);
			return hResult;
		}
		if(S_OK != hResult)
		{
			continue;
		}

		GUID Data[1];
		DWORD DataLength = sizeof(Data);
		hResult = RegQueryValueEx(GuidKey, _T("GUID"), 0, NULL, (BYTE*)(Data), &DataLength);
		if(FAILED(hResult))
		{
			RegCloseKey(GuidKey);
			RegCloseKey(DeviceKey);
			RegCloseKey(DeviceClassKey);
			return hResult;
		}
		if(S_OK != hResult)
		{
			RegCloseKey(GuidKey);
			continue;
		}
		
		if(IsEqualGUID(Data[0], GuidInstance))
		{
			RegCloseKey(GuidKey);
			break;
		}

		RegCloseKey(GuidKey);
	}

	*pConfigKey = DeviceKey;
	
	return hResult;
}

HRESULT SetConfigVISTA(GUID GuidInstance, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag)
{
	HRESULT hResult = S_OK;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
	BYTE aclBuffer[1024];
	PACL pacl=(PACL)&aclBuffer;
	BYTE sidBuffer[100];
	PSID psid=(PSID) &sidBuffer;
	DWORD sidBufferSize = 100;
	TCHAR domainBuffer[80];
	DWORD domainBufferSize = 80;
	SID_NAME_USE snu;
	
	InitializeSecurityDescriptor(&sd,
		SECURITY_DESCRIPTOR_REVISION);
	InitializeAcl(pacl, 1024, ACL_REVISION);
	LookupAccountName(0, _T("Everyone"), psid,
		&sidBufferSize, domainBuffer,
		&domainBufferSize, &snu);
	AddAccessAllowedAce(pacl, ACL_REVISION,
		KEY_ALL_ACCESS|0x0200|0x0100, psid);
	SetSecurityDescriptorDacl(&sd, TRUE, pacl,
		FALSE);
	
	sa.nLength= sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = &sd;
	
	HKEY ConfigKey = NULL;
	hResult = OpenConfigKeyVISTA(GuidInstance, &ConfigKey);
	if(NULL == ConfigKey)
	{
		return E_FAIL;
	}

	if(DIJC_REGHWCONFIGTYPE == (Flag & DIJC_REGHWCONFIGTYPE))
	{
		HKEY AxesKey;
		hResult = RegOpenKeyEx(ConfigKey, _T("Type\\Axes"),0,KEY_READ|KEY_WRITE, &AxesKey);
		if(S_OK != hResult)
		{

			if(ERROR_FILE_NOT_FOUND == hResult)
			{
				DWORD dwDes;
				hResult=(ERROR_SUCCESS==RegCreateKeyEx(ConfigKey, _T("Type\\Axes"),0,_T(""),0,
		KEY_ALL_ACCESS|0x0200|0x0100,&sa,&AxesKey,&dwDes)); 
			}
			else
			{
				RegCloseKey(ConfigKey);
				return hResult;
			}
		}

		HKEY OneAxesKey;
		TCHAR AxesName[4][2] = {_T("0"), _T("1"), _T("2"), _T("5")};
		DWORD Data[3] = {0, 0, 0};
		DWORD DataLength = sizeof(Data);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwX;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwX;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwX;
		hResult = RegOpenKeyEx(AxesKey, AxesName[0],0,KEY_READ|KEY_WRITE, &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
		
			DWORD dwDes;
			hResult = RegCreateKeyEx(AxesKey, AxesName[0],0,NULL,0,KEY_READ|KEY_WRITE,&sa, &OneAxesKey,&dwDes);
		}
		hResult = RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		if (hResult!=ERROR_SUCCESS) 
		{
			
		}
		RegCloseKey(OneAxesKey);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwY;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwY;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwY;
		hResult = RegOpenKeyEx(AxesKey, AxesName[1],0,KEY_READ|KEY_WRITE, &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
				DWORD dwDes;
			hResult = RegCreateKeyEx(AxesKey, AxesName[1],0,NULL,0,KEY_READ|KEY_WRITE,&sa, &OneAxesKey,&dwDes);
		}
		RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwZ;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwZ;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwZ;
		hResult = RegOpenKeyEx(AxesKey, AxesName[2],0,KEY_READ|KEY_WRITE, &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
				DWORD dwDes;
			hResult = RegCreateKeyEx(AxesKey, AxesName[2],0,NULL,0,KEY_READ|KEY_WRITE,&sa, &OneAxesKey,&dwDes);
		}
		RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwR;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwR;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwR;
		hResult = RegOpenKeyEx(AxesKey, AxesName[3],0,KEY_READ|KEY_WRITE, &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
				DWORD dwDes;
			hResult = RegCreateKeyEx(AxesKey, AxesName[3],0,NULL,0,KEY_READ|KEY_WRITE,&sa, &OneAxesKey,&dwDes);
		}
		RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		RegCloseKey(AxesKey);
	}
	
	RegCloseKey(ConfigKey);
	
	return hResult;
}

HRESULT OpenConfigKeyVISTA(GUID GuidInstance, PHKEY pConfigKey)
{
	HRESULT hResult = S_OK;
	

	HKEY CalibrationKey;
	TCHAR KeyName[256];
	_stprintf(KeyName, _T("SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\DirectInput\\VID_%04X&PID_%04X\\Calibration"), VID, PID);
	hResult = RegOpenKeyEx(HKEY_CURRENT_USER, KeyName,0,KEY_READ|KEY_WRITE, &CalibrationKey);
	if(S_OK != hResult)
	{
		return hResult;
	}
	HKEY DeviceKey = NULL;
	TCHAR DeviceKeyName[2048];
	int i = 0;
	while(TRUE)
	{
		hResult = RegEnumKey(CalibrationKey, i ++, DeviceKeyName, 2048);
		if(S_OK != hResult)
		{
			RegCloseKey(CalibrationKey);
			return hResult;
		}
		if(NULL != DeviceKey)
		{
			RegCloseKey(DeviceKey);
		}
		hResult = RegOpenKeyEx(CalibrationKey, DeviceKeyName,0,KEY_READ|KEY_WRITE, &DeviceKey);
		if(S_OK != hResult)
		{
			RegCloseKey(CalibrationKey);
			return hResult;
		}

		GUID Data[1];
		DWORD DataLength = sizeof(Data);
		hResult = RegQueryValueEx(DeviceKey, _T("GUID"), 0, NULL, (BYTE*)(Data), &DataLength);
		if(FAILED(hResult))
		{
			RegCloseKey(DeviceKey);
			RegCloseKey(CalibrationKey);
			return hResult;
		}
		if(S_OK != hResult)
		{
			continue;
		}
		if(IsEqualGUID(Data[0], GuidInstance))
		{
			break;
		}
	}

	*pConfigKey = DeviceKey;
	
	return hResult;
}

DWORD GetDxVersion()
{
	HKEY hDxKey;
	DWORD hr = 0;
	hr = RegOpenKey(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\DirectX"), &hDxKey);
	if (hr != S_OK){
		return 0x0700;
	}

	TCHAR strData[256];
	DWORD BytesTxd = 256;
	hr = RegQueryValueEx(hDxKey, _T("Version"), NULL, NULL, (BYTE *)(&strData[0]), &BytesTxd);
	if (hr != S_OK){
		RegCloseKey(hDxKey);
		return 0x0700;
	}

	DWORD Version1 = 0, Version2 = 0, Version3 = 0, Version4 = 0;
	int i = _stscanf(strData, _T("%x.%x.%x.%x"), &Version1, &Version2, &Version3, &Version4);
	if (i != 4){
		RegCloseKey(hDxKey);
		return 0x0700;
	}

	RegCloseKey(hDxKey);
	return Version2 * 256 + Version3;
}


HRESULT SetConfigXP(GUID GuidInstance, LPDIJOYCONFIG pDIJoyConfig, DWORD Flag)
{
	HRESULT hResult = S_OK;
	
	HKEY ConfigKey = NULL;
	hResult = OpenConfigKeyXP(GuidInstance, &ConfigKey);
	if(NULL == ConfigKey)
	{
		return E_FAIL;
	}

	if(DIJC_REGHWCONFIGTYPE == (Flag & DIJC_REGHWCONFIGTYPE))
	{
		HKEY AxesKey;
		hResult = RegOpenKey(ConfigKey, _T("Type\\Axes"), &AxesKey);
		if(S_OK != hResult)
		{
			if(ERROR_FILE_NOT_FOUND == hResult)
			{
				hResult = RegCreateKey(ConfigKey, _T("Type\\Axes"), &AxesKey);
			}
			else
			{
				RegCloseKey(ConfigKey);
				return hResult;
			}
		}

		HKEY OneAxesKey;
		TCHAR AxesName[4][2] = {_T("0"), _T("1"), _T("2"), _T("5")};
		DWORD Data[3] = {0, 0, 0};
		DWORD DataLength = sizeof(Data);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwX;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwX;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwX;
		hResult = RegOpenKey(AxesKey, AxesName[0], &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
			hResult = RegCreateKey(AxesKey, AxesName[0], &OneAxesKey);
		}
		hResult = RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwY;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwY;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwY;
		hResult = RegOpenKey(AxesKey, AxesName[1], &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
			hResult = RegCreateKey(AxesKey, AxesName[1], &OneAxesKey);
		}
		RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwZ;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwZ;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwZ;
		hResult = RegOpenKey(AxesKey, AxesName[2], &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
			hResult = RegCreateKey(AxesKey, AxesName[2], &OneAxesKey);
		}
		RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		Data[0] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMin.dwR;
		Data[1] = pDIJoyConfig->hwc.hwv.jrvHardware.jpCenter.dwR;
		Data[2] = pDIJoyConfig->hwc.hwv.jrvHardware.jpMax.dwR;
		hResult = RegOpenKey(AxesKey, AxesName[3], &OneAxesKey);
		if(ERROR_FILE_NOT_FOUND == hResult)
		{
			hResult = RegCreateKey(AxesKey, AxesName[3], &OneAxesKey);
		}
		RegSetValueEx(OneAxesKey, _T("Calibration"), 0, REG_BINARY, (BYTE*)Data, DataLength);
		RegCloseKey(OneAxesKey);
		RegCloseKey(AxesKey);
	}
	
	RegCloseKey(ConfigKey);
	
	return hResult;
}

HRESULT OpenConfigKeyXP(GUID GuidInstance, PHKEY pConfigKey)
{
	HRESULT hResult = S_OK;
	
	HKEY CalibrationKey;
	TCHAR KeyName[256];
	_stprintf(KeyName, _T("SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\DirectInput\\VID_%04X&PID_%04X\\Calibration"), VID, PID);
	hResult = RegOpenKey(HKEY_LOCAL_MACHINE, KeyName, &CalibrationKey);
	if(S_OK != hResult)
	{
		return hResult;
	}

	HKEY DeviceKey = NULL;
	TCHAR DeviceKeyName[2048];
	int i = 0;
	while(TRUE)
	{
		hResult = RegEnumKey(CalibrationKey, i++, DeviceKeyName, 2048);
		if(S_OK != hResult)
		{
			RegCloseKey(CalibrationKey);
			return hResult;
		}
		if(NULL != DeviceKey)
		{
			RegCloseKey(DeviceKey);
		}
		hResult = RegOpenKey(CalibrationKey, DeviceKeyName, &DeviceKey);
		if(S_OK != hResult)
		{
			RegCloseKey(CalibrationKey);
			return hResult;
		}

		GUID Data[1];
		DWORD DataLength = sizeof(Data);
		hResult = RegQueryValueEx(DeviceKey, _T("GUID"), 0, NULL, (BYTE*)(Data), &DataLength);
		if(FAILED(hResult))
		{
			RegCloseKey(DeviceKey);
			RegCloseKey(CalibrationKey);
			return hResult;
		}
		if(S_OK != hResult)
		{
			continue;
		}
		if(IsEqualGUID(Data[0], GuidInstance))
		{
			break;
		}
	}

	*pConfigKey = DeviceKey;
	
	return hResult;
}

bool GetDeviceName(wchar_t* wszPath, LPDIRECTINPUTDEVICE pd)
{
	DIPROPGUIDANDPATH dip;
	bool bRet(false);
	
	memset(&dip, 0, sizeof(DIPROPGUIDANDPATH));
	dip.diph.dwSize = sizeof(DIPROPGUIDANDPATH);
	dip.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dip.diph.dwObj = 0;
	dip.diph.dwHow = DIPH_DEVICE;
	
	HRESULT hr = pd->GetProperty(DIPROP_GUIDANDPATH,  &dip.diph);
	if (SUCCEEDED(hr))
	{
		bRet = true;
		wcscpy(wszPath, dip.wszPath);
	} 
	
	return bRet;
}
HANDLE GetDeviceHandle(LPDIRECTINPUTDEVICE pDID)
{
	
	wchar_t wszLink[MAX_PATH];
	HANDLE hDevice=NULL;
	if (GetDeviceName(wszLink, pDID))
	{
		LPTSTR DeviceName=(TCHAR *) malloc( _MAX_PATH );
		#ifndef UNICODE
				WideCharToMultiByte(CP_ACP, 0, wszLink, -1, DeviceName, MAX_PATH, NULL, NULL);
		#else
				lstrcpy(DeviceName, wszLink);
		#endif
	
		hDevice = CreateFile(DeviceName,GENERIC_READ|GENERIC_WRITE ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
		
		free(DeviceName);
	}
	return hDevice;
}