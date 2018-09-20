// Calibration.h
// 功能:游戏控制面板属性页校准页面
// 版本:1.2003.10.06
// 日期:2003/10/06
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
//void				Calibration(LPDIJOYCONFIG pDInputJoyConfig, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet); //2003.02.27删除
BOOL				Calibration(LPDIJOYCONFIG pDInputJoyConfig, CDIGameCntrlPropSheet* pDIGameCntrlPropSheet); //2003.02.27添加
//出口函数
BOOL CALLBACK		CalibrationProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
/////////////////////////////////////////////////////////////////////////////
#endif // __CALIBRATION_H__
