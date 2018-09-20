// MyIClassFactory.h
// 功能:
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __DIGAMECNTRLPROPSHEET_H__
#define __DIGAMECNTRLPROPSHEET_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
#include "DInput.h"
#include "DInputD.h"
/////////////////////////////////////////////////////////////////////////////
#define PAGES_NUMBER		2
#define MAX_CAPTION_LENGTH	64
/////////////////////////////////////////////////////////////////////////////
typedef struct
{
	LPWSTR DlgTemplate;
	DLGPROC PageProc;
} PAGEDLG, *PPAGEDLG;
/////////////////////////////////////////////////////////////////////////////
typedef struct 
{
	DWORD		dwSize;
	LPWSTR		PageTitle;
	DLGPROC		PageProc;
	BOOL		ProcFlag;
	DLGPROC		PrePostProc;
	BOOL		IconFlag;
	LPWSTR		PageIcon;
	LPWSTR		DlgTemplate; 
	LPARAM		lParam;
	HINSTANCE	hInstance;
} DIGCPAGEINFO, *LPDIGCPAGEINFO;
/////////////////////////////////////////////////////////////////////////////
typedef struct 
{
	DWORD		dwSize;
	USHORT		PagesNumber;
	LPWSTR		SheetCaption;
	BOOL		SheetIconFlag;
	LPWSTR		SheetIcon;
} DIGCSHEETINFO, *LPDIGCSHEETINFO;
/////////////////////////////////////////////////////////////////////////////
DECLARE_INTERFACE_(IDIGameCntrlPropSheet, IUnknown)
{
	STDMETHOD(QueryInterface)	(THIS_ REFIID, PVOID*) PURE;
	STDMETHOD_(ULONG, AddRef)	(THIS) PURE;
	STDMETHOD_(ULONG,Release)	(THIS) PURE;

	STDMETHOD(GetSheetInfo)		(THIS_ LPDIGCSHEETINFO*) PURE;     
	STDMETHOD(GetPageInfo)		(THIS_ LPDIGCPAGEINFO*) PURE;  
	STDMETHOD(SetID)			(THIS_ USHORT) PURE;
};
/////////////////////////////////////////////////////////////////////////////
class CDIGameCntrlPropSheet : public IDIGameCntrlPropSheet
{
public:
	CDIGameCntrlPropSheet();
	~CDIGameCntrlPropSheet();

	virtual STDMETHODIMP			QueryInterface(REFIID riid, PVOID* ppv);
	virtual STDMETHODIMP_(ULONG)	AddRef();
	virtual STDMETHODIMP_(ULONG)	Release();

	virtual STDMETHODIMP			GetSheetInfo(LPDIGCSHEETINFO* pSheetInfo);
	virtual STDMETHODIMP			GetPageInfo (LPDIGCPAGEINFO* pPageInfo);
	virtual STDMETHODIMP			SetID(USHORT JoystickID);

	virtual STDMETHODIMP			Initialize();
	virtual STDMETHODIMP_(USHORT)	GetID();
	virtual STDMETHODIMP			GetDevice(LPDIRECTINPUTDEVICE2* pDInputDevice2);
	virtual STDMETHODIMP			SetDevice(LPDIRECTINPUTDEVICE2 pDInputDevice2);
	virtual STDMETHODIMP			GetJoyConfig(LPDIRECTINPUTJOYCONFIG* pDInputJoyConfig);
	virtual STDMETHODIMP			SetJoyConfig(LPDIRECTINPUTJOYCONFIG pDInputJoyConfig);

protected:
	DIGCSHEETINFO				m_SheetInfo;
	DIGCPAGEINFO				m_PageInfo[PAGES_NUMBER];
	LPDIRECTINPUTDEVICE2		m_pDInputDevice2;
	LPDIRECTINPUTJOYCONFIG		m_pDInputJoyConfig;

private:
	short	m_DIGameCntrlPropSheetRefCount;
	short	m_JoystickID;
};
typedef CDIGameCntrlPropSheet *LPCDIGAMECNTRLPROPSHEET;
/////////////////////////////////////////////////////////////////////////////
#endif // __DIGAMECNTRLPROPSHEET_H__