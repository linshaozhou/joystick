// Lamp.h
// 功能:注册按键指示灯控件窗口,设置按键指示灯控件颜色,显示按键指示灯控件
// 版本:1.2002.07.03
// 日期:2002/07/03
/////////////////////////////////////////////////////////////////////////////
#ifndef __LAMP_H__
#define __LAMP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK	LampWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				LampOnPaint(HWND hWnd);

//出口函数
BOOL				RegisterLampClass(HINSTANCE hInst);
void				SetLampState(HWND hDlg, UINT LampID, UINT LampColor);
/////////////////////////////////////////////////////////////////////////////
#endif // __LAMP_H__