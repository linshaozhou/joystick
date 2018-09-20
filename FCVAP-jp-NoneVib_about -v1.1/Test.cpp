#include "StdAfx.h"
#include "Resource.h"
#include "Test.h"
#include <shellapi.h>
#include "Lamp.h"
#include "Axes.h"
#include "Pov.h"
#include "getfilepath.h"
UINT TestConnect = 0;
#define GWL_USERDATA        (-21)
#define DWL_USER        8
extern HINSTANCE g_hInst;
BOOL CALLBACK TestProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CDIGameCntrlPropSheet* pDIGameCntrlPropSheet = (CDIGameCntrlPropSheet*)GetWindowLong(hWnd, DWL_USER);

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
			for(int i = 0; i < BUTTON_NUMBER; i ++)
			{
				SetLampState(hWnd, IDC_BUTTON1 + i, RGB(150, 0, 0));

			}
		}
		break;
	//case WM_COMMAND:
	//	{
	//		if (HIWORD(wParam)==BN_CLICKED&&LOWORD(wParam)==IDC_SETTING) {
	//			TCHAR szFileName[MAX_PATH];
	//			ZeroMemory(szFileName,sizeof(TCHAR)*MAX_PATH);
	//			GetModuleFileDir(g_hInst,szFileName,sizeof(TCHAR)*MAX_PATH);		
	//			_tcscat(szFileName,_T("JoyToKey_en\\JoyToKey.exe"));
	//			ShellExecute(hWnd,_T("open"),szFileName,NULL,NULL,SW_SHOW);
	//			exit(0);
	//		}
	//	}
	//	
	//	break;
	case WM_DESTROY:
		KillTimer(hWnd, ID_TEST_TIMER);
		TestConnect = 0;
		break;
	case WM_TIMER:
		DisplayTestState(hWnd, pDIGameCntrlPropSheet);
		
		break;
	case WM_NOTIFY:
        switch(((LPNMHDR)lParam)->code)
		{
		case PSN_SETACTIVE:
			SetTimer(hWnd, ID_TEST_TIMER, TEST_TIMER_ELAPSE, NULL);
			break;
		case PSN_KILLACTIVE:
			KillTimer(hWnd, ID_TEST_TIMER);
			break;
        }
		return TRUE;
	}
    
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HRESULT DisplayTestState(HWND hWnd, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet)
{
	HRESULT hResult;

	if(IsBadReadPtr((void*)pDIGameCntrlPropSheet, sizeof(CDIGameCntrlPropSheet)))
	{
		return E_POINTER;
	}

	LPDIRECTINPUTDEVICE2 pDInputDevice2 = NULL;
	pDIGameCntrlPropSheet->GetDevice(&pDInputDevice2);

	hResult = pDInputDevice2->Acquire();
	if((DI_OK != hResult) && (S_FALSE != hResult))
	{
		if(TestConnect != 2)
		{
			SetDlgItemText(hWnd, IDC_STATIC_DEVICESTATE_TEST,_T("コントローラーが接続されていません。"));
            HWND hItem=GetDlgItem(hWnd,IDC_TESTICON);
			ShowWindow(hItem,SW_SHOW);
			TestConnect = 2;
		}
		return S_OK;
	}
	else
	{
		if(TestConnect != 1)
		{
			SetDlgItemText(hWnd, IDC_STATIC_DEVICESTATE_TEST, _T("コントローラーが接続されています。") );
            HWND hItem=GetDlgItem(hWnd,IDC_TESTICON);
			ShowWindow(hItem,SW_HIDE);
			TestConnect = 1;
		}
	}
	
	DIJOYSTATE JoyState;
	static DIJOYSTATE OldJoyState;
	hResult = pDInputDevice2->GetDeviceState(sizeof(DIJOYSTATE), &JoyState);
	if(DI_OK == hResult)
	{
		for(int i = 0; i < BUTTON_NUMBER; i ++)
		{
			if(OldJoyState.rgbButtons[i] != JoyState.rgbButtons[i])
			{
				int R = (JoyState.rgbButtons[i] > 0) ? 255 : 150;
//				int R = (JoyState.rgbButtons[i] > 0) ? 255 : 64;
				SetLampState(hWnd, IDC_BUTTON1 + i, RGB(R, 0, 0));
			}
		}
		
		if((OldJoyState.lX != JoyState.lX) || (OldJoyState.lY != JoyState.lY) )
		{
			SetAxesState(hWnd, IDC_LAXIS, JoyState.lX, JoyState.lY);
		}
		//ADD 03/12/08 START
		if((OldJoyState.lZ != JoyState.lZ) || (OldJoyState.lRz != JoyState.lRz) )
		{
			SetAxesState(hWnd, IDC_RAXIS, JoyState.lZ, JoyState.lRz);
		}
        //ADD END 03/12/08 END
		if(OldJoyState.rgdwPOV[0] != JoyState.rgdwPOV[0])
		{
			SetPovState(hWnd, IDC_POV, JoyState.rgdwPOV[0], 100, 100);
		}
	}
	CopyMemory(&OldJoyState, &JoyState, sizeof(DIJOYSTATE));
	
	return S_OK;
}

