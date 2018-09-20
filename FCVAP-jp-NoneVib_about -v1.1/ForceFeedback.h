// ForceFeedback.h
// 功能:游戏控制面板属性页振动页面
// 版本:1.2003.10.06
// 日期:2003/10/06
/////////////////////////////////////////////////////////////////////////////
#ifndef __FORCEFEEDBACK_H__
#define __FORCEFEEDBACK_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
#define ID_FF_TIMER					3
#define FF_TIMER_ELAPSE				10
/////////////////////////////////////////////////////////////////////////////
#include "DInputUtil.h"
/////////////////////////////////////////////////////////////////////////////
void				InitEffect(HWND hWnd, LPDIRECTINPUTDEVICE2 pDInputDevice2);
void				FreeEffect();
HRESULT				SetForceXY(LPDIRECTINPUTEFFECT pDInputEffect, int x, int y);
HRESULT				DisplayForceFeedbackState(CDIGameCntrlPropSheet* pDIGameCntrlPropSheet, HWND hWnd);
// 出口函数
BOOL CALLBACK		ForceFeedbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
/////////////////////////////////////////////////////////////////////////////
#endif // __FORCEFEEDBACK_H__
