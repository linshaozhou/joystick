#ifndef __GETFILEPATH_H__
#define __GETFILEPATH_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
VOID GetModuleFileDir(HINSTANCE hInstance,TCHAR* FileDirname, DWORD nSize);
#endif // __GETFILEPATH_H__