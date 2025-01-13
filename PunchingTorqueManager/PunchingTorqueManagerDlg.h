
// PunchingTorqueManagerDlg.h : ��� ����
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
	int nNum;			// ȣ�� ��ȣ
	stTorque Torq;
	stUnit()
	{
		nNum = 0;
	}
};

struct stThick
{
	int nTotalUnit;		// 4 : 11, 12, 13, 14
	int *pUnitList;		// ȣ�� ��ȣ List
	stUnit *pUnit;		// ȣ�� : 11, 12, 13, 14

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

	void LoadUnit(int nThickness)		// �β��� ȣ���� ��ũ�� �ε�
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
	int nThick;			// 0 ~ 3 :  ��������[0], ���� �β�[1], �߰� �β�[2], �β��� �β�[3]

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
	int *pUnitList;		// ȣ�� ��ȣ List
	int nTotalModels;
	stModel *pModel;
	stThick *pThick;	// �β��� �⺻ ��ũ�� (�⺻+3�ܰ�) : ��������[0], ���� �β�[1], �߰� �β�[2], �β��� �β�[3]

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
				pThick[i].LoadUnit(i); //  ��������[0], ���� �β�[1], �߰� �β�[2], �β��� �β�[3] : �β����� ��� Unit(11 ~ 14ȣ��)�� ���� ��/�� ��ũ���� �ε���.
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

	void ReloadList()
	{
		if (pUnitList)
		{
			delete[] pUnitList;
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

		LoadList();
	}

	void AllocMem()
	{
		if (nTotalMachines > 0)
		{
			if(pUnitList)
				delete[] pUnitList;

			pUnitList = new int[nTotalMachines];
		}

		if (nTotalThicknesses > 0)	// �β��� �⺻ ��ũ�� (�⺻+3�ܰ�) : ��������[0], ���� �β�[1], �߰� �β�[2], �β��� �β�[3]
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




// CPunchingTorqueManagerDlg ��ȭ ����
class CPunchingTorqueManagerDlg : public CDialog
{
	stList m_stList;
	int m_nTotalUnit, m_nTotalModel, m_nTotalThick;
	CString m_sPathCamSpecDir, m_sModel;
	int m_nUnit;
	int m_nThickModel, m_nThickUnit;

	void LoadIni();
	void InitList();
	void InitModel();
	void ModifyModelData();
	void ModifyThicknessData();
	BOOL DirectoryExists(LPCTSTR szPath);
	void StringToChar(CString str, char* pCh); // char* returned must be deleted... 
	void ModifyComboThickModel(int nThick);
	void ModifyComboThickUnit(int nUnit, int nThick);
	void ModifyEditTorqueUnit(int nUnit, int nThick);

// �����Դϴ�.
public:
	CPunchingTorqueManagerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PUNCHINGTORQUEMANAGER_DIALOG };
#endif

	void Log(CString strMsg, int nType=0);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	int GetModelThickness(CString sModel);
	CString GetThicknessName(int nIdx); // ��������[0], ���� �β�[1], �߰� �β�[2], �β��� �β�[3]
	BOOL UpdateModel(CString sModel, int nThickness);
	BOOL UpdateUnit(int nUnit, int nThickness, double dTorqL, double dTorqR);
	int SearchModel(CString sModel);
	void InsertModel(CString sModel, int nThickness);
	void ModifyModel(CString sModel, int nThickness);
	int SearchUnit(int nUnit);
	void InsertUnit(int nUnit, int nThickness, double dTorqL, double dTorqR);
	void ModifyUnit(int nUnit, int nThickness, double dTorqL, double dTorqR);
	void UpdateList();
	BOOL GetTorque(int nUnit, int nThick, double &dTorqL, double &dTorqR);
	void ResetComboThickModel();
	void ResetEditUnit();
	void DispList();

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnExit();
	CListCtrl m_List;
	afx_msg void OnSelchangeComboModel();
	afx_msg void OnSelchangeComboThickModel();
	afx_msg void OnSelchangeComboThickUnit();
	afx_msg void OnBnClickedButtonSaveModel();
	afx_msg void OnBnClickedButtonSaveUnit();
	afx_msg void OnChangeEditUnit();
};
