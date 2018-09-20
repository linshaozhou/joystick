// Axes.h
// 功能:注册轴控件窗口,设置轴控件X和Y坐标,显示轴控件状态
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __AXES_H__
#define __AXES_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK	AxesWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				AxesOnPaint(HWND hWnd);

//出口函数
BOOL				RegisterAxesClass(HINSTANCE hInst);
void				SetAxesState(HWND hDlg, UINT AxesID, UINT x, UINT y);
/////////////////////////////////////////////////////////////////////////////
#endif // __AXES_H__