// Pov.h
// ����:ע��POVñ�ؼ�����,����POVñ�ؼ�����,��ʾPOVñ�ؼ�
// �汾:1.2003.10.06
// ����:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __POV_H__
#define __POV_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK	PovWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				PovOnPaint(HWND hWnd);

//���ں���
BOOL				RegisterPovClass(HINSTANCE hInst);
void				SetPovState(HWND hDlg, UINT PovID, UINT Direction, UINT XAnalog, UINT YAnalog);
/////////////////////////////////////////////////////////////////////////////
#endif // __POV_H__