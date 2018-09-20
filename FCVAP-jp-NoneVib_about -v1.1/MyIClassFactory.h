// MyIClassFactory.h
// 功能:
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __MYICLASSFACTORY_H__
#define __MYICLASSFACTORY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
class CMyIClassFactory : public IClassFactory
{
protected:
	ULONG m_MyIClassFactoryRefCount;

public:
	CMyIClassFactory();
	~CMyIClassFactory();

	STDMETHODIMP			QueryInterface(REFIID riid, PVOID* ppv);
	STDMETHODIMP_(ULONG)	AddRef();
	STDMETHODIMP_(ULONG)	Release();

	STDMETHODIMP			CreateInstance(LPUNKNOWN pUnknownOuter, REFIID riid, PVOID* Object);
	STDMETHODIMP			LockServer(BOOL Lock);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __MYICLASSFACTORY_H__