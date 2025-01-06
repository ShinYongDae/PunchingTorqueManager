
// PunchingTorqueManagerDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"


struct stTorque
{
	double dLeft, dRight;

	stTorque()
	{
		dLeft = 0.0;
		dRight = 0.0;
	}
};

struct stUnit
{
	int nNum;			// 호기 번호
	stTorque Torq;
	stUnit()
	{
		nNum = 0;
	}
};

struct stThick
{
	int nTotalUnit;		// 4 : 11, 12, 13, 14
	int *pUnitList;		// 호기 번호 List
	stUnit *pUnit;		// 호기 : 11, 12, 13, 14

	int i;
	TCHAR szData[MAX_PATH];
	TCHAR sep[6]; // _T(",/;\r\n\t");
	TCHAR *token;
	CString sThickIdx, sUnitNum;

	stThick()
	{
		nTotalUnit = 0;
		pUnitList = NULL;
		pUnit = NULL;
		sep[0] = _T(',');
		sep[1] = _T('/');
		sep[2] = _T(';');
		sep[3] = _T('\r');
		sep[4] = _T('\n');
		sep[5] = _T('\t');
		token = NULL;
	}

	~stThick()
	{
		if (pUnitList)
		{
			delete[] pUnitList;
			pUnitList = NULL;
		}

		if (pUnit)
		{
			delete[] pUnit;
			pUnit = NULL;
		}
	}

	void LoadUnit(int nThickness)		// 두께별 호기의 토크값 로딩
	{
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Machines"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalUnit = _ttoi(szData);
		else
			nTotalUnit = 0;

		AllocMem();

		if (nTotalUnit > 0)
		{
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Unit_Nums"), NULL, szData, sizeof(szData), LIST_PATH))
			{
				token = _tcstok(szData, sep);
				for (i = 0; i < nTotalUnit; i++)
				{
					pUnitList[i] = _ttoi(token);
					if (i < nTotalUnit-1)
						token = _tcstok(NULL, sep);
				}
			}

			sThickIdx.Format(_T("%d"), nThickness);
			for (i = 0; i < nTotalUnit; i++)		// 4 : 11, 12, 13, 14
			{
				pUnit[i].nNum = pUnitList[i];
				sUnitNum.Format(_T("%d"), pUnit[i].nNum);
				if (0 < ::GetPrivateProfileString(sThickIdx, sUnitNum, NULL, szData, sizeof(szData), LIST_PATH))
				{
					token = _tcstok(szData, sep);
					pUnit[i].Torq.dLeft = _ttof(token);
					token = _tcstok(NULL, sep);
					pUnit[i].Torq.dRight = _ttof(token);
				}
			}
		}
	}

	void AllocMem()
	{
		if (nTotalUnit > 0)
		{
			if (pUnitList)
				delete[] pUnitList;
			pUnitList = new int[nTotalUnit];

			if (pUnit)
				delete[] pUnit;
			pUnit = new stUnit[nTotalUnit];
		}
	}
};

struct stModel
{
	CString sModel;
	int nThick;			// 0 ~ 3 :  설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]

	stModel()
	{
		sModel = _T("");
		nThick = 0;
	}
};

struct stList
{
	int nTotalMachines;
	int nTotalThicknesses;
	int *pUnitList;		// 호기 번호 List
	int nTotalModels;
	stModel *pModel;
	stThick *pThick;	// 두께별 기본 토크값 (기본+3단계) : 설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]

	int i, nTotUnits;
	TCHAR szData[MAX_PATH];
	TCHAR sep[6]; // _T(",/;\r\n\t");
	TCHAR *token;
	CString sModelIdx;

	stList()
	{
		nTotalMachines = 0;
		nTotalThicknesses = 0;
		pUnitList = NULL;
		pThick = NULL;
		nTotalModels = 0;
		pModel = NULL;
		sep[0] = _T(',');
		sep[1] = _T('/');
		sep[2] = _T(';');
		sep[3] = _T('\r');
		sep[4] = _T('\n');
		sep[5] = _T('\t');
		token = NULL;

		LoadList();
	}
	~stList()
	{
		if (pUnitList)
		{
			delete [] pUnitList;
			pUnitList = NULL;
		}
		if (pThick)
		{
			delete[] pThick;
			pThick = NULL;
		}
		if (pModel)
		{
			delete[] pModel;
			pModel = NULL;
		}
	}

	void LoadList()
	{
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Machines"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalMachines = _ttoi(szData);
		else
			nTotalMachines = 0;

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Thicknesses"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalThicknesses = _ttoi(szData);
		else
			nTotalThicknesses = 0;

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Models"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalModels = _ttoi(szData);
		else
			nTotalModels = 0;

		AllocMem();

		if (nTotalMachines > 0)
		{
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Unit_Nums"), NULL, szData, sizeof(szData), LIST_PATH))
			{
				token = _tcstok(szData, sep);
				for (i = 0; i < nTotalMachines; i++)
				{
					pUnitList[i] = _ttoi(token);
					if (i < nTotalMachines - 1)
						token = _tcstok(NULL, sep);
				}
			}
		}

		if (nTotalThicknesses > 0)
		{
			for (i = 0; i < nTotalThicknesses; i++)
			{
				pThick[i].LoadUnit(i); //  설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3] : 두께별로 모든 Unit(11 ~ 14호기)에 대한 좌/우 토크값을 로딩함.
			}
		}

		if (nTotalModels > 0)
		{
			for (i = 0; i < nTotalModels; i++)
			{
				sModelIdx.Format(_T("%d"), i);
				if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
				{
					token = _tcstok(szData, sep);
					pModel[i].sModel = CString(token);
					token = _tcstok(NULL, sep);
					pModel[i].nThick = _ttoi(token);
				}
			}
		}
	}

	void AllocMem()
	{
		if (nTotalMachines > 0)
		{
			if(pUnitList)
				delete[] pUnitList;

			pUnitList = new int[nTotalMachines];
		}

		if (nTotalThicknesses > 0)	// 두께별 기본 토크값 (기본+3단계) : 설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]
		{
			if (pThick)
				delete[] pThick;

			pThick = new stThick[nTotalThicknesses];
		}

		if (nTotalModels > 0)
		{
			if (pModel)
				delete[] pModel;

			pModel = new stModel[nTotalModels];
		}
	}
};




// CPunchingTorqueManagerDlg 대화 상자
class CPunchingTorqueManagerDlg : public CDialog
{
	stList m_stList;

	void InitList();

// 생성입니다.
public:
	CPunchingTorqueManagerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PUNCHINGTORQUEMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnExit();
	CListCtrl m_List;
};
