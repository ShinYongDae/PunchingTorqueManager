
// PunchingTorqueManager.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

#define LIST_PATH _T("C:\\R2RSet\\PunchingTorqueList.ini")
#define LOG_PATH _T("C:\\R2RSet\\Log\\")

#define THICKNESS_0 _T("설정없음")
#define THICKNESS_1 _T("얇은 두께")
#define THICKNESS_2 _T("중간 두께")
#define THICKNESS_3 _T("두꺼운 두께")

// CPunchingTorqueManagerApp:
// 이 클래스의 구현에 대해서는 PunchingTorqueManager.cpp을 참조하십시오.
//

class CPunchingTorqueManagerApp : public CWinApp
{
public:
	CPunchingTorqueManagerApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CPunchingTorqueManagerApp theApp;