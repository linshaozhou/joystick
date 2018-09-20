// Calibration.cpp
// 功能:游戏控制面板属性页校准页脕E
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Resource.h"
#include "Calibration.h"

#include "Axes.h"

#include <Stdio.h>
/////////////////////////////////////////////////////////////////////////////
extern HINSTANCE g_hInst;
extern CRITICAL_SECTION g_CriticalSection;

DIJOYCONFIG JoyConfig;
UINT CalibrationConnect = 0;
/////////////////////////////////////////////////////////////////////////////
#define DWL_USER        8
//校准页面窗口过程
BOOL CALLBACK CalibrationProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CDIGameCntrlPropSheet *pDIGameCntrlPropSheet = (CDIGameCntrlPropSheet*)GetWindowLong(hWnd, DWL_USER);
	LPDIRECTINPUTDEVICE2 pDInputDevice2 = NULL;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			pDIGameCntrlPropSheet = (CDIGameCntrlPropSheet*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, DWL_USER, (LPARAM)pDIGameCntrlPropSheet);
			if(FAILED(InitDInput(GetParent(hWnd), pDIGameCntrlPropSheet)))
			{
		        return TRUE;
			}

			pDIGameCntrlPropSheet->GetDevice(&pDInputDevice2);

			LPDIRECTINPUTJOYCONFIG pDInputJoyConfig = NULL;
			pDIGameCntrlPropSheet->GetJoyConfig(&pDInputJoyConfig);
			pDInputJoyConfig->Acquire();
			JoyConfig.dwSize = sizeof(DIJOYCONFIG);
			HRESULT hResult = pDInputJoyConfig->GetConfig(pDIGameCntrlPropSheet->GetID(), &JoyConfig, DIJC_REGHWCONFIGTYPE);
			if(S_OK != hResult)
			{
				return FALSE;
			}
			pDInputJoyConfig->Unacquire();


			JoyConfig.hwc.hwv.jrvHardware.jpMin.dwX = 0x80;
			JoyConfig.hwc.hwv.jrvHardware.jpMin.dwY = 0x80;
			JoyConfig.hwc.hwv.jrvHardware.jpMin.dwZ = 0x80;
			JoyConfig.hwc.hwv.jrvHardware.jpMin.dwR = 0x80;

			JoyConfig.hwc.hwv.jrvHardware.jpMax.dwX = 0x80;
			JoyConfig.hwc.hwv.jrvHardware.jpMax.dwY = 0x80;
			JoyConfig.hwc.hwv.jrvHardware.jpMax.dwZ = 0x80;
			JoyConfig.hwc.hwv.jrvHardware.jpMax.dwR = 0x80;

			JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwX = 0x80;
			JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwY = 0x80;
			JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwZ = 0x80;
			JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwR = 0x80;
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, ID_CALIBRATION_TIMER);
		CalibrationConnect = 0;
		break;
	case WM_TIMER:
		DisplayCalibrationState(hWnd, pDIGameCntrlPropSheet);
		break;
	case WM_NOTIFY:
        switch(((LPNMHDR)lParam)->code)
		{
		case PSN_SETACTIVE:
			SetTimer(hWnd, ID_CALIBRATION_TIMER, CALIBRATION_TIMER_ELAPSE, NULL);
			break;
		case PSN_KILLACTIVE:
			KillTimer(hWnd, ID_CALIBRATION_TIMER);
			break;
        }
		return TRUE;
	case WM_COMMAND:
		{
			WORD wID = LOWORD(wParam);
			WORD wNotifyCode = HIWORD(wParam);
			switch (wNotifyCode)
			{
			case BN_CLICKED:
				switch (wID)
				{
				case IDC_SET:
				
					if(Calibration(&JoyConfig, pDIGameCntrlPropSheet))
					{
						ReInitDInput(GetParent(hWnd), pDIGameCntrlPropSheet);
					}
				
					break;
				case IDC_DEFAULT:
					JoyConfig.hwc.hwv.jrvHardware.jpMin.dwX = 0x00;
					JoyConfig.hwc.hwv.jrvHardware.jpMin.dwY = 0x00;
					JoyConfig.hwc.hwv.jrvHardware.jpMin.dwZ = 0x00;
					JoyConfig.hwc.hwv.jrvHardware.jpMin.dwR = 0x00;

					JoyConfig.hwc.hwv.jrvHardware.jpMax.dwX = 0xFF;
					JoyConfig.hwc.hwv.jrvHardware.jpMax.dwY = 0xFF;
					JoyConfig.hwc.hwv.jrvHardware.jpMax.dwZ = 0xFF;
					JoyConfig.hwc.hwv.jrvHardware.jpMax.dwR = 0xFF;

					JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwX = 0x80;
					JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwY = 0x80;
					JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwZ = 0x80;
					JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwR = 0x80;
					//2003.02.27删除开始
					/*
					Calibration(&JoyConfig, pDIGameCntrlPropSheet);
					ReInitDInput(GetParent(hWnd), pDIGameCntrlPropSheet);
					*/
					//2003.02.27删除结蕘E
					//2003.02.27铁赜开始
					if(Calibration(&JoyConfig, pDIGameCntrlPropSheet))
					{
						ReInitDInput(GetParent(hWnd), pDIGameCntrlPropSheet);
					}
					//2003.02.27铁赜结蕘E
					break;
				}
				break;
			}	
		}
		break;
	}
    
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////
//显示校准页脕E
HRESULT DisplayCalibrationState(HWND hWnd, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet)
{
	HRESULT hResult;

	if((IsBadReadPtr((void*)pDIGameCntrlPropSheet, sizeof(CDIGameCntrlPropSheet))))
	{
       return E_POINTER;
	}

	LPDIRECTINPUTDEVICE2 pDInputDevice2 = NULL;
	pDIGameCntrlPropSheet->GetDevice(&pDInputDevice2);

	hResult = pDInputDevice2->Acquire();
	if((DI_OK != hResult) && (S_FALSE != hResult))
	{
		if(CalibrationConnect != 2)
		{
			SetDlgItemText(hWnd, IDC_STATIC_DEVICESTATE_CALIBRATION, _T("僐儞僩儘乕儔乕偑愙懕偝傟偰偄傑偣傫丅"));
            HWND hItem=GetDlgItem(hWnd,IDC_CALIICON);
			ShowWindow(hItem,SW_SHOW);
			CalibrationConnect = 2;
		}
		return S_OK;
	}
	else
	{
		if(CalibrationConnect != 1)
		{
			SetDlgItemText(hWnd, IDC_STATIC_DEVICESTATE_CALIBRATION, _T("僐儞僩儘乕儔乕偑愙懕偝傟偰偄傑偡丅"));
            HWND hItem=GetDlgItem(hWnd,IDC_CALIICON);
			ShowWindow(hItem,SW_HIDE);
			CalibrationConnect = 1;
		}
	}

	DIJOYSTATE JoyState;
	static DIJOYSTATE OldJoyState;
	hResult = pDInputDevice2->GetDeviceState(sizeof(DIJOYSTATE), &JoyState);
	if(hResult == DI_OK)
	{
		if((OldJoyState.lX != JoyState.lX) || (OldJoyState.lY != JoyState.lY))
		{
			SetAxesState(hWnd, IDC_LAXIS, JoyState.lX, JoyState.lY);
			TCHAR szBuffer[50]={0};
			_stprintf(szBuffer, _T("%d"), (JoyState.lX>>8));
			SetDlgItemText(hWnd, IDC_XPOS, szBuffer);
			_stprintf(szBuffer, _T("%d"), (JoyState.lY>>8));
			SetDlgItemText(hWnd, IDC_YPOS, szBuffer);
		}
		//ADD 03/12/08 START
		if((OldJoyState.lZ != JoyState.lZ) || (OldJoyState.lRz != JoyState.lRz))
		{
			SetAxesState(hWnd, IDC_RAXIS, JoyState.lZ, JoyState.lRz);
			TCHAR szBuffer[50]={0};
			_stprintf(szBuffer, _T("%d"), (JoyState.lZ>>8));
			SetDlgItemText(hWnd, IDC_ZPOS, szBuffer);
			_stprintf(szBuffer, _T("%d"), (JoyState.lRz>>8));
			SetDlgItemText(hWnd, IDC_RzPOS, szBuffer);
		}
        //ADD END
	}

	EnterCriticalSection(&g_CriticalSection);
	if((DWORD)(JoyState.lX >> 8) < JoyConfig.hwc.hwv.jrvHardware.jpMin.dwX)
	{
		JoyConfig.hwc.hwv.jrvHardware.jpMin.dwX = JoyState.lX >> 8;
	}
	if((DWORD)(JoyState.lX >> 8) > JoyConfig.hwc.hwv.jrvHardware.jpMax.dwX)
	{
		JoyConfig.hwc.hwv.jrvHardware.jpMax.dwX = JoyState.lX >> 8;
	}

	if((DWORD)(JoyState.lY >> 8) < JoyConfig.hwc.hwv.jrvHardware.jpMin.dwY)
	{
		JoyConfig.hwc.hwv.jrvHardware.jpMin.dwY = JoyState.lY >> 8;
	}
	if((DWORD)(JoyState.lY >> 8) > JoyConfig.hwc.hwv.jrvHardware.jpMax.dwY)
	{
		JoyConfig.hwc.hwv.jrvHardware.jpMax.dwY = JoyState.lY >> 8;
	}

	if((DWORD)(JoyState.lZ >> 8) < JoyConfig.hwc.hwv.jrvHardware.jpMin.dwZ)
	{
		JoyConfig.hwc.hwv.jrvHardware.jpMin.dwZ = JoyState.lZ >> 8;
	}
	if((DWORD)(JoyState.lZ >> 8) > JoyConfig.hwc.hwv.jrvHardware.jpMax.dwZ)
	{
		JoyConfig.hwc.hwv.jrvHardware.jpMax.dwZ = JoyState.lZ >> 8;
	}

	if((DWORD)(JoyState.lRz >> 8) < JoyConfig.hwc.hwv.jrvHardware.jpMin.dwR)
	{
		JoyConfig.hwc.hwv.jrvHardware.jpMin.dwR = JoyState.lRz >> 8;
	}
	if((DWORD)(JoyState.lRz >> 8) > JoyConfig.hwc.hwv.jrvHardware.jpMax.dwR)
	{
		JoyConfig.hwc.hwv.jrvHardware.jpMax.dwR = JoyState.lRz >> 8;
	}
	LeaveCriticalSection(&g_CriticalSection);

	JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwX = JoyState.lX >> 8;
	JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwY = JoyState.lY >> 8;
	JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwZ = JoyState.lZ >> 8;
	JoyConfig.hwc.hwv.jrvHardware.jpCenter.dwR = JoyState.lRz >> 8;

	CopyMemory(&OldJoyState, &JoyState, sizeof(DIJOYSTATE));
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//校准并保存数据到注册眮E
//2003.02.27删除开始
/*
void Calibration(LPDIJOYCONFIG pJoyConfig, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet)
{
	HRESULT hResult;

	LPDIRECTINPUTJOYCONFIG pDInputJoyConfig = NULL;
	pDIGameCntrlPropSheet->GetJoyConfig(&pDInputJoyConfig);
	DIJOYCONFIG JoyConfig;
	JoyConfig.dwSize = sizeof(DIJOYCONFIG);
	hResult = pDInputJoyConfig->GetConfig(pDIGameCntrlPropSheet->GetID(), &JoyConfig, DIJC_GUIDINSTANCE);
	if(S_OK == hResult)
	{
		SetConfig(pJoyConfig, DIJC_REGHWCONFIGTYPE, JoyConfig.guidInstance, pDIGameCntrlPropSheet->GetID());
	}
}
*/
//2003.02.27删除结蕘E
//2003.02.27铁赜开始
BOOL Calibration(LPDIJOYCONFIG pJoyConfig, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet)
{
	HRESULT hResult;

	LPDIRECTINPUTDEVICE2 pDInputDevice2 = NULL;
	pDIGameCntrlPropSheet->GetDevice(&pDInputDevice2);
	hResult = pDInputDevice2->Acquire();
	if((DI_OK != hResult) && (S_FALSE != hResult))
	{
		return FALSE;
	}

	LPDIRECTINPUTJOYCONFIG pDInputJoyConfig = NULL;
	pDIGameCntrlPropSheet->GetJoyConfig(&pDInputJoyConfig);
	DIJOYCONFIG JoyConfig;
	JoyConfig.dwSize = sizeof(DIJOYCONFIG);
	hResult = pDInputJoyConfig->GetConfig(pDIGameCntrlPropSheet->GetID(), &JoyConfig, DIJC_GUIDINSTANCE);
	if(S_OK == hResult)
	{
		SetConfig(pJoyConfig, DIJC_REGHWCONFIGTYPE, JoyConfig.guidInstance, pDIGameCntrlPropSheet->GetID());
	}

	return TRUE;
}
//2003.02.27铁赜结蕘E
/////////////////////////////////////////////////////////////////////////////

