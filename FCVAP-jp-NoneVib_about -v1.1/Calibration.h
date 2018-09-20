// Calibration.h
// ����:��Ϸ�����������ҳУ׼ҳ��
// �汾:1.2003.10.06
// ����:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
#define ID_CALIBRATION_TIMER					2
#define CALIBRATION_TIMER_ELAPSE				10
/////////////////////////////////////////////////////////////////////////////
#include "DInputUtil.h"
/////////////////////////////////////////////////////////////////////////////
HRESULT				DisplayCalibrationState(HWND hWnd, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet);
//void				Calibration(LPDIJOYCONFIG pDInputJoyConfig, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet); //2003.02.27ɾ��
BOOL				Calibration(LPDIJOYCONFIG pDInputJoyConfig, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet); //2003.02.27���
//���ں���
BOOL CALLBACK		CalibrationProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
/////////////////////////////////////////////////////////////////////////////
#endif // __CALIBRATION_H__
