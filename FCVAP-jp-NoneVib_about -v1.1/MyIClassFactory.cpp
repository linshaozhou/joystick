// MyIClassFactory.cpp
// 功能:
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "MyIClassFactory.h"
#include "DIGameCntrlPropSheet.h"
/////////////////////////////////////////////////////////////////////////////
extern LONG	g_DLLRefCount;
/////////////////////////////////////////////////////////////////////////////
//
CMyIClassFactory::CMyIClassFactory()
{
	m_MyIClassFactoryRefCount = 0;
	AddRef();

	InterlockedIncrement(&g_DLLRefCount);
}
/////////////////////////////////////////////////////////////////////////////
//
CMyIClassFactory::~CMyIClassFactory()
{
	InterlockedDecrement(&g_DLLRefCount);
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMyIClassFactory::QueryInterface(REFIID riid, PVOID* ppv)
{
	*ppv = NULL;

	if(IsEqualIID(riid, IID_IUnknown))
	{
		*ppv = (LPUNKNOWN)(LPCLASSFACTORY)this;
	}
	else
	{
		if(IsEqualIID(riid, IID_IClassFactory))
		{
			*ppv = (LPCLASSFACTORY)this;
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
STDMETHODIMP CMyIClassFactory::CreateInstance(LPUNKNOWN pUnknownOuter, REFIID riid, PVOID* Object)
{
	CDIGameCntrlPropSheet* pDIGameCntrlPropSheet = NULL;
	*Object = NULL;

	if(NULL != pUnknownOuter)
	{
		return CLASS_E_NOAGGREGATION;
	}

	pDIGameCntrlPropSheet = new CDIGameCntrlPropSheet();
	if(NULL == pDIGameCntrlPropSheet)
	{
		return E_OUTOFMEMORY;
    }

	HRESULT hResult = pDIGameCntrlPropSheet->Initialize();
	if(FAILED(hResult))
	{
		pDIGameCntrlPropSheet->Release();
		return hResult;
	}

    hResult = pDIGameCntrlPropSheet->QueryInterface(riid, Object);
	pDIGameCntrlPropSheet->Release();

    return hResult;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMyIClassFactory::LockServer(BOOL Lock)
{
	if(Lock)
	{
		InterlockedIncrement(&g_DLLRefCount);
	}
	else
	{
		InterlockedDecrement(&g_DLLRefCount);
	}

	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(ULONG) CMyIClassFactory::AddRef()
{
	InterlockedIncrement((LPLONG)&m_MyIClassFactoryRefCount);
	return m_MyIClassFactoryRefCount;
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(ULONG) CMyIClassFactory::Release()
{
	ULONG ReturnValue = 0;
	
	InterlockedDecrement((LPLONG)&m_MyIClassFactoryRefCount);
	if(0 == m_MyIClassFactoryRefCount)
	{
		delete this;
		ReturnValue = 0;
	}
	else
	{
		ReturnValue = m_MyIClassFactoryRefCount;
	}
	
	return ReturnValue;
}
/////////////////////////////////////////////////////////////////////////////

