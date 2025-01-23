
// PunchingTorqueManagerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "PunchingTorqueManager.h"
#include "PunchingTorqueManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "safelock.h"
CCriticalSection g_LogLock;

// CPunchingTorqueManagerDlg ��ȭ ����


CPunchingTorqueManagerDlg::CPunchingTorqueManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PUNCHINGTORQUEMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sPathCamSpecDir = _T("");
	m_nTotalUnit = 0;
	m_nTotalModel = 0;
	m_nTotalThick = 0;
	m_sModel = _T("");
	m_nUnit = -1; // Not Selected
	m_nThickModel = 0;
	m_nThickUnit = 0;
}

void CPunchingTorqueManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CPunchingTorqueManagerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CPunchingTorqueManagerDlg::OnBnClickedBtnExit)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, &CPunchingTorqueManagerDlg::OnSelchangeComboModel)
	ON_CBN_SELCHANGE(IDC_COMBO_THICK_MODEL, &CPunchingTorqueManagerDlg::OnSelchangeComboThickModel)
	ON_CBN_SELCHANGE(IDC_COMBO_THICK_UNIT, &CPunchingTorqueManagerDlg::OnSelchangeComboThickUnit)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_MODEL, &CPunchingTorqueManagerDlg::OnBnClickedButtonSaveModel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_UNIT, &CPunchingTorqueManagerDlg::OnBnClickedButtonSaveUnit)
	ON_EN_CHANGE(IDC_EDIT_UNIT, &CPunchingTorqueManagerDlg::OnChangeEditUnit)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_MODEL, &CPunchingTorqueManagerDlg::OnBnClickedButtonRefreshModel)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CPunchingTorqueManagerDlg::OnClickList1)
	ON_BN_CLICKED(IDC_BUTTON_FIND_MODEL, &CPunchingTorqueManagerDlg::OnBnClickedButtonFindModel)
END_MESSAGE_MAP()


// CPunchingTorqueManagerDlg �޽��� ó����

BOOL CPunchingTorqueManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_STATIC_UNIT)->ShowWindow(SW_HIDE);

	LoadIni();
	InitList();
	DispList();

	ModifyModelData();
	ModifyThicknessData();

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SelectString(0, m_sModel);
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetWindowText(m_sModel);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPunchingTorqueManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CPunchingTorqueManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPunchingTorqueManagerDlg::OnBnClickedBtnExit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}

void CPunchingTorqueManagerDlg::InitList()
{
	// Addition ListControl
	int nIdxUnit;
	CString strItem = _T("");
	/*
		LVITEM item;
		::ZeroMemory(&item, sizeof(item));
		item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		// ���̵�
		item.iItem = m_List.GetItemCount(); //5;
		item.iSubItem = 0;
		item.iImage = 1;
		item.pszText = _T("1��");
		m_List.InsertItem(&item);
		// �̸�
		item.iSubItem = 1;
		item.pszText = _T("�ſ��");
		m_List.SetItem(&item);
		// �Ҽ�
		item.iSubItem = 2;
		item.pszText = _T("�ſ���� ���α׷���");
		m_List.SetItem(&item);
		//item.state = LVIS_SELECTED | LVIS_FOCUSED;
		//m_List.InsertItem(&item);
	*/
	m_List.DeleteAllItems();
	Sleep(30);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // ���� �߰�
	m_List.InsertColumn(0, _T("Index"), LVCFMT_CENTER, 60);//LVCFMT_LEFT
	m_List.InsertColumn(1, _T("��"), LVCFMT_CENTER, 140);//LVCFMT_LEFT
	m_List.InsertColumn(2, _T("�β�����"), LVCFMT_CENTER, 100);

	m_nTotalUnit = m_stList.nTotalMachines;
	if (m_nTotalUnit < 1)
		return;

	for (nIdxUnit = 0; nIdxUnit < m_nTotalUnit; nIdxUnit++)
	{
		strItem.Format(_T("#%d ��"), m_stList.pUnitList[nIdxUnit]);
		m_List.InsertColumn(3 + (nIdxUnit * 2), strItem, LVCFMT_CENTER, 80);
		strItem.Format(_T("#%d ��"), m_stList.pUnitList[nIdxUnit]);
		m_List.InsertColumn(4 + (nIdxUnit * 2), strItem, LVCFMT_CENTER, 80);
	}
}


void CPunchingTorqueManagerDlg::DispList()
{
	int nIdxModel, nIdxUnit;
	TCHAR tChar[200];
	CString sVal;

	m_List.DeleteAllItems();
	Sleep(30);

	for (nIdxModel = 0; nIdxModel < m_stList.nTotalModels; nIdxModel++)
	{
		LVITEM item;
		::ZeroMemory(&item, sizeof(item));
		item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		// Index
		item.iItem = m_List.GetItemCount();
		item.iSubItem = 0;
		item.iImage = 1;
		sVal.Format(_T("%d"), nIdxModel);
		_stprintf(tChar, _T("%s"), sVal);
		item.pszText = tChar;
		m_List.InsertItem(&item);
		// ��
		item.iSubItem = 1;
		_stprintf(tChar, _T("%s"), m_stList.pModel[nIdxModel].sModel);
		item.pszText = tChar;
		m_List.SetItem(&item);
		// �β�����
		item.iSubItem = 2;
		_stprintf(tChar, _T("%s"), GetThicknessName(m_stList.pModel[nIdxModel].nThick));
		item.pszText = tChar;
		m_List.SetItem(&item);

		for (nIdxUnit = 0; nIdxUnit < m_nTotalUnit; nIdxUnit++)
		{
			// "#%d ��" - Torque
			item.iSubItem = 3+(nIdxUnit *2);
			sVal.Format(_T("%.3f"), m_stList.pThick[m_stList.pModel[nIdxModel].nThick].pUnit[nIdxUnit].Torq.dLeft);
			_stprintf(tChar, _T("%s"), sVal);
			item.pszText = tChar;
			m_List.SetItem(&item);
			// "#%d ��" - Torque
			item.iSubItem = 4+(nIdxUnit *2);
			sVal.Format(_T("%.3f"), m_stList.pThick[m_stList.pModel[nIdxModel].nThick].pUnit[nIdxUnit].Torq.dRight);
			_stprintf(tChar, _T("%s"), sVal);
			item.pszText = tChar;
			m_List.SetItem(&item);
		}

		//item.state = LVIS_SELECTED | LVIS_FOCUSED;
		//m_List.InsertItem(&item);
		//m_List.InsertItem(i, m_stList.pModel[i].sModel, 0);
	}

	m_List.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}

CString CPunchingTorqueManagerDlg::GetThicknessName(int nIdx) // ��������[0], ���� �β�[1], �߰� �β�[2], �β��� �β�[3]
{
	CString sName = _T("");
	switch (nIdx)
	{
	case 0:
		sName = THICKNESS_0;
		break;
	case 1:
		sName = THICKNESS_1;
		break;
	case 2:
		sName = THICKNESS_2;
		break;
	case 3:
		sName = THICKNESS_3;
		break;
	default:
		sName = THICKNESS_0;
		break;
	}
	return sName;
}

void CPunchingTorqueManagerDlg::LoadIni()
{
	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("CamSpecDir"), NULL, szData, sizeof(szData), LIST_PATH))
		m_sPathCamSpecDir = CString(szData);
	else
	{
		AfxMessageBox(_T("CamMaster�� Spec���丮 Path�� �����Ǿ� ���� �ʽ��ϴ�."), MB_ICONWARNING | MB_OK);
		m_sPathCamSpecDir = CString(_T(""));
	}
}

void CPunchingTorqueManagerDlg::OnSelchangeComboModel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sModel, sMsg;
	int nThickModel;
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(nIndex, sModel); 
		m_sModel = sModel;

		nThickModel = GetModelThickness(sModel);
		if (nThickModel < 1)
		{
			sMsg.Format(_T("%s ���� ��ũ���� �������� �ʾҽ��ϴ�."), sModel);
			AfxMessageBox(sMsg);
			ResetComboThickModel();
			return;
		}
		ModifyComboThickModel(nThickModel);
		m_sModel = sModel;
		m_nThickModel = nThickModel;
		SelectList(sModel);
	}
}

void CPunchingTorqueManagerDlg::ModifyComboThickModel(int nThick)
{
	if (nThick < 1)
		return;

	int nIndex = nThick - 1;
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->SetCurSel(nIndex);
}

void CPunchingTorqueManagerDlg::OnSelchangeComboThickModel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sThickness = _T("");

	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->GetLBText(nIndex, sThickness);
	}

	if (sThickness.IsEmpty())
		return;

	if (sThickness == THICKNESS_0)
		m_nThickModel = 0;

	if (sThickness == THICKNESS_1)
		m_nThickModel = 1;

	if (sThickness == THICKNESS_2)
		m_nThickModel = 2;

	if (sThickness == THICKNESS_3)
		m_nThickModel = 3;

	ResetEditUnit();
}

void CPunchingTorqueManagerDlg::InitModel()
{
	m_sModel = _T("");
}

void CPunchingTorqueManagerDlg::ModifyModelData()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->ResetContent();

	TCHAR FN[100];
	_stprintf(FN, _T("%s*.*"), m_sPathCamSpecDir);

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->Dir(0x8010, FN);
	int t = 0;

	//	"[..]"�� ���� 
	//((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->DeleteString(0);
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->FindStringExact(-1, _T("[..]"));
	if(nIndex > -1)
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->DeleteString(nIndex);

	int nCount = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCount();

	CString strBuf, strBuf2;
	for (int i = 0; i < nCount; i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(i, strBuf);

		if (strBuf.GetLength() < 3)
			continue;
		// �����̸����� "["�� ���� 
		CString strBuf2 = strBuf.Mid(1, strBuf.GetLength() - 2);
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->InsertString(t, strBuf2);
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->DeleteString(t + 1);
		t++;
	}
}

void CPunchingTorqueManagerDlg::ModifyThicknessData()
{
	// Model Thickness
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->InsertString(0, GetThicknessName(1));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->InsertString(1, GetThicknessName(2));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->InsertString(2, GetThicknessName(3));

	// Unit Thickness
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->InsertString(0, GetThicknessName(1));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->InsertString(1, GetThicknessName(2));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->InsertString(2, GetThicknessName(3));
}



void CPunchingTorqueManagerDlg::OnBnClickedButtonSaveModel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_sModel.IsEmpty())
	{
		AfxMessageBox(_T("Model�� ���õ��� �ʾҽ��ϴ�."));
		return;
	}

	if (m_nThickModel < 1)
	{
		AfxMessageBox(_T("�β��� ���õ��� �ʾҽ��ϴ�."));
		return;
	}

	CString sMsg;
	sMsg.Format(_T("%s ���� �β����� %s�� �����Ͻðڽ��ϱ�?"), m_sModel, GetThicknessName(m_nThickModel));
	if (IDNO == MessageBox(sMsg, _T("����"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	UpdateModel(m_sModel, m_nThickModel);
	UpdateList();
	sMsg.Format(_T("%s ���� �β����� %s�� ����"), m_sModel, GetThicknessName(m_nThickModel));
	Log(sMsg);
}


void CPunchingTorqueManagerDlg::OnBnClickedButtonSaveUnit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sUnit, sTorqueLeft, sTorqueRight;
	GetDlgItem(IDC_EDIT_UNIT)->GetWindowText(sUnit);
	GetDlgItem(IDC_EDIT_TORQUE_LEFT)->GetWindowText(sTorqueLeft);
	GetDlgItem(IDC_EDIT_TORQUE_RIGHT)->GetWindowText(sTorqueRight);

	if (sUnit.IsEmpty())
	{
		AfxMessageBox(_T("ȣ�� ��ȣ�� ���õ��� �ʾҽ��ϴ�."));
		return;
	}

	if (sTorqueLeft.IsEmpty())
	{
		AfxMessageBox(_T("���� ��ũ���� �������� �ʾҽ��ϴ�."));
		return;
	}

	if (sTorqueRight.IsEmpty())
	{
		AfxMessageBox(_T("������ ��ũ���� �������� �ʾҽ��ϴ�."));
		return;
	}

	if (m_nThickUnit < 1)
	{
		AfxMessageBox(_T("�β��� ���õ��� �ʾҽ��ϴ�."));
		return;
	}

	int nUnit = _tstoi(sUnit);
	double dTorqL = _tstof(sTorqueLeft);
	double dTorqR = _tstof(sTorqueRight);

	m_nUnit = nUnit;

	CString sMsg;
	sMsg.Format(_T("#%d ȣ���� %s�� ��ũ���� ��: %.3f , ��: %.3f ���� �����Ͻðڽ��ϱ�?"), nUnit, GetThicknessName(m_nThickUnit), dTorqL, dTorqR);
	if (IDNO == MessageBox(sMsg, _T("����"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	UpdateUnit(nUnit, m_nThickUnit, dTorqL, dTorqR);
	UpdateList();
	sMsg.Format(_T("#%d ȣ���� %s�� ��ũ���� ��: %.3f , ��: %.3f ���� ����"), nUnit, GetThicknessName(m_nThickUnit), dTorqL, dTorqR);
	Log(sMsg);
}

BOOL CPunchingTorqueManagerDlg::UpdateModel(CString sModel, int nThickness)
{
	int nIdx = SearchModel(sModel);

	if (nIdx < 0) // Not Found
	{
		InsertModel(sModel, nThickness);
	}
	else
	{
		ModifyModel(sModel, nThickness);
	}

	return TRUE;
}

int CPunchingTorqueManagerDlg::SearchUnit(int nUnit)
{
	int nIdxUnit;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR sep[] = { _T(",;\r\n\t") };
	TCHAR *token;
	int nListUnit;

	m_nTotalUnit = m_stList.nTotalMachines;

	
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Unit_Nums"), NULL, szData, sizeof(szData), LIST_PATH))
	{
		token = _tcstok(szData, sep);
		for (nIdxUnit = 0; nIdxUnit < m_stList.nTotalMachines; nIdxUnit++)
		{
			nListUnit = _ttoi(token);

			if (nListUnit == nUnit)
				return nIdxUnit;

			if (nIdxUnit < m_stList.nTotalMachines - 1)
				token = _tcstok(NULL, sep);
		}
	}

	return -1; // Not Found
}

void CPunchingTorqueManagerDlg::InsertModel(CString sModel, int nThickness)
{
	CString sModelIdx, strData;
	int nModelIdx = m_stList.nTotalModels;
	sModelIdx.Format(_T("%d"), nModelIdx);
	strData.Format(_T("%s,%d"), sModel, nThickness);
	::WritePrivateProfileString(_T("Model"), sModelIdx, strData, LIST_PATH);
	m_stList.nTotalModels++;

	strData.Format(_T("%d"), m_stList.nTotalModels);
	::WritePrivateProfileString(_T("Info"), _T("Total_Models"), strData, LIST_PATH);
}

void CPunchingTorqueManagerDlg::ModifyModel(CString sModel, int nThickness)
{
	CString sModelIdx, strData;
	int nModelIdx = SearchModel(sModel);
	sModelIdx.Format(_T("%d"), nModelIdx);
	strData.Format(_T("%s,%d"), sModel, nThickness);
	::WritePrivateProfileString(_T("Model"), sModelIdx, strData, LIST_PATH);
}

BOOL CPunchingTorqueManagerDlg::UpdateUnit(int nUnit, int nThickness, double dTorqL, double dTorqR)
{
	int nIdx = SearchUnit(nUnit);

	if (nIdx < 0) // Not Found
	{
		InsertUnit(nUnit, nThickness, dTorqL, dTorqR);
	}
	else
	{
		ModifyUnit(nUnit, nThickness, dTorqL, dTorqR);
	}

	return TRUE;
}

void CPunchingTorqueManagerDlg::InsertUnit(int nUnit, int nThickness, double dTorqL, double dTorqR)
{
	CString sUnits, strItem, strData, strTorq;
	m_nTotalUnit = m_stList.nTotalMachines;

	if (m_nTotalUnit < 0)
		return;

	if (nThickness < 1)
		return;

	if (dTorqL < 0.0 || dTorqR < 0.0)
		return;

	int nUnitIdx = m_nTotalUnit;

	m_nTotalUnit++;
	strData.Format(_T("%d"), m_nTotalUnit);
	::WritePrivateProfileString(_T("Info"), _T("Total_Models"), strData, LIST_PATH);

	TCHAR szData[MAX_PATH];
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Unit_Nums"), NULL, szData, sizeof(szData), LIST_PATH))
	{
		strData = CString(szData);
		if(strData.IsEmpty())
			sUnits.Format(_T("%d"), nUnit);
		else
			sUnits.Format(_T("%s,%d"), strData, nUnit);
	}
	else
		sUnits.Format(_T("%d"), nUnit);
	::WritePrivateProfileString(_T("Info"), _T("Unit_Nums"), sUnits, LIST_PATH);

	strItem.Format(_T("%d"), nThickness);
	sUnits.Format(_T("%d"), nUnit);
	strTorq.Format(_T("%.3f,%.3f"), dTorqL, dTorqR);
	::WritePrivateProfileString(strItem, sUnits, strTorq, LIST_PATH);
}

void CPunchingTorqueManagerDlg::ModifyUnit(int nUnit, int nThickness, double dTorqL, double dTorqR)
{
	CString sUnits, strItem, strTorq;

	if (nThickness < 1)
		return;

	if (dTorqL < 0.0 || dTorqR < 0.0)
		return;

	strItem.Format(_T("%d"), nThickness);
	sUnits.Format(_T("%d"), nUnit);
	strTorq.Format(_T("%.3f,%.3f"), dTorqL, dTorqR);
	::WritePrivateProfileString(strItem, sUnits, strTorq, LIST_PATH);
}

int CPunchingTorqueManagerDlg::GetModelThickness(CString sModel)
{
	int nModelIdx, nThickness;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	nModelIdx = SearchModel(sModel);
	if (nModelIdx < 0)
		return -1; // Not Found

	sModelIdx.Format(_T("%d"), nModelIdx);
	if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
	{
		token = _tcstok(szData, sep);
		sListModel = CString(token);
		token = _tcstok(NULL, sep);
		nThickness = _ttoi(token);
	}
	else
		return -1; // Not found.

	return nThickness;
}

int CPunchingTorqueManagerDlg::SearchModel(CString sModel)
{
	int nModelIdx, nThickness;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	for (nModelIdx = 0; nModelIdx < m_stList.nTotalModels; nModelIdx++)
	{
		sModelIdx.Format(_T("%d"), nModelIdx);
		if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
		{
			token = _tcstok(szData, sep);
			sListModel = CString(token);
			token = _tcstok(NULL, sep);
			nThickness = _ttoi(token);
		}
		else
			sListModel = _T("");

		if (sListModel == sModel)
			return nModelIdx;
	}

	return -1; // Not Found
}

void CPunchingTorqueManagerDlg::StringToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t* wszStr;
	int	nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* ���� ���ѱ��̸� ���� 

	//3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}

BOOL CPunchingTorqueManagerDlg::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// Write Log for Auto
void CPunchingTorqueManagerDlg::Log(CString strMsg, int nType)
{
	CSafeLock lock(&g_LogLock);

	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;

	_stprintf(szPath, LOG_PATH);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();

	switch (nType)
	{
	case 0:
		_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));
		break;
	}

	CString strDate;
	CString strContents;
	CTime now;

	strDate.Format(_T("%s - "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));
	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r");

	CFile file;
	if (file.Open(szFile, CFile::modeWrite) == 0)
	{
		if (file.Open(szFile, CFile::modeCreate | CFile::modeWrite) == 0)
			return;
	}

	char cameraKey[1024];
	StringToChar(strContents, cameraKey);

	file.SeekToEnd();
	int nLenth = strContents.GetLength();
	int nLenth2 = strlen(cameraKey);
	file.Write(cameraKey, nLenth2);
	file.Flush();
	file.Close();
}

void CPunchingTorqueManagerDlg::UpdateList()
{
	m_stList.ReloadList();
	DispList();
}

void CPunchingTorqueManagerDlg::OnChangeEditUnit()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	GetDlgItem(IDC_EDIT_UNIT)->GetWindowText(str);
	int nUnit = _tstoi(str);
	int nUnitIdx = SearchUnit(nUnit);
	if (nUnitIdx < 0) // Not Found.
		return;

	int nThick = m_nThickUnit;
	m_nUnit = nUnit;

	if (nThick < 0)
		nThick = m_nThickModel;

	if(nThick > 0)
		ModifyComboThickUnit(nUnit, nThick);
}

void CPunchingTorqueManagerDlg::ModifyComboThickUnit(int nUnit, int nThick)
{
	if (nThick < 1)
		return;

	int nIndex = nThick - 1;
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->SetCurSel(nIndex);

	ModifyEditTorqueUnit(nUnit, nThick);
}

void CPunchingTorqueManagerDlg::OnSelchangeComboThickUnit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sThickness = _T("");

	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->GetLBText(nIndex, sThickness);
	}

	if (sThickness.IsEmpty())
		return;

	if (sThickness == THICKNESS_0)
		m_nThickUnit = 0;

	if (sThickness == THICKNESS_1)
		m_nThickUnit = 1;

	if (sThickness == THICKNESS_2)
		m_nThickUnit = 2;

	if (sThickness == THICKNESS_3)
		m_nThickUnit = 3;

	ModifyEditTorqueUnit(m_nUnit, m_nThickUnit);
}

void CPunchingTorqueManagerDlg::ModifyEditTorqueUnit(int nUnit, int nThick)
{
	double dTorqL, dTorqR;
	CString sVal;
	if (!GetTorque(nUnit, nThick, dTorqL, dTorqR))
		return;

	sVal.Format(_T("%.3f"), dTorqL);
	GetDlgItem(IDC_EDIT_TORQUE_LEFT)->SetWindowText(sVal);
	sVal.Format(_T("%.3f"), dTorqR);
	GetDlgItem(IDC_EDIT_TORQUE_RIGHT)->SetWindowText(sVal);
}

BOOL CPunchingTorqueManagerDlg::GetTorque(int nUnit, int nThick, double &dTorqL, double &dTorqR)
{
	int nTotalUnit;
	TCHAR szData[MAX_PATH];
	TCHAR sep[] = { _T(",;\r\n\t") };
	TCHAR *token;

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Machines"), NULL, szData, sizeof(szData), LIST_PATH))
		m_nTotalUnit = nTotalUnit = _ttoi(szData);
	else
	{
		m_nTotalUnit = nTotalUnit = 0;
		return FALSE;
	}

	CString sThickIdx, sUnitNum;
	sThickIdx.Format(_T("%d"), nThick);

	sUnitNum.Format(_T("%d"), nUnit);
	if (0 < ::GetPrivateProfileString(sThickIdx, sUnitNum, NULL, szData, sizeof(szData), LIST_PATH))
	{
		token = _tcstok(szData, sep);
		dTorqL = _ttof(token);
		token = _tcstok(NULL, sep);
		dTorqR = _ttof(token);
	}
	else
		return FALSE;

	return TRUE;;
}

void CPunchingTorqueManagerDlg::ResetComboThickModel()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->SetWindowText(_T(""));
	ResetEditUnit();
}

void CPunchingTorqueManagerDlg::ResetEditUnit()
{
	GetDlgItem(IDC_EDIT_UNIT)->SetWindowText(_T(""));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_TORQUE_LEFT)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_TORQUE_RIGHT)->SetWindowText(_T(""));
}

void CPunchingTorqueManagerDlg::OnBnClickedButtonRefreshModel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ModifyModelData();
}


void CPunchingTorqueManagerDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	// �� Ŭ���� �� �ѹ��� �޾ƿ���
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;

	// ���õ� �����۰�(idx)�� ������ [1]��° col�� �������� �����´�.
	CString sModel = m_List.GetItemText(idx, 1);

	// Model ComboBox���� ������ ���� ǥ���Ѵ�.
	int nSel = SearchModelInCombo(sModel);
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetCurSel(nSel);
	int nThickModel = GetModelThickness(sModel);
	if (nThickModel < 1)
	{
		ResetComboThickModel();
		return;
	}
	ModifyComboThickModel(nThickModel);

	m_sModel = sModel;
	m_nThickModel = nThickModel;
}

int CPunchingTorqueManagerDlg::SearchModelInCombo(CString sModel)
{
	int i, idx = -1, nTotalModel = 0;
	CString sModelInCombo;

	nTotalModel = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCount();

	for (i = 0; i < nTotalModel; i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(i, sModelInCombo);

		if (sModelInCombo == sModel)
			return i;
	}

	return idx;
}

void CPunchingTorqueManagerDlg::SelectList(CString sModel)
{
	int i = SearchModel(sModel);

	// ���� ���� ���û��¸� �����մϴ�
	m_List.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
	// ���ϴ� �������� �����մϴ�
	m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	// ���õ� �������� ǥ���մϴ�
	m_List.EnsureVisible(i, false);
	// ����Ʈ ��Ʈ�ѿ� ��Ŀ���� ����ϴ�
	m_List.SetFocus();

	//Ư����ġ ���̶���Ʈ
	m_List.SetSelectionMark(i);
	m_List.EnsureVisible(i, TRUE); //��ũ��
	m_List.SetItemState(m_List.GetSelectionMark(), LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_List.SetFocus();
}

void CPunchingTorqueManagerDlg::OnBnClickedButtonFindModel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sModel, sMsg;
	GetDlgItem(IDC_EDIT_MODEL)->GetWindowText(sModel);
	sModel.MakeUpper();

	int nSel = SearchModelInCombo(sModel);
	if (nSel  < 0)
	{
		sMsg.Format(_T("%s ���� ��ũ���� �������� �ʾҽ��ϴ�."), sModel);
		AfxMessageBox(sMsg);
		return;
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetCurSel(nSel);
	m_sModel = sModel;

	int nThickModel = GetModelThickness(sModel);
	if (nThickModel < 1)
	{
		ResetComboThickModel();
		return;
	}
	ModifyComboThickModel(nThickModel);

	m_sModel = sModel;
	m_nThickModel = nThickModel;

	SelectList(sModel);
}
