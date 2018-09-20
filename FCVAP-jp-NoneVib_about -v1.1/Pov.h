// Pov.h
// 功能:注册POV帽控件窗口,设置POV帽控件方向,显示POV帽控件
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __POV_H__
#define __POV_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK	PovWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				PovOnPaint(HWND hWnd);

//出口函数
BOOL				RegisterPovClass(HINSTANCE hInst);
void				SetPovState(HWND hDlg, UINT PovID, UINT Direction, UINT XAnalog, UINT YAnalog);
/////////////////////////////////////////////////////////////////////////////
#endif // __POV_H__