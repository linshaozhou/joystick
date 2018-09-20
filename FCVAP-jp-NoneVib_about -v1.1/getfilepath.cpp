#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
VOID GetModuleFileDir(HINSTANCE hInstance,TCHAR* FileDirname, DWORD nSize)
{
	//	GetModuleFileName(AfxGetInstanceHandle(),FileDirname.GetBufferSetLength(nSize),nSize);	
	// 	int nPos;   
	// 	nPos=FileDirname.ReverseFind(_T('\\'));   
	// 	FileDirname=FileDirname.Left(nPos);	
	TCHAR   szFullPath[MAX_PATH];
	TCHAR   szDrive[_MAX_DRIVE];
	TCHAR	SzDir[_MAX_DIR];
	GetModuleFileName(hInstance,szFullPath,MAX_PATH);  
	_tsplitpath(szFullPath,szDrive,SzDir,NULL,NULL);
	_tcscat(szDrive,SzDir);
	_tcscat(FileDirname,szDrive);
}
