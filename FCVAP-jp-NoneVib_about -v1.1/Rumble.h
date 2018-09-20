// Rumble.h
// 功能:注册振动控件窗口,设置振动控件X和Y坐标,显示振动控件状态
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __RUMBLE_H__
#define __RUMBLE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK	RumbleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				RumbleOnPaint(HWND hWnd);

//出口函数
BOOL				RegisterRumbleClass(HINSTANCE hInst);
void				SetRumbleState(HWND hDlg, UINT RumbleID, UINT x, UINT y);
/////////////////////////////////////////////////////////////////////////////
#endif // __RUMBLE_H__