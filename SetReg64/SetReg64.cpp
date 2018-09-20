// SetReg64.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Shellapi.h>
void ReplayFile(HINSTANCE hInstance)
{		
	TCHAR SzNewFile[MAX_PATH];
	TCHAR SzExistFile[MAX_PATH];
	ZeroMemory(SzNewFile,sizeof(TCHAR)*MAX_PATH);
	ZeroMemory(SzExistFile,sizeof(TCHAR)*MAX_PATH);
	GetModuleFileDir(hInstance,SzNewFile,MAX_PATH);

	lstrcpy(SzExistFile,SzNewFile);
	lstrcat(SzNewFile,"GameControlerl.exe");
	lstrcat(SzExistFile,"GameControler_64l.exe");
	CopyFile(SzExistFile,SzNewFile,FALSE);
	
// 	ZeroMemory(SzNewFile,sizeof(TCHAR)*MAX_PATH);
// 	ZeroMemory(SzExistFile,sizeof(TCHAR)*MAX_PATH);
// 	GetModuleFileDir(hInstance,SzNewFile,MAX_PATH);
// 	lstrcpy(SzExistFile,SzNewFile);
// 	lstrcat(SzNewFile,"USB Gamepad.exe");
// 	lstrcat(SzExistFile,"USB Gamepad64.exe");
// 	CopyFile(SzExistFile,SzNewFile,FALSE);
}
//////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle("kernel32"),"IsWow64Process");
 
BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;
 
    if (NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            // handle error
        }
    }
    return bIsWow64;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
///
///主程序
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	
	if(IsWow64())
	{		
 		//ReplayFile(hInstance);
		OSVERSIONINFO osvi;
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx (&osvi);
		bool bIsWinNT=(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT);		
		TCHAR zddllfilepath[MAX_PATH],jmdllfilepath[MAX_PATH];//注册振动和测试界面dll
		memset(zddllfilepath,0,sizeof(zddllfilepath));
		memset(jmdllfilepath,0,sizeof(jmdllfilepath));
		strcpy(zddllfilepath,"\"");//作为参数路径要加引号，否则不能有空格
		GetModuleFileDir(hInstance,zddllfilepath,MAX_PATH);
		strcpy(jmdllfilepath,zddllfilepath);
		strcat(zddllfilepath,"EZFRD64.DLL\" ");//作为参数路径要加引号，否则不能有空格
		strcat(jmdllfilepath,"FCVAP64.dll\" ");
		strcat(zddllfilepath,lpCmdLine);
		strcat(jmdllfilepath,lpCmdLine);
		if (bIsWinNT&&6<=osvi.dwMajorVersion)//vista
		{
			ShellExecute(NULL,"runas","regsvr32.exe",zddllfilepath,NULL,SW_NORMAL);
			ShellExecute(NULL,"runas","regsvr32.exe",jmdllfilepath,NULL,SW_NORMAL);
		}else
		{
			ShellExecute(NULL,"open","regsvr32.exe",zddllfilepath,NULL,SW_NORMAL);
			ShellExecute(NULL,"open","regsvr32.exe",jmdllfilepath,NULL,SW_NORMAL);
		}
	}// else
// 	{
// 		
// 		OSVERSIONINFO osvi;
// 		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
// 		GetVersionEx (&osvi);
// 		bool bIsWinNT=(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT);		
// 		TCHAR zddllfilepath[MAX_PATH],jmdllfilepath[MAX_PATH];//注册振动和测试界面dll
// 		memset(zddllfilepath,0,sizeof(zddllfilepath));
// 		memset(jmdllfilepath,0,sizeof(jmdllfilepath));
// 		strcpy(zddllfilepath,"\"");//作为参数路径要加引号，否则不能有空格
// 		GetModuleFileDir(hInstance,zddllfilepath,MAX_PATH);
// 		strcpy(jmdllfilepath,zddllfilepath);
// 		strcat(zddllfilepath,"EZFRD32.DLL\" ");//作为参数路径要加引号，否则不能有空格
// 		strcat(jmdllfilepath,"FCVAP32.dll\" ");
// 		strcat(zddllfilepath,lpCmdLine);
// 		strcat(jmdllfilepath,lpCmdLine);
// 		if (bIsWinNT&&6<=osvi.dwMajorVersion)//vista
// 		{
// 			ShellExecute(NULL,"runas","regsvr32.exe",zddllfilepath,NULL,SW_NORMAL);
// // 			ShellExecute(NULL,"runas","regsvr32.exe",jmdllfilepath,NULL,SW_NORMAL);
// 		}else
// 		{
// 			ShellExecute(NULL,"open","regsvr32.exe",zddllfilepath,NULL,SW_NORMAL);
// // 			ShellExecute(NULL,"open","regsvr32.exe",jmdllfilepath,NULL,SW_NORMAL);
// 		}
// 	}
	return 0;
}

VOID GetModuleFileDir(HINSTANCE hInstance,TCHAR* FileDirname, DWORD nSize)
{
	//	GetModuleFileName(AfxGetInstanceHandle(),FileDirname.GetBufferSetLength(nSize),nSize);	
	// 	int nPos;   
	// 	nPos=FileDirname.ReverseFind(_T('\\'));   
	// 	FileDirname=FileDirname.Left(nPos);	
	TCHAR   szFullPath[MAX_PATH]={0};
	TCHAR   szDrive[MAX_PATH]={0};
	TCHAR	SzDir[_MAX_DIR]={0};
	GetModuleFileName(hInstance,szFullPath,MAX_PATH);  
	_splitpath(szFullPath,szDrive,SzDir,NULL,NULL);
	strcat(szDrive,SzDir);
	strcat(FileDirname,szDrive);
}




