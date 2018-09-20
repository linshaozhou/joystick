
#ifndef __TEST_H__
#define __TEST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ID_TEST_TIMER			1
#define TEST_TIMER_ELAPSE		10
#define BUTTON_NUMBER			12  //10

#include "DInputUtil.h"
/////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK TestProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT DisplayTestState(HWND hWnd, CDIGameCntrlPropSheet* pdigcPropSheet);
/////////////////////////////////////////////////////////////////////////////
#endif // __TEST_H__
