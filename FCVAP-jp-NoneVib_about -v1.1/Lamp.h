// Lamp.h
// ����:ע�ᰴ��ָʾ�ƿؼ�����,���ð���ָʾ�ƿؼ���ɫ,��ʾ����ָʾ�ƿؼ�
// �汾:1.2002.07.03
// ����:2002/07/03
/////////////////////////////////////////////////////////////////////////////
#ifndef __LAMP_H__
#define __LAMP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK	LampWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				LampOnPaint(HWND hWnd);

//���ں���
BOOL				RegisterLampClass(HINSTANCE hInst);
void				SetLampState(HWND hDlg, UINT LampID, UINT LampColor);
/////////////////////////////////////////////////////////////////////////////
#endif // __LAMP_H__