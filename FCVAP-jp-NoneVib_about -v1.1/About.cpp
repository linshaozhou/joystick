// about.cpp
// 功能:游戏控制面板属性页振动页面
// 版本:1.2002.10.08
// 日期:2003/10/08
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "about.h"
#include "DIGameCntrlPropSheet.h"
#include "DInputUtil.h"
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//about page
BOOL CALLBACK AboutProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*static CDIGameCntrlPropSheet *pDIGameCntrlPropSheet = (CDIGameCntrlPropSheet*)GetWindowLong(hWnd, DWL_USER);

	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			pDIGameCntrlPropSheet = (CDIGameCntrlPropSheet*)((LPPROPSHEETPAGE)lParam)->lParam;
			SetWindowLong(hWnd, DWL_USER, (LPARAM)pDIGameCntrlPropSheet);
			if(FAILED(InitDInput(GetParent(hWnd), pDIGameCntrlPropSheet)))
			{
		        return TRUE;
			}
		}
		break;
	case WM_NOTIFY:
 	 	 return TRUE;
    }*/
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////
