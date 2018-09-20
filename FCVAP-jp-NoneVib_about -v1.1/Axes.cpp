// Axes.cpp
// 功能:注册轴控件窗口,设置轴控件X和Y坐标,显示轴控件状态
// 版本:1.2002.07.03
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Axes.h"
/////////////////////////////////////////////////////////////////////////////
#define GWL_USERDATA        (-21)
//注册轴控件窗口类
BOOL RegisterAxesClass(HINSTANCE hInst)
{
	WNDCLASS AxesClass;

	AxesClass.style          = CS_HREDRAW | CS_VREDRAW;
	AxesClass.lpfnWndProc    = AxesWndProc;
	AxesClass.cbClsExtra     = 0;
	AxesClass.cbWndExtra     = 0;
	AxesClass.hInstance      = hInst;
	AxesClass.hIcon          = NULL;
	AxesClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	AxesClass.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
	AxesClass.lpszMenuName   = NULL;
	AxesClass.lpszClassName  = TEXT("AxesControl");

	ATOM aRet = RegisterClass(&AxesClass);

	return (aRet) ? TRUE : FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//轴控件窗口过程
LRESULT CALLBACK AxesWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		SetWindowLong(hWnd, GWL_USERDATA, 0x80008000);
		break;
	case WM_PAINT:
		AxesOnPaint(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//显示轴控件
void AxesOnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC AxesHdc;
	AxesHdc = BeginPaint(hWnd, &ps);

	RECT AxesRect;
	GetClientRect(hWnd, &AxesRect);
	AxesRect.right -= 1;
	AxesRect.bottom -= 1;

	HPEN BlackPen		= CreatePen(PS_SOLID, 1, 0x00000000);
	HPEN DarkGrayPen	= CreatePen(PS_SOLID, 1, 0x009D9DA1);
	HPEN LightGrayPen	= CreatePen(PS_SOLID, 1, 0x00E2EFF1);
	HPEN WhitePen		= CreatePen(PS_SOLID, 1, 0x00FFFFFF);
//	HPEN CrossPen		= CreatePen(PS_SOLID, 1, 0x000000FF);
	HPEN CrossPen		= CreatePen(PS_SOLID, 1, 0x00000000);
	HBRUSH FillBrush	= CreateSolidBrush(0x00FFFFFF);
	HPEN OldPen = (HPEN)SelectObject(AxesHdc, DarkGrayPen);
	MoveToEx(AxesHdc, AxesRect.left, AxesRect.bottom, NULL);
	LineTo(AxesHdc, AxesRect.left, AxesRect.top);
	LineTo(AxesHdc, AxesRect.right, AxesRect.top);
	SelectObject(AxesHdc, WhitePen);
	MoveToEx(AxesHdc, AxesRect.right, AxesRect.top, NULL);
	LineTo(AxesHdc, AxesRect.right, AxesRect.bottom);
	LineTo(AxesHdc, AxesRect.left, AxesRect.bottom);
	InflateRect(&AxesRect, -1, -1);
	SelectObject(AxesHdc, BlackPen);
	MoveToEx(AxesHdc, AxesRect.left, AxesRect.bottom, NULL);
	LineTo(AxesHdc, AxesRect.left, AxesRect.top);
	LineTo(AxesHdc, AxesRect.right, AxesRect.top);
	SelectObject(AxesHdc, LightGrayPen);
	LineTo(AxesHdc, AxesRect.right, AxesRect.bottom);
	LineTo(AxesHdc, AxesRect.left, AxesRect.bottom);

	InflateRect(&AxesRect, -1, -1);
	AxesRect.right += 1;
	AxesRect.bottom += 1;
	FillRect(AxesHdc, &AxesRect, FillBrush);

	long UserData = GetWindowLong(hWnd, GWL_USERDATA);
	DWORD x = HIWORD(UserData);
	DWORD y = LOWORD(UserData);
	int CrossSize = 5;
	InflateRect(&AxesRect, 2, 2);
	POINT CrossCenterPoint;
	CrossCenterPoint.x = x * (AxesRect.right - AxesRect.left) / 65536;
	CrossCenterPoint.y = y * (AxesRect.bottom - AxesRect.top) / 65536;
	CrossCenterPoint.x = (CrossCenterPoint.x < 3) ? 3 : CrossCenterPoint.x;
	CrossCenterPoint.y = (CrossCenterPoint.y < 3) ? 3 : CrossCenterPoint.y;
	CrossCenterPoint.x = (CrossCenterPoint.x > AxesRect.right - AxesRect.left - 4) ? (AxesRect.right - AxesRect.left - 4) : CrossCenterPoint.x;
	CrossCenterPoint.y = (CrossCenterPoint.y > AxesRect.bottom - AxesRect.top - 4) ? (AxesRect.bottom - AxesRect.top - 4) : CrossCenterPoint.y;
	RECT CrossRect;
	CrossRect.left = (CrossCenterPoint.x < (CrossSize + 3)) ? 3 : (CrossCenterPoint.x - CrossSize);
	CrossRect.right = ((CrossCenterPoint.x + CrossSize) > (AxesRect.right - AxesRect.left - 4)) ? (AxesRect.right - AxesRect.left - 4) : (CrossCenterPoint.x + CrossSize + 1);
	CrossRect.top = (CrossCenterPoint.y < (CrossSize + 3)) ? 3 : (CrossCenterPoint.y - CrossSize);
	CrossRect.bottom = ((CrossCenterPoint.y + CrossSize) > (AxesRect.bottom - AxesRect.top - 4)) ? (AxesRect.bottom - AxesRect.top - 4) : (CrossCenterPoint.y + CrossSize + 1);
	SelectObject(AxesHdc, CrossPen);
	MoveToEx(AxesHdc, CrossRect.left, CrossCenterPoint.y, NULL);
	LineTo(AxesHdc, CrossRect.right, CrossCenterPoint.y);
	MoveToEx(AxesHdc,  CrossCenterPoint.x, CrossRect.top, NULL);
	LineTo(AxesHdc, CrossCenterPoint.x, CrossRect.bottom);

	SelectObject(AxesHdc, OldPen);
	DeleteObject(BlackPen);
	DeleteObject(DarkGrayPen);
	DeleteObject(LightGrayPen);
	DeleteObject(WhitePen);
	DeleteObject(CrossPen);
	DeleteObject(FillBrush);

	EndPaint(hWnd, &ps);
}
/////////////////////////////////////////////////////////////////////////////
//设置轴控件的X和Y坐标
void SetAxesState(HWND hDlg, UINT AxesID, UINT x, UINT y)
{
	HWND hAxesWnd = GetDlgItem(hDlg, AxesID);
	SetWindowLong(hAxesWnd, GWL_USERDATA, (x << 16) + y);
	RedrawWindow(hAxesWnd, NULL, NULL, RDW_INVALIDATE);
}                                                    
/////////////////////////////////////////////////////////////////////////////
