// DRSK.cpp
// 功能:游戏杆控制面板DLL入口和两个必须导出的接口
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DRSK.h"
#include "MyIClassFactory.h"
#include "Attribute.h"
/////////////////////////////////////////////////////////////////////////////
HINSTANCE				g_hInst				= NULL;
CRITICAL_SECTION		g_CriticalSection;
LONG					g_DLLRefCount		= 0;
//standard self-registration table
const TCHAR * g_RegTable[][3]={
	{JMROOTCLSID,0,_T("")},
	{JMCLSID,0,(const TCHAR * )-1 /*表示文件名的值*/},
	{JMCLSID,_T("ThreadingModel"),_T("Both")}
};
/////////////////////////////////////////////////////////////////////////////
//游戏杆控制面板DLL入口
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  Reason, LPVOID Reserved)
{
	switch(Reason)
	{
	case DLL_PROCESS_ATTACH:
		g_hInst = (HINSTANCE)(hModule);
		InitializeCriticalSection(&g_CriticalSection);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		DeleteCriticalSection(&g_CriticalSection);
		break;
    }
	
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, PVOID* ppv)
{
	HRESULT	hResult = E_NOTIMPL;

	CMyIClassFactory* pMyIClassFactory = NULL;
	*ppv = NULL;

	//if(FALSE == IsEqualCLSID(rclsid, CLSID_GAJOYPS))
	if(FALSE == IsEqualCLSID(rclsid, CLSID_FVACP))
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	if(FALSE == IsEqualIID(riid, IID_IClassFactory))
	{
		return E_NOINTERFACE;
	}

	pMyIClassFactory = new CMyIClassFactory();
	if(NULL == pMyIClassFactory)
	{
		return E_OUTOFMEMORY;
	}
	

	hResult = pMyIClassFactory->QueryInterface(riid, ppv);

	pMyIClassFactory->Release();

    return hResult;
}
/////////////////////////////////////////////////////////////////////////////
//
STDAPI DllCanUnloadNow()
{
	if(0 == g_DLLRefCount)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}
/////////////////////////////////////////////////////////////////////////////
STDAPI DllRegisterServer(void)
{
	HRESULT hr=S_OK;
	TCHAR szFileName [MAX_PATH];
	::GetModuleFileName(g_hInst,szFileName,MAX_PATH);
	
	int nEntries=sizeof(g_RegTable)/sizeof(*g_RegTable);
	for(int i =0;SUCCEEDED(hr)&&i<nEntries;i++)
	{
		const TCHAR * pszKeyName=g_RegTable[i][0];
		const TCHAR * pszValueName=g_RegTable[i][1];
		const TCHAR * pszValue=g_RegTable[i][2];
		
		if(pszValue==(const TCHAR *)-1)
		{
			pszValue=szFileName;
		}
		
		HKEY hkey;
		long err=::RegCreateKey(HKEY_CLASSES_ROOT,pszKeyName,&hkey);
		if(err==ERROR_SUCCESS)
		{
			err=::RegSetValueEx( hkey,
				pszValueName,
				0,
				REG_SZ,
				( const BYTE*)pszValue,
				( _tcslen(pszValue)+1 ) );
			::RegCloseKey(hkey);
		}
		if(err!=ERROR_SUCCESS)
		{
			::DllUnregisterServer();
			hr=E_FAIL;
		}
		
	}
	return hr;
}

STDAPI DllUnregisterServer(void)
{
	HRESULT hr=S_OK;
	TCHAR szFileName [MAX_PATH];
	::GetModuleFileName(g_hInst,szFileName,MAX_PATH);
	
	int nEntries=sizeof(g_RegTable)/sizeof(*g_RegTable);
	for(int i =(nEntries-1);SUCCEEDED(hr)&&i>=0;i--)
	{
		const TCHAR * pszKeyName=g_RegTable[i][0];
		long err=::RegDeleteKey(HKEY_CLASSES_ROOT,pszKeyName);
		if(err!=ERROR_SUCCESS)
			hr=S_FALSE;
	}	
	return hr;
}