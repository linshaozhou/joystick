// ForceFeedback.cpp
// 功能:游戏控制面板属性页振动页脕E
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Resource.h"
#include "ForceFeedback.h"
#include <Stdio.h>
#include "Rumble.h"
#include "GetOsType.h"
#include <Mmsystem.h>
#include <Math.h>
//#include "debug.h"
extern "C" {
#include <HidSdi.h>
}
/////////////////////////////////////////////////////////////////////////////
extern HINSTANCE g_hInst;
HANDLE hDevice = NULL;
LPDIRECTINPUTEFFECT	pDInputEffect = NULL;
UINT ForceFeedbackConnect = 0;
int    iForceFlag=0;
DWORD  dwLastEffectTime =0; 
bool  bEffectOver=false;
DWORD OS=WINDOWS_98;
UCHAR g_ReportID=0; 
/////////////////////////////////////////////////////////////////////////////
HRESULT SendRumbleData(HANDLE hDevice, UCHAR ReportID, int x, int y);
//振动页面窗口过程
BOOL CALLBACK ForceFeedbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CDIGameCntrlPropSheet *pDIGameCntrlPropSheet = (CDIGameCntrlPropSheet*)GetWindowLong(hWnd, 8);
	static LPDIRECTINPUTDEVICE2 pDInputDevice2 = NULL;
	HWND EffectSliderWnd = NULL;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			pDIGameCntrlPropSheet = (CDIGameCntrlPropSheet*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, 8, (LPARAM)pDIGameCntrlPropSheet);
			if(FAILED(InitDInput(GetParent(hWnd), pDIGameCntrlPropSheet)))
			{
		        return TRUE;
			}
			OS=GetOsType();
			pDIGameCntrlPropSheet->GetDevice(&pDInputDevice2);
			EffectSliderWnd = GetDlgItem(hWnd, IDC_SLIDER_EFFECT);
			SendMessage(EffectSliderWnd, TBM_SETRANGE, TRUE, MAKELONG(0, 1200));
			if (NULL!=hDevice) {
				CloseHandle(hDevice);
				hDevice = GetDeviceHandle(pDInputDevice2);

				PHIDP_PREPARSED_DATA HidPreparsedData;
				HidD_GetPreparsedData(hDevice, &HidPreparsedData);
				
				HIDP_VALUE_CAPS OutCaps[4];
				ZeroMemory(OutCaps, sizeof(HIDP_VALUE_CAPS) * 4);
				USHORT OutCapsNum = 4;
				HidP_GetValueCaps(HidP_Output, OutCaps, &OutCapsNum, HidPreparsedData);
				
				g_ReportID = OutCaps[0].ReportID;
				
				HidD_FreePreparsedData(HidPreparsedData);
			}
			
		}
		iForceFlag=0;
		dwLastEffectTime=0;
		bEffectOver=false;
		break;
	case WM_DESTROY:
		 pDInputDevice2->Unacquire();
		 KillTimer(hWnd, ID_FF_TIMER);
		 //FreeEffect();
		 if (NULL!=hDevice) {
			 SendRumbleData(hDevice,g_ReportID, 0,0);
			 	CloseHandle(hDevice);
		        hDevice = NULL;
		 }
		 ForceFeedbackConnect = 0;
		 iForceFlag=0;
		 dwLastEffectTime=0;
		 bEffectOver=false;
		 break;
	case WM_TIMER:
		 DisplayForceFeedbackState(pDIGameCntrlPropSheet, hWnd);
		 break;
	case WM_HSCROLL:
		{
	     HKEY JoystickKey;
		 TCHAR KeyName[256];
		 EffectSliderWnd = GetDlgItem(hWnd, IDC_SLIDER_EFFECT);
		 long Effect = SendMessage(EffectSliderWnd, TBM_GETPOS, 0, 0);	
		 _stprintf(KeyName, _T("SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\OEM\\VID_%04X&PID_%04X"), VID, PID);
		 RegOpenKeyEx(HKEY_CURRENT_USER, KeyName,0,KEY_READ|KEY_WRITE, &JoystickKey);
		 RegSetValueEx(JoystickKey, _T("Amplify"), 0, REG_DWORD, (UCHAR*)(&Effect), sizeof(Effect));
		 TCHAR cListPre[25]={_T("\0")};
		 float  fListPre=(float)(Effect/1200.0)*100;
		 int    iListPre=(int)fListPre;
		 _stprintf(cListPre,_T("%d%c"),iListPre,_T('%'));
		 SetDlgItemText(hWnd,IDC_STATICLISTPRE,cListPre);
		 
		 RegCloseKey(JoystickKey);
		}
		
         break;		 
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case PSN_SETACTIVE:
			{
				pDIGameCntrlPropSheet->GetDevice(&pDInputDevice2);
				SetTimer(hWnd, ID_FF_TIMER, FF_TIMER_ELAPSE, NULL);
			//	FreeEffect();
			//	InitEffect(hWnd, pDInputDevice2);
				HKEY JoystickKey;
				LONG Effect;
				DWORD Type = REG_DWORD;
				DWORD DataSize = sizeof(Effect);
				TCHAR KeyName[256];
				_stprintf(KeyName, _T("SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\OEM\\VID_%04X&PID_%04X"), VID, PID);
				RegOpenKeyEx(HKEY_CURRENT_USER, KeyName,0,KEY_READ, &JoystickKey);
				RegQueryValueEx(JoystickKey, _T("Amplify"), 0, &Type, (UCHAR*)(&Effect), &DataSize);
				//Add 04/05/07 START
				TCHAR cListPre[25]={_T("\0")};
				float  fListPre=(float)(Effect/1200.0)*100;
				int    iListPre=(int)fListPre;
				_stprintf(cListPre,_T("%d%c"),iListPre,_T('%'));
				SetDlgItemText(hWnd,IDC_STATICLISTPRE,cListPre);
				//Add 04/05/07 END 
		
				RegCloseKey(JoystickKey);
				EffectSliderWnd = GetDlgItem(hWnd, IDC_SLIDER_EFFECT);
				SendMessage(EffectSliderWnd, TBM_SETPOS, TRUE, Effect);
			}
			iForceFlag=0;
			dwLastEffectTime=0;
			bEffectOver=false;
			break;
		case PSN_KILLACTIVE:
			pDInputDevice2->Unacquire();
			KillTimer(hWnd, ID_FF_TIMER);
		//	FreeEffect();
			 if (NULL!=hDevice) {
			 SendRumbleData(hDevice,g_ReportID, 0,0);
			 	CloseHandle(hDevice);
		        hDevice = NULL;
		 }
			iForceFlag=0;
		    dwLastEffectTime=0;
			bEffectOver=false;
			break;
		}
		return TRUE;
	case WM_COMMAND:
		{
			WORD ID = LOWORD(wParam);
			WORD NotifyCode = HIWORD(wParam);
			switch(NotifyCode)
			{
			case BN_CLICKED:
				{
					EffectSliderWnd = GetDlgItem(hWnd, IDC_SLIDER_EFFECT);
					long Effect = SendMessage(EffectSliderWnd, TBM_GETPOS, 0, 0);
					switch(ID)
					{
					case IDC_BUTTON_APPLY:
						HKEY JoystickKey;
						TCHAR KeyName[256];
						_stprintf(KeyName, _T("SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\OEM\\VID_%04X&PID_%04X"), VID, PID);
						if(RegOpenKey(HKEY_LOCAL_MACHINE, KeyName, &JoystickKey) == ERROR_SUCCESS)
						{
							RegSetValueEx(JoystickKey, _T("Amplify"), 0, REG_DWORD, (UCHAR*)(&Effect), sizeof(Effect));
							if(pDInputEffect)
							{
								pDInputEffect->Start(1, 0);
							}
						}
						RegCloseKey(JoystickKey);
						break;
					case IDC_BUTTON_LFORCE:
						 iForceFlag=1;
						 bEffectOver=false;
						 dwLastEffectTime=timeGetTime();
						 if(pDInputEffect)
							{
								pDInputEffect->Start(1, 0);
							}
						 break;
					case IDC_BUTTON_BFORCE:
						 iForceFlag=2;
						 bEffectOver=false;
						 dwLastEffectTime=timeGetTime();
						 if(pDInputEffect)
							{
								pDInputEffect->Start(1, 0);
							}
						 break;
					case IDC_BUTTON_RFORCE:
						 iForceFlag=3;
						 bEffectOver=false;
						 dwLastEffectTime=timeGetTime();
						 if(pDInputEffect)
							{
								pDInputEffect->Start(1, 0);
							}
						 break;
					case IDC_BUTTON_STOP:
						 iForceFlag=0;
						 bEffectOver=false;
						 dwLastEffectTime=0;
						 break;
					}
				}
				break;//BN_CLICK END
			}	
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////
//初始化振动
void InitEffect(HWND hWnd, LPDIRECTINPUTDEVICE2 pDInputDevice2)
{
	HRESULT hResult;

	DIEFFECT Effect;
	DWORD Axes[2] = {DIJOFS_X, DIJOFS_Y};
	LONG Direction[2] = {0, 0};
	DICONSTANTFORCE ConstantForce = {0};
	Effect.dwSize					= sizeof(DIEFFECT);
    Effect.dwFlags					= DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    Effect.dwDuration				= INFINITE;
    Effect.dwSamplePeriod			= 0;
    Effect.dwGain					= DI_FFNOMINALMAX;
    Effect.dwTriggerButton			= DIEB_NOTRIGGER;
    Effect.dwTriggerRepeatInterval	= 0;
    Effect.cAxes					= 2;
    Effect.rgdwAxes					= Axes;
    Effect.rglDirection				= Direction;
    Effect.lpEnvelope				= 0;
    Effect.cbTypeSpecificParams		= sizeof(DICONSTANTFORCE);
    Effect.lpvTypeSpecificParams	= &ConstantForce;
    hResult = pDInputDevice2->CreateEffect(GUID_ConstantForce, &Effect, &pDInputEffect, NULL);

    if(FAILED(hResult) != TRUE)
    {
		pDInputEffect->Start(1, 0);
    }
	else
	{
		KillTimer(hWnd, ID_FF_TIMER);
	}
}
/////////////////////////////////////////////////////////////////////////////
//释放振动
void FreeEffect()
{
	if(NULL != pDInputEffect)
	{
		pDInputEffect->Release();
		pDInputEffect = NULL;
	}
}
/////////////////////////////////////////////////////////////////////////////
//设置震动强度
HRESULT SetForceXY(LPDIRECTINPUTEFFECT pDInputEffect, int x, int y)
{
	HRESULT hResult;

	LONG Direction[2] = {x, y};

	DICONSTANTFORCE ConstantForce;
    ConstantForce.lMagnitude = (DWORD)sqrt((double)x * (double)x + (double)y * (double)y);

    DIEFFECT Effect;
    Effect.dwSize					= sizeof(DIEFFECT);
    Effect.dwFlags					= DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    Effect.cAxes					= 2;
    Effect.rglDirection				= Direction;
    Effect.lpEnvelope				= 0;
    Effect.cbTypeSpecificParams		= sizeof(DICONSTANTFORCE);
    Effect.lpvTypeSpecificParams	= &ConstantForce;

    hResult = pDInputEffect->SetParameters(&Effect, DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS | DIEP_START);

    return hResult;
}
/////////////////////////////////////////////////////////////////////////////
//显示震动页脕E
HRESULT DisplayForceFeedbackState(CDIGameCntrlPropSheet* pDIGameCntrlPropSheet, HWND hWnd)
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
		if (NULL!=hDevice) {
			 SendRumbleData(hDevice,g_ReportID, 0,0);
			 	CloseHandle(hDevice);
		        hDevice = NULL;
		 }
		if(ForceFeedbackConnect != 2)
		{
			SetDlgItemText(hWnd, IDC_STATIC_DEVICESTATE_FORCEFEEDBACK, _T("僐儞僩儘乕儔乕偑愙懕偝傟偰偄傑偣傫丅") );
			//MessageBox(NULL,"Test","test title",MB_OK);
			ForceFeedbackConnect = 2;
            HWND hItem=GetDlgItem(hWnd,IDC_EFEEICON);
			ShowWindow(hItem,SW_SHOW);
			iForceFlag=0;
            dwLastEffectTime =0; 
            bEffectOver=false;
		}
		return S_OK;
	}
	else
	{
		if(ForceFeedbackConnect != 1)
		{
			SetDlgItemText(hWnd, IDC_STATIC_DEVICESTATE_FORCEFEEDBACK,_T("僐儞僩儘乕儔乕偑愙懕偝傟偰偄傑偡丅"));
			ForceFeedbackConnect = 1;
            HWND hItem=GetDlgItem(hWnd,IDC_EFEEICON);
			ShowWindow(hItem,SW_HIDE);
		}
	}

	DIJOYSTATE JoyState;
	int ForceX=0,ForceY=0;
	int iCountX1=0,iCountY1=0;
	int iCountX2=0,iCountY2=0;
	static int LastiCountX2 = 0;
	static int LastiCountY2 = 0;
    DWORD  dwCurrentEffectTime=0 ; 
	
	hResult = pDInputDevice2->GetDeviceState(sizeof(DIJOYSTATE), &JoyState);
	
	if(DI_OK == hResult)
	{
		iCountX2 = (int)((JoyState.lX - 32768) / 32768.0 * 10000);
		iCountY2 = (int)((JoyState.lY - 32768) / 32768.0 * 10000);

		if(iForceFlag>=1&&iForceFlag<=3&&bEffectOver==false)
		{
		   dwCurrentEffectTime=timeGetTime();
		   if((dwCurrentEffectTime-dwLastEffectTime) > 3800 )		   
		   {
			   bEffectOver=true;  
			   iForceFlag=0;
			   dwLastEffectTime=0;
		   }
		}
		
		switch(iForceFlag)
		{
	      case 1://left
			     iCountX1=10000;
                 iCountY1=0;
		  	     break;
	      case 2://both
                 iCountX1=10000;
                 iCountY1=10000;
			     break;
		  case 3://right
			     iCountX1=0;
                 iCountY1=10000;
			     break;	
		  case 0://stop
			     iCountX1=0;
                 iCountY1=0;
				 break;
		}

       ForceX=0;ForceY=0;
	   
	   /*if(iCountX2!=0||iCountY2!=0)
	   {
		ForceX=iCountX2;
		ForceY=iCountY2;
		
		iForceFlag=0;
		dwLastEffectTime=0;
		bEffectOver=false;
	   }
	   else
	   {
		 ForceX=iCountX1;
		 ForceY=iCountY1;	
	   }*/

	   if(iCountX1!=0||iCountY1!=0)
	   {
		ForceX=iCountX1;
		ForceY=iCountY1;		
	   }
	   else
	   {
		 ForceX=iCountX2;
		 ForceY=iCountY2;	

		 iForceFlag=0;
		 dwLastEffectTime=0;
		 bEffectOver=false;
	   }

	   if(pDInputEffect)
		{
		   pDInputEffect->Start(1, 0);
		}
	   //04.09.14 Start
	   if(abs(ForceX)<2500) ForceX=0;
	   if(abs(ForceY)<2500) ForceY=0;
       //04.09.14 end
	    if (NULL==hDevice) {
		   hDevice=GetDeviceHandle(pDInputDevice2);
		   PHIDP_PREPARSED_DATA HidPreparsedData;
		   HidD_GetPreparsedData(hDevice, &HidPreparsedData);
		   
		   HIDP_VALUE_CAPS OutCaps[4];
		   ZeroMemory(OutCaps, sizeof(HIDP_VALUE_CAPS) * 4);
		   USHORT OutCapsNum = 4;
		   HidP_GetValueCaps(HidP_Output, OutCaps, &OutCapsNum, HidPreparsedData);
		   
		   g_ReportID = OutCaps[0].ReportID;
		   
		   HidD_FreePreparsedData(HidPreparsedData);
	   }
	   SendRumbleData(hDevice,g_ReportID,ForceY,ForceX);
	
	   //SetForceXY(pDInputEffect, abs(ForceX), abs(ForceY));       	   
   }

	if((iCountX2 != LastiCountX2) || (iCountY2 != LastiCountY2))
	{
		if(iCountX2<=(-10000+1250)) iCountX2=-10000;
		if(iCountX2>=(10000-1250))  iCountX2=10000;
		if(iCountY2<=(-10000+1250)) iCountY2=-10000;
		if(iCountY2>=(10000-1250))  iCountY2=10000;
		SetRumbleState(hWnd, IDC_RUMBLE, iCountX2, iCountY2);
	}
	
	LastiCountX2 = iCountX2;
	LastiCountY2 = iCountY2;


	/*DIJOYSTATE JoyState;
	static int LastForceX = 0;
	static int LastForceY = 0;
	int ForceX;
	int ForceY;

	hResult = pDInputDevice2->GetDeviceState(sizeof(DIJOYSTATE), &JoyState);
	if(DI_OK == hResult)
	{
		ForceX = (int)((JoyState.lX - 32768) / 32768.0 * 10000);
		ForceY = (int)((JoyState.lY - 32768) / 32768.0 * 10000);
		SetForceXY(pDInputEffect, abs(ForceX), abs(ForceY));
	}
	if((ForceX != LastForceX) || (ForceY != LastForceY))
	{
		if(ForceX<=(-10000+1250)) ForceX=-10000;
		if(ForceX>=(10000-1250))  ForceX=10000;
		if(ForceY<=(-10000+1250)) ForceY=-10000;
		if(ForceY>=(10000-1250))  ForceY=10000;
		SetRumbleState(hWnd, IDC_RUMBLE, ForceX, ForceY);
	}
	
	LastForceX = ForceX;
	LastForceY = ForceY;*/
//
/*	char szBuffer[1024];
	sprintf(szBuffer, "x=%d", iCountX2);
	SetDlgItemText(hWnd, IDC_STATICFX, szBuffer);
	sprintf(szBuffer, "y=%d", iCountY2);
	SetDlgItemText(hWnd, IDC_STATICFY, szBuffer);*/
//
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////

HRESULT SendRumbleData(HANDLE hDevice, UCHAR ReportID, int x, int y)
{
//	//TRACE("SendRumbleData \n");
	HKEY JoystickKey;
	TCHAR KeyName[256];
	DWORD dwAmplify, dwType,dwSize;
	dwType=REG_DWORD;
	dwSize=sizeof(DWORD);
	_stprintf(KeyName, _T("SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\OEM\\VID_%04X&PID_%04X"), VID, PID);
	if(RegOpenKeyEx(HKEY_CURRENT_USER, KeyName,0,KEY_READ,  &JoystickKey) == ERROR_SUCCESS)
	{//TRACE("RegOpenKeyEx OK\n");
		RegQueryValueEx(JoystickKey, _T("Amplify"), 0, &dwType, (UCHAR*)(&dwAmplify), &dwSize);	
		RegCloseKey(JoystickKey);
	}
	float fAmplify=dwAmplify/1200.0;
	x=fabs((double)x);
	y=fabs((double)y);
	x=255*fAmplify*x/10000.0;
	y=255*fAmplify*y/10000.0;
	//TRACE("x=%f\n",x);	
	static bool IsFirstZero=true;

	BYTE Output[8];

	DWORD BytesTxd = 0;

	//04.06.17 Add Start	
	if(x == 0 && y ==0) 
	{
		if(IsFirstZero==true)
		{			 
			Output[0] = ReportID;
			Output[1] = 0xFA;
			Output[2] = 0xFE;
			Output[3] = 0x00;
			Output[4] = 0x00;
			Output[5] = 0x00;
			Output[6] = 0x00;
			Output[7] = 0x00;
			WriteFile(hDevice, Output, 8, &BytesTxd, NULL);

			if(BytesTxd != 8) 
			{
				//TRACE("WriteFile faild 1\n");
				return GetLastError();
			}

		}

		Output[0] = ReportID;
		Output[1] = 0xF3;
		Output[2] = 0x00;
		Output[3] = 0x00;
		Output[4] = 0x00;
		Output[5] = 0x00;
		Output[6] = 0x00;
		Output[7] = 0x00;
		WriteFile(hDevice, Output, 8, &BytesTxd, NULL);

		if(BytesTxd != 8) 
		{ //TRACE("WriteFile faild 2\n");
			return GetLastError();
		}
		IsFirstZero=false;
		return S_OK;
	}
	else
		IsFirstZero=true;
	//04.06.17 Add End

	Output[0] = ReportID;
	Output[1] = 0x51;
	Output[2] = 0x00;
	Output[3] = (BYTE)x;
	Output[4] = 0x00;
	Output[5] = (BYTE)y;
	Output[6] = 0x00;
	Output[7] = 0x00;
	WriteFile(hDevice, Output, 8, &BytesTxd, NULL);

	if(BytesTxd != 8)
	{ //TRACE("WriteFile faild 3 %d\n",GetLastError());
		return GetLastError();
	}

	BytesTxd = 0;
	Output[0] = ReportID;
	Output[1] = 0xFA;
	Output[2] = 0xFE;
	Output[3] = 0x00;
	Output[4] = 0x00;
	Output[5] = 0x00;
	Output[6] = 0x00;
	Output[7] = 0x00;
	WriteFile(hDevice, Output, 8, &BytesTxd, NULL);

	if(BytesTxd != 8) {
		//TRACE("WriteFile faild 4\n");
		return GetLastError();
	}

	return S_OK;
}

