// Rumble.cpp
// ����:ע���񶯿ؼ�����,�����񶯿ؼ�X��Y����,��ʾ�񶯿ؼ�״̬
// �汾:1.2003.10.06
// ����:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Rumble.h"
#define GWL_USERDATA        (-21)
#define DWL_USER        8
/////////////////////////////////////////////////////////////////////////////
//ע���񶯿ؼ�������
BOOL RegisterRumbleClass(HINSTANCE hInst)
{
	WNDCLASS RumbleClass;

	RumbleClass.style          = CS_HREDRAW | CS_VREDRAW;
	RumbleClass.lpfnWndProc    = RumbleWndProc;
	RumbleClass.cbClsExtra     = 0;
	RumbleClass.cbWndExtra     = 0;
	RumbleClass.hInstance      = hInst;
	RumbleClass.hIcon          = NULL;
	RumbleClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	RumbleClass.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
	RumbleClass.lpszMenuName   = NULL;
	RumbleClass.lpszClassName  = TEXT("RumbleControl");

	ATOM aRet = RegisterClass(&RumbleClass);

	return (aRet) ? TRUE : FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//�񶯿ؼ����ڹ���
LRESULT CALLBACK RumbleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		RumbleOnPaint(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//��ʾ�񶯿ؼ�
void RumbleOnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC RumbleHdc;
	RumbleHdc = BeginPaint(hWnd, &ps);

	RECT RumbleRect;
	GetClientRect(hWnd, &RumbleRect);
	RumbleRect.right -= 1;
	RumbleRect.bottom -= 1;

	HPEN BlackPen		= CreatePen(PS_SOLID, 1, 0x00000000);
	HPEN DarkGrayPen	= CreatePen(PS_SOLID, 1, 0x009D9DA1);
	HPEN LightGrayPen	= CreatePen(PS_SOLID, 1, 0x00E2EFF1);
	HPEN WhitePen		= CreatePen(PS_SOLID, 1, 0x00FFFFFF);
	HPEN CrossPen		= CreatePen(PS_SOLID, 1, 0x00000000);
	HBRUSH EffectBrush	= CreateSolidBrush(0X000000FF);
	HBRUSH FillBrush	= CreateSolidBrush(0x00FFFFFF);

	HPEN OldPen = (HPEN)SelectObject(RumbleHdc, DarkGrayPen);
	MoveToEx(RumbleHdc, RumbleRect.left, RumbleRect.bottom, NULL);
	LineTo(RumbleHdc, RumbleRect.left, RumbleRect.top);
	LineTo(RumbleHdc, RumbleRect.right, RumbleRect.top);
	SelectObject(RumbleHdc, WhitePen);
	MoveToEx(RumbleHdc, RumbleRect.right, RumbleRect.top, NULL);
	LineTo(RumbleHdc, RumbleRect.right, RumbleRect.bottom);
	LineTo(RumbleHdc, RumbleRect.left, RumbleRect.bottom);
	InflateRect(&RumbleRect, -1, -1);
	SelectObject(RumbleHdc, BlackPen);
	MoveToEx(RumbleHdc, RumbleRect.left, RumbleRect.bottom, NULL);
	LineTo(RumbleHdc, RumbleRect.left, RumbleRect.top);
	LineTo(RumbleHdc, RumbleRect.right, RumbleRect.top);
	SelectObject(RumbleHdc, LightGrayPen);
	LineTo(RumbleHdc, RumbleRect.right, RumbleRect.bottom);
	LineTo(RumbleHdc, RumbleRect.left, RumbleRect.bottom);

	InflateRect(&RumbleRect, -1, -1);
	RumbleRect.right += 1;
	RumbleRect.bottom += 1;
	FillRect(RumbleHdc, &RumbleRect, FillBrush);

	long UserData = GetWindowLong(hWnd, GWL_USERDATA);
	short x = HIWORD(UserData);
 	short y = LOWORD(UserData);
	InflateRect(&RumbleRect, -5, -5);
	POINT CrossCenterPoint = {(RumbleRect.left + RumbleRect.right) / 2, (RumbleRect.top + RumbleRect.bottom) / 2};
	SelectObject(RumbleHdc, CrossPen);
/*	MoveToEx(RumbleHdc, CrossCenterPoint.x - 15, CrossCenterPoint.y, NULL);
	LineTo(RumbleHdc, CrossCenterPoint.x + 16, CrossCenterPoint.y);
	MoveToEx(RumbleHdc, CrossCenterPoint.x, CrossCenterPoint.y - 15, NULL);
	LineTo(RumbleHdc, CrossCenterPoint.x, CrossCenterPoint.y + 16);*/

	MoveToEx(RumbleHdc, CrossCenterPoint.x - 9, CrossCenterPoint.y, NULL);
	LineTo(RumbleHdc, CrossCenterPoint.x + 10, CrossCenterPoint.y);
	MoveToEx(RumbleHdc, CrossCenterPoint.x, CrossCenterPoint.y - 9, NULL);
	LineTo(RumbleHdc, CrossCenterPoint.x, CrossCenterPoint.y + 10);


	POINT CircleCenter = {(int)(CrossCenterPoint.x + ((RumbleRect.right - RumbleRect.left) / 2) * (x / 10000.0)), (int)(CrossCenterPoint.y + ((RumbleRect.bottom - RumbleRect.top) / 2) * (y / 10000.0))};
	HBRUSH OldBrush = (HBRUSH)SelectObject(RumbleHdc, EffectBrush);
	Ellipse(RumbleHdc, CircleCenter.x - 5, CircleCenter.y - 5, CircleCenter.x + 6, CircleCenter.y + 6);

	SelectObject(RumbleHdc, OldPen);
	SelectObject(RumbleHdc, OldBrush);
	DeleteObject(BlackPen);
	DeleteObject(DarkGrayPen);
	DeleteObject(LightGrayPen);
	DeleteObject(WhitePen);
	DeleteObject(CrossPen);
	DeleteObject(EffectBrush);
	DeleteObject(FillBrush);

	EndPaint(hWnd, &ps);
}
/////////////////////////////////////////////////////////////////////////////
//�����񶯿ؼ���X��Y����
void SetRumbleState(HWND hDlg, UINT RumbleID, UINT x, UINT y)
{
	HWND hRumbleWnd = GetDlgItem(hDlg, RumbleID);
	SetWindowLong(hRumbleWnd, GWL_USERDATA, (x << 16) + y);
	RedrawWindow(hRumbleWnd, NULL, NULL, RDW_INVALIDATE);
}                                                    
/////////////////////////////////////////////////////////////////////////////
