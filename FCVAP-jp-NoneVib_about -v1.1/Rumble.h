// Rumble.h
// ����:ע���񶯿ؼ�����,�����񶯿ؼ�X��Y����,��ʾ�񶯿ؼ�״̬
// �汾:1.2003.10.06
// ����:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __RUMBLE_H__
#define __RUMBLE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK	RumbleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				RumbleOnPaint(HWND hWnd);

//���ں���
BOOL				RegisterRumbleClass(HINSTANCE hInst);
void				SetRumbleState(HWND hDlg, UINT RumbleID, UINT x, UINT y);
/////////////////////////////////////////////////////////////////////////////
#endif // __RUMBLE_H__