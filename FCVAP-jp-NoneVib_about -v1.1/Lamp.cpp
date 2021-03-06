// Lamp.cpp
// 功能:注册按键指示灯窗口控件,设置按键指示灯颜色控件,显示按键指示灯控件
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Lamp.h"
#define GWL_USERDATA        (-21)
#define DWL_USER        8
/////////////////////////////////////////////////////////////////////////////
//注册按键指示灯控件窗口类
BOOL RegisterLampClass(HINSTANCE hInst)
{
	WNDCLASS LampClass;

	LampClass.style          = CS_HREDRAW | CS_VREDRAW;
	LampClass.lpfnWndProc    = LampWndProc;
	LampClass.cbClsExtra     = 0;
	LampClass.cbWndExtra     = 0;
	LampClass.hInstance      = hInst;
	LampClass.hIcon          = NULL;
	LampClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	LampClass.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
	LampClass.lpszMenuName   = NULL;
	LampClass.lpszClassName  = TEXT("LampControl");

	ATOM aRet = RegisterClass(&LampClass);

	return (aRet) ? TRUE : FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//按键指示灯控件窗口过程
LRESULT CALLBACK LampWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT:
			LampOnPaint(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//显示按键指示灯控件
void LampOnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC LampHdc = BeginPaint(hWnd, &ps);

	RECT LampRect;
	GetClientRect(hWnd, &LampRect);
	COLORREF LampColor = (COLORREF)GetWindowLong(hWnd, GWL_USERDATA);
	HBRUSH LampBrush = CreateSolidBrush(LampColor);
	HBRUSH OldBrush = (HBRUSH)SelectObject(LampHdc, LampBrush);
	Ellipse(LampHdc, LampRect.left, LampRect.top, LampRect.right, LampRect.bottom);
	SelectObject(LampHdc, OldBrush);
	DeleteObject(LampBrush);

	SetTextColor(LampHdc, RGB(202, 202, 202));
	SetBkMode(LampHdc, TRANSPARENT);
	TCHAR LampText[64]={0};
	GetWindowText(hWnd, LampText, 64);
	RECT LampTextRect = {LampRect.left, LampRect.top, LampRect.right, LampRect.bottom};
	LampTextRect.top = ((LampRect.top + LampRect.bottom) / 2) - 8;
	LampTextRect.bottom = ((LampRect.top + LampRect.bottom) / 2) + 8;
	DrawText(LampHdc, LampText,_tcslen(LampText), &LampTextRect, DT_CENTER);
//04.03.03 ad
	int nXStartArc=int((LampRect.right-LampRect.left)*1.2/4);  
    int nYStartArc=int((LampRect.bottom-LampRect.top)*3.2/4);  
    int nXEndArc=int((LampRect.right-LampRect.left)*3.2/4);    
    int nYEndArc=int((LampRect.bottom-LampRect.top)*1.2/4);  
	SelectObject(LampHdc,GetStockObject(WHITE_PEN));
    Arc(LampHdc,LampRect.left, LampRect.top, LampRect.right, LampRect.bottom,nXStartArc,nYStartArc,nXEndArc,nYEndArc);
//04.03.03 add end
	EndPaint(hWnd, &ps);
}
/////////////////////////////////////////////////////////////////////////////
//设置按键指示灯控件的颜色
void SetLampState(HWND hDlg, UINT LampID, UINT LampColor)
{
	HWND hLampWnd = GetDlgItem(hDlg, LampID);
	SetWindowLong(hLampWnd, GWL_USERDATA, LampColor);
	RedrawWindow(hLampWnd, NULL, NULL, RDW_INVALIDATE);
}
/////////////////////////////////////////////////////////////////////////////