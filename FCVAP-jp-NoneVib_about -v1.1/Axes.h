// Axes.h
// ����:ע����ؼ�����,������ؼ�X��Y����,��ʾ��ؼ�״̬
// �汾:1.2003.10.06
// ����:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __AXES_H__
#define __AXES_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK	AxesWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				AxesOnPaint(HWND hWnd);

//���ں���
BOOL				RegisterAxesClass(HINSTANCE hInst);
void				SetAxesState(HWND hDlg, UINT AxesID, UINT x, UINT y);
/////////////////////////////////////////////////////////////////////////////
#endif // __AXES_H__