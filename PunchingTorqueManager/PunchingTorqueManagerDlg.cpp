
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
	m_sModel = _T("");
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
	int nIdxModel, nIdxUnit;
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
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // ���� �߰�
	m_List.InsertColumn(0, _T("Idx"), LVCFMT_CENTER, 60);//LVCFMT_LEFT
	m_List.InsertColumn(1, _T("��"), LVCFMT_CENTER, 140);//LVCFMT_LEFT
	m_List.InsertColumn(2, _T("�β�����"), LVCFMT_CENTER, 100);

	for (nIdxUnit = 0; nIdxUnit < m_stList.nTotalMachines; nIdxUnit++)
	{
		strItem.Format(_T("#%d ��"), m_stList.pUnitList[nIdxUnit]);
		m_List.InsertColumn(3+(nIdxUnit *2), strItem, LVCFMT_CENTER, 100);
		strItem.Format(_T("#%d ��"), m_stList.pUnitList[nIdxUnit]);
		m_List.InsertColumn(4+(nIdxUnit *2), strItem, LVCFMT_CENTER, 100);
	}

	TCHAR tChar[200];
	CString sVal;
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

		for (nIdxUnit = 0; nIdxUnit < m_stList.nTotalMachines; nIdxUnit++)
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
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(nIndex, m_sModel); 
	}
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

	CString sMsg;
	sMsg.Format(_T("#%d ȣ���� %s�� ��ũ���� ��: %.3f , ��: %.3f ���� �����Ͻðڽ��ϱ�?"), nUnit, GetThicknessName(m_nThickUnit), dTorqL, dTorqR);
	if (IDNO == MessageBox(sMsg, _T("����"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	UpdateUnit(nUnit, m_nThickUnit, dTorqL, dTorqR);
	sMsg.Format(_T("#%d ȣ���� %s�� ��ũ���� ��: %.3f , ��: %.3f ���� ����"), nUnit, GetThicknessName(m_nThickUnit), dTorqL, dTorqR);
	Log(sMsg);
}

BOOL CPunchingTorqueManagerDlg::UpdateModel(CString sModel, int nThickness)
{
	return TRUE;
}

BOOL CPunchingTorqueManagerDlg::UpdateUnit(int nUnit, int nThickness, double dTorqL, double dTorqR)
{
	return TRUE;
}

void CPunchingTorqueManagerDlg::StringToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t*	wszStr;
	int				nLenth;

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
