
// PunchingTorqueManager.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

#define LIST_PATH _T("C:\\R2RSet\\PunchingTorqueList.ini")
#define LOG_PATH _T("C:\\R2RSet\\Log\\")

#define THICKNESS_0 _T("��������")
#define THICKNESS_1 _T("���� �β�")
#define THICKNESS_2 _T("�߰� �β�")
#define THICKNESS_3 _T("�β��� �β�")

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