
// PunchingTorqueManager.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

#define LIST_PATH _T("C:\\R2RSet\\PunchingTorqueList.ini")


// CPunchingTorqueManagerApp:
// �� Ŭ������ ������ ���ؼ��� PunchingTorqueManager.cpp�� �����Ͻʽÿ�.
//

class CPunchingTorqueManagerApp : public CWinApp
{
public:
	CPunchingTorqueManagerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPunchingTorqueManagerApp theApp;