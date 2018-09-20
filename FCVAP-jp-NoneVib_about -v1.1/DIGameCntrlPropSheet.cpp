// MyIClassFactory.cpp
// 功能:
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <InitGuid.h>
#include "Resource.h"
#include "DIGameCntrlPropSheet.h"

#include "About.h"
#include "Test.h"
#include "Calibration.h"
#include "ForceFeedback.h"
#include "Lamp.h"
#include "Axes.h"
#include "Pov.h"
#include "Rumble.h"
/////////////////////////////////////////////////////////////////////////////
DEFINE_GUID(IID_IDIGameCntrlPropSheet, 0x7854fb22, 0x8ee3, 0x11d0, 0xa1, 0xac, 0x0, 0x0, 0xf8, 0x2, 0x69, 0x77);

static PAGEDLG g_PageDlg[PAGES_NUMBER] = 
{
	{MAKEINTRESOURCEW(IDD_TEST), (DLGPROC)TestProc},
	{MAKEINTRESOURCEW(IDD_CALIBRATION), (DLGPROC)CalibrationProc},
	//{MAKEINTRESOURCEW(IDD_FORCEFEEDBACK), (DLGPROC)ForceFeedbackProc}//,
	//{MAKEINTRESOURCEW(IDD_ABOUTDLG), (DLGPROC)AboutProc} 
};
/////////////////////////////////////////////////////////////////////////////
extern HINSTANCE g_hInst;
extern LONG	g_DLLRefCount;
/////////////////////////////////////////////////////////////////////////////
//
CDIGameCntrlPropSheet::CDIGameCntrlPropSheet()
{
	m_DIGameCntrlPropSheetRefCount = 0;
	AddRef();

	m_JoystickID		= -1;
	m_pDInputDevice2	= NULL;
	m_pDInputJoyConfig	= NULL;
    
	InterlockedIncrement(&g_DLLRefCount);
}
/////////////////////////////////////////////////////////////////////////////
//
CDIGameCntrlPropSheet::~CDIGameCntrlPropSheet()
{
	if(NULL != m_pDInputDevice2)
	{
		m_pDInputDevice2->Unacquire();
		m_pDInputDevice2->Release();
		m_pDInputDevice2 = NULL;
	}

	if(NULL != m_pDInputJoyConfig)
	{
		m_pDInputJoyConfig->Unacquire();
		m_pDInputJoyConfig->Release();
		m_pDInputJoyConfig = NULL;
	}

	InterlockedDecrement(&g_DLLRefCount);
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CDIGameCntrlPropSheet::QueryInterface(REFIID riid, PVOID* ppv)
{
	*ppv = NULL;

	if(IsEqualIID(riid, IID_IUnknown))
    {
		*ppv = (LPUNKNOWN)(LPCDIGAMECNTRLPROPSHEET)this;
	}
	else
	{
		if(IsEqualIID(riid, IID_IDIGameCntrlPropSheet))
		{
			*ppv = (LPCDIGAMECNTRLPROPSHEET)this;
        }
		else
		{
			return E_NOINTERFACE;
		}
	}

	AddRef();
	
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(ULONG) CDIGameCntrlPropSheet::AddRef()
{   
	InterlockedIncrement((LPLONG)&m_DIGameCntrlPropSheetRefCount);
	return m_DIGameCntrlPropSheetRefCount;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(ULONG) CDIGameCntrlPropSheet::Release()
{
	ULONG ReturnValue = 0;
	
	InterlockedDecrement((LPLONG)&m_DIGameCntrlPropSheetRefCount);
	if(0 == m_DIGameCntrlPropSheetRefCount)
	{
		delete this;
		ReturnValue = 0;
	}
	else
	{
		ReturnValue = m_DIGameCntrlPropSheetRefCount;
	}
	
	return ReturnValue;
}
/////////////////////////////////////////////////////////////////////////////
//
HRESULT CDIGameCntrlPropSheet::Initialize()
{
	for(BYTE i = 0; i < PAGES_NUMBER; i ++)
	{
		m_PageInfo[i].dwSize		= sizeof(DIGCPAGEINFO);
		m_PageInfo[i].ProcFlag		= FALSE;
		m_PageInfo[i].PrePostProc	= NULL;
		m_PageInfo[i].IconFlag		= TRUE;
		//m_PageInfo[i].PageIcon		= MAKEINTRESOURCEW(IDI_FACTORY);		
		m_PageInfo[i].PageIcon		= MAKEINTRESOURCEW(IDI_WINLOGO);
		m_PageInfo[i].hInstance		= g_hInst;
		m_PageInfo[i].lParam		= (LPARAM)this;
		m_PageInfo[i].PageTitle		= NULL;
		m_PageInfo[i].PageProc		= g_PageDlg[i].PageProc;
		m_PageInfo[i].DlgTemplate	= g_PageDlg[i].DlgTemplate;
	}

	m_SheetInfo.dwSize			= sizeof(DIGCSHEETINFO);
	m_SheetInfo.PagesNumber		= PAGES_NUMBER;
	m_SheetInfo.SheetIconFlag	= TRUE;
	m_SheetInfo.SheetIcon		= MAKEINTRESOURCEW(IDI_WINLOGO);

//	LPTSTR BufferString = new(TCHAR[64]);
//	if(NULL == BufferString)
//	{
//		return E_OUTOFMEMORY;
//	}

	WCHAR* SheetCaption = (WCHAR*)LocalAlloc(LPTR, MAX_CAPTION_LENGTH * sizeof(WCHAR));
	if(NULL == SheetCaption)
    {
		return E_OUTOFMEMORY;
	}
//	LoadString(g_hInst, IDS_SHEETCAPTION, BufferString, MAX_CAPTION_LENGTH);
#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, SheetCaptionString, lstrlen(SheetCaptionString), SheetCaption, sizeof(WCHAR) * MAX_CAPTION_LENGTH);
#else
	lstrcpy(SheetCaption, SheetCaptionString);
#endif
//	if(BufferString)
//	{
//		delete[] (BufferString);
//	}
	m_SheetInfo.SheetCaption = SheetCaption;

	RegisterLampClass(g_hInst);
	RegisterAxesClass(g_hInst);
	RegisterPovClass(g_hInst);
	RegisterRumbleClass(g_hInst);

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CDIGameCntrlPropSheet::GetSheetInfo(LPDIGCSHEETINFO* pSheetInfo)
{
	*pSheetInfo = &m_SheetInfo;

    return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CDIGameCntrlPropSheet::GetPageInfo(LPDIGCPAGEINFO* pPageInfo)
{
	*pPageInfo = m_PageInfo;

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(USHORT) CDIGameCntrlPropSheet::GetID()
{
	return m_JoystickID;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CDIGameCntrlPropSheet::SetID(USHORT JoystickID)
{
	m_JoystickID = JoystickID;

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CDIGameCntrlPropSheet::GetDevice(LPDIRECTINPUTDEVICE2* pDInputDevice2)
{
	*pDInputDevice2 = m_pDInputDevice2;

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CDIGameCntrlPropSheet::SetDevice(LPDIRECTINPUTDEVICE2 pDInputDevice2)
{
	m_pDInputDevice2 = pDInputDevice2;

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CDIGameCntrlPropSheet::GetJoyConfig(LPDIRECTINPUTJOYCONFIG* pDInputJoyConfig)
{
	*pDInputJoyConfig = m_pDInputJoyConfig;

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CDIGameCntrlPropSheet::SetJoyConfig(LPDIRECTINPUTJOYCONFIG pDInputJoyConfig)
{
	m_pDInputJoyConfig = pDInputJoyConfig;

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
