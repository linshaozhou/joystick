// GetOsType.cpp
// 功能:获得Windows操作系统的类型
// 版本:1.2002.07.04
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GetOsType.h"
/////////////////////////////////////////////////////////////////////////////
//获得Windows操作系统的类型
DWORD GetOsType()
{
	DWORD OsType = 0;
	
	OSVERSIONINFO OsVersionInfo;
	OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(FALSE == GetVersionEx(&OsVersionInfo))
	{
		return OsType;
	}

	switch(OsVersionInfo.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		switch(OsVersionInfo.dwMajorVersion)
		{
		case 3:
		case 4:
			OsType = WINDOWS_NT;
			break;
		case 5:
			switch(OsVersionInfo.dwMinorVersion)
			{
			case 0:
				OsType = WINDOWS_2K;
				break;
			//!Modified by MH 2005*11*13		;
			//case 1:
			default:
			//end
				OsType = WINDOWS_XP;
				break;
			}
			break;
		case 6:
			OsType=WINDOWS_VISTA;
			break;
		}
		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		switch(OsVersionInfo.dwMajorVersion)
		{
		case 4:
			switch(OsVersionInfo.dwMinorVersion)
			{
			case 0:
				OsType = WINDOWS_95;
				break;
			case 90:
				OsType = WINDOWS_ME;
				break;
			default:
				OsType = WINDOWS_98;
				break;
			}
		}
		break;
	case VER_PLATFORM_WIN32s:
		OsType = WINDOWS_31;
		break;
	default:
		break;
	}
	
	return OsType;
}
/////////////////////////////////////////////////////////////////////////////