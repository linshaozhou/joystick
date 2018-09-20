// Pov.cpp
// 功能:注册POV帽控件窗口,设置POV帽控件方向,显示POV帽控件
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Pov.h"
#include <math.h>
HRGN hRgnNew = NULL;
HRGN hRgnOld = NULL;
#define GWL_USERDATA        (-21)
#define DWL_USER        8
#define DELETE_OBJECT(_x_) {if(NULL != _x_) {DeleteObject(_x_); _x_ = NULL;}}
/////////////////////////////////////////////////////////////////////////////
//注册POV帽控件窗口类
BOOL RegisterPovClass(HINSTANCE hInst)
{
	WNDCLASS PovClass;

	PovClass.style          = CS_HREDRAW | CS_VREDRAW;
	PovClass.lpfnWndProc    = PovWndProc;
	PovClass.cbClsExtra     = 0;
	PovClass.cbWndExtra     = 0;
	PovClass.hInstance      = hInst;
	PovClass.hIcon          = NULL;
	PovClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	PovClass.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
	PovClass.lpszMenuName   = NULL;
	PovClass.lpszClassName  = TEXT("PovControl");

	ATOM aRet = RegisterClass(&PovClass);

	return (aRet) ? TRUE : FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//POV帽控件窗口过程
LRESULT CALLBACK PovWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		SetWindowLong(hWnd, GWL_USERDATA, 0x0064FFFF);
		break;
	case WM_PAINT:
		PovOnPaint(hWnd);
		break;
	case WM_DESTROY:
		if ( (hRgnOld == NULL) || (hRgnOld == hRgnNew) ){
			DELETE_OBJECT(hRgnNew);
			hRgnOld = NULL;
		}
		else{
			DELETE_OBJECT(hRgnOld);
			DELETE_OBJECT(hRgnNew);
		}
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//显示POV帽控件
void PovOnPaint(HWND hWnd)
{
	HDC	PovHdc;
	PAINTSTRUCT ps;
	PovHdc = BeginPaint(hWnd, &ps);

	LONG UserData = GetWindowLong(hWnd, GWL_USERDATA);
	DWORD Direction = LOWORD(UserData);
	double Analog = HIWORD(UserData) / 100.00;
//	HPEN RedPen 	= CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
//	HBRUSH ArrowBrush 	= CreateSolidBrush(RGB(255, 0, 0));
	HPEN RedPen 	= CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	HBRUSH ArrowBrush 	= CreateSolidBrush(RGB(100, 100, 100));


	RECT PovRect;
	GetClientRect(hWnd, &PovRect);
	PovRect.right -= 1;
	PovRect.bottom -= 1;

	InflateRect(&PovRect, -10, -10);

	HPEN OldPen = (HPEN)SelectObject(PovHdc, RedPen);
	Ellipse(PovHdc, PovRect.left, PovRect.top, PovRect.right, PovRect.bottom);
	HBRUSH OldBrush = (HBRUSH)SelectObject(PovHdc, ArrowBrush);
	POINT CenterPoint = {(PovRect.left + PovRect. right) / 2, (PovRect.top + PovRect.bottom) / 2};
	Ellipse(PovHdc, CenterPoint.x - 5, CenterPoint.y - 5, CenterPoint.x + 6, CenterPoint.y + 6);
	SIZE PovSize = {(PovRect.right - PovRect.left) / 2, (PovRect.bottom - PovRect.top) / 2};
	
	if(Direction < 36000)
	{
		double Radian = ((Direction / 100.0) * 3.14159265358979323864) / 180;
		
		POINT Temp[2];
		Temp[0].x = (long)(CenterPoint.x + sin(Radian) * PovSize.cx * Analog);
		Temp[0].y = (long)(CenterPoint.y - cos(Radian) * PovSize.cy * Analog);
		Temp[1].x = (long)(CenterPoint.x + sin(Radian) * PovSize.cx * Analog - sin(Radian) * 10 * Analog);
		Temp[1].y = (long)(CenterPoint.y - cos(Radian) * PovSize.cy * Analog + cos(Radian) * 10 * Analog);

		POINT Arrow[7];
/*		Arrow[0].x = (long)(Temp[0].x + sin(Radian) * 10 * Analog);
		Arrow[0].y = (long)(Temp[0].y - cos(Radian) * 10 * Analog);
		Arrow[1].x = (long)(Temp[0].x + cos(Radian) * 10 * Analog);
		Arrow[1].y = (long)(Temp[0].y + sin(Radian) * 10 * Analog);
		Arrow[2].x = (long)(Temp[0].x + cos(Radian) * 5 * Analog);
		Arrow[2].y = (long)(Temp[0].y + sin(Radian) * 5 * Analog);
		Arrow[3].x = (long)(Temp[1].x + cos(Radian) * 5 * Analog);
		Arrow[3].y = (long)(Temp[1].y + sin(Radian) * 5 * Analog);
		Arrow[4].x = (long)(Temp[1].x - cos(Radian) * 5 * Analog);
		Arrow[4].y = (long)(Temp[1].y - sin(Radian) * 5 * Analog);
		Arrow[5].x = (long)(Temp[0].x - cos(Radian) * 5 * Analog);
		Arrow[5].y = (long)(Temp[0].y - sin(Radian) * 5 * Analog);
		Arrow[6].x = (long)(Temp[0].x - cos(Radian) * 10 * Analog);
		Arrow[6].y = (long)(Temp[0].y - sin(Radian) * 10 * Analog);*/
		int Rx=6,Ry=3;
		Arrow[0].x = (long)(Temp[0].x + sin(Radian) * Rx * Analog);
		Arrow[0].y = (long)(Temp[0].y - cos(Radian) * Rx * Analog);
		Arrow[1].x = (long)(Temp[0].x + cos(Radian) * Rx * Analog);
		Arrow[1].y = (long)(Temp[0].y + sin(Radian) * Rx * Analog);
		Arrow[2].x = (long)(Temp[0].x + cos(Radian) * Ry * Analog);
		Arrow[2].y = (long)(Temp[0].y + sin(Radian) * Ry * Analog);
		Arrow[3].x = (long)(Temp[1].x + cos(Radian) * Ry * Analog);
		Arrow[3].y = (long)(Temp[1].y + sin(Radian) * Ry * Analog);
		Arrow[4].x = (long)(Temp[1].x - cos(Radian) * Ry * Analog);
		Arrow[4].y = (long)(Temp[1].y - sin(Radian) * Ry * Analog);
		Arrow[5].x = (long)(Temp[0].x - cos(Radian) * Ry * Analog);
		Arrow[5].y = (long)(Temp[0].y - sin(Radian) * Ry * Analog);
		Arrow[6].x = (long)(Temp[0].x - cos(Radian) * Rx * Analog);
		Arrow[6].y = (long)(Temp[0].y - sin(Radian) * Rx * Analog);



		Polygon(PovHdc, Arrow, 7);
	}

	SelectObject(PovHdc, OldPen);
	SelectObject(PovHdc, OldBrush);

	DELETE_OBJECT(RedPen);
	DELETE_OBJECT(ArrowBrush);

	EndPaint(hWnd, &ps);
}
/////////////////////////////////////////////////////////////////////////////
//设置POV帽控件的方向
void SetPovState(HWND hDlg, UINT PovID, UINT Direction, UINT XAnalog, UINT YAnalog)
{
	HWND hPovWnd = GetDlgItem(hDlg, PovID);
	UINT Analog;
	Direction &= 0xFFFF;
	switch(Direction / 4500)
	{
	case 0:
	case 4:
		Analog = fabsf(YAnalog);
		break;
	case 2:
	case 6:
		Analog = fabsf(XAnalog);
		break;
	case 1:
	case 3:
	case 5:
	case 7:
		Analog = (fabsf(XAnalog) + fabsf(YAnalog)) / 2;
		break;
	}
	Direction += (Analog << 16);
	SetWindowLong(hPovWnd, GWL_USERDATA, Direction);

	double fAnalog = Analog / 100.00;
	Direction = Direction & 0xFFFF;
	RECT PovRect;
	GetClientRect(hPovWnd, &PovRect);
	InflateRect(&PovRect, -12, -12);
	POINT CenterPoint = {(PovRect.left + PovRect. right) / 2, (PovRect.top + PovRect.bottom) / 2};
	SIZE PovSize = {(PovRect.right - PovRect.left) / 2, (PovRect.bottom - PovRect.top) / 2};
	
	if(Direction < 36000)
	{
		double Radian = ((Direction / 100.0) * 3.14159265358979323864) / 180;
		
		POINT Temp[2];
		Temp[0].x = (long)(CenterPoint.x + sin(Radian) * PovSize.cx * fAnalog);
		Temp[0].y = (long)(CenterPoint.y - cos(Radian) * PovSize.cy * fAnalog);
		Temp[1].x = (long)(CenterPoint.x + sin(Radian) * PovSize.cx * fAnalog - sin(Radian) * 10 * fAnalog);
		Temp[1].y = (long)(CenterPoint.y - cos(Radian) * PovSize.cy * fAnalog + cos(Radian) * 10 * fAnalog);

		POINT Arrow[8];
/*		Arrow[0].x = (long)(Temp[0].x + sin(Radian) * 10 * fAnalog);
		Arrow[0].y = (long)(Temp[0].y - cos(Radian) * 10 * fAnalog);
		Arrow[1].x = (long)(Temp[0].x + cos(Radian) * 10 * fAnalog);
		Arrow[1].y = (long)(Temp[0].y + sin(Radian) * 10 * fAnalog);
		Arrow[2].x = (long)(Temp[0].x + cos(Radian) * 5 * fAnalog);
		Arrow[2].y = (long)(Temp[0].y + sin(Radian) * 5 * fAnalog);
		Arrow[3].x = (long)(Temp[1].x + cos(Radian) * 5 * fAnalog);
		Arrow[3].y = (long)(Temp[1].y + sin(Radian) * 5 * fAnalog);
		Arrow[4].x = (long)(Temp[1].x - cos(Radian) * 5 * fAnalog);
		Arrow[4].y = (long)(Temp[1].y - sin(Radian) * 5 * fAnalog);
		Arrow[5].x = (long)(Temp[0].x - cos(Radian) * 5 * fAnalog);
		Arrow[5].y = (long)(Temp[0].y - sin(Radian) * 5 * fAnalog);
		Arrow[6].x = (long)(Temp[0].x - cos(Radian) * 10 * fAnalog);
		Arrow[6].y = (long)(Temp[0].y - sin(Radian) * 10 * fAnalog);
		Arrow[7].x = (long)(Temp[0].x + sin(Radian) * 10 * fAnalog);
		Arrow[7].y = (long)(Temp[0].y - cos(Radian) * 10 * fAnalog);*/
        int Rx=6, Ry=3;
		Arrow[0].x = (long)(Temp[0].x + sin(Radian) * Rx * fAnalog);
		Arrow[0].y = (long)(Temp[0].y - cos(Radian) * Rx * fAnalog);
		Arrow[1].x = (long)(Temp[0].x + cos(Radian) * Rx * fAnalog);
		Arrow[1].y = (long)(Temp[0].y + sin(Radian) * Rx * fAnalog);
		Arrow[2].x = (long)(Temp[0].x + cos(Radian) * Ry * fAnalog);
		Arrow[2].y = (long)(Temp[0].y + sin(Radian) * Ry * fAnalog);
		Arrow[3].x = (long)(Temp[1].x + cos(Radian) * Ry * fAnalog);
		Arrow[3].y = (long)(Temp[1].y + sin(Radian) * Ry * fAnalog);
		Arrow[4].x = (long)(Temp[1].x - cos(Radian) * Ry * fAnalog);
		Arrow[4].y = (long)(Temp[1].y - sin(Radian) * Ry * fAnalog);
		Arrow[5].x = (long)(Temp[0].x - cos(Radian) * Ry * fAnalog);
		Arrow[5].y = (long)(Temp[0].y - sin(Radian) * Ry * fAnalog);
		Arrow[6].x = (long)(Temp[0].x - cos(Radian) * Rx * fAnalog);
		Arrow[6].y = (long)(Temp[0].y - sin(Radian) * Rx * fAnalog);
		Arrow[7].x = (long)(Temp[0].x + sin(Radian) * Rx * fAnalog);
		Arrow[7].y = (long)(Temp[0].y - cos(Radian) * Rx * fAnalog);

		hRgnNew = CreatePolygonRgn(Arrow, 8, ALTERNATE);
	}

	RECT Rect;
	if ( (hRgnOld == NULL) || (hRgnOld == hRgnNew) ){
		GetRgnBox(hRgnNew, &Rect);
		InflateRect(&Rect, 3, 3);
		InvalidateRect(hPovWnd, &Rect, TRUE);
		hRgnOld = hRgnNew;
	}
	else{
		HRGN hRgnTmp = hRgnOld;
		GetRgnBox(hRgnOld, &Rect);
		InflateRect(&Rect, 3, 3);
		InvalidateRect(hPovWnd, &Rect, TRUE);
		GetRgnBox(hRgnNew, &Rect);
		InflateRect(&Rect, 3, 3);
		InvalidateRect(hPovWnd, &Rect, TRUE);
		hRgnOld = hRgnNew;
		DELETE_OBJECT(hRgnTmp);
	}
}                                                    
/////////////////////////////////////////////////////////////////////////////
