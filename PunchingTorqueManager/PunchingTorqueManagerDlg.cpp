
// PunchingTorqueManagerDlg.cpp : 구현 파일
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

// CPunchingTorqueManagerDlg 대화 상자


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


// CPunchingTorqueManagerDlg 메시지 처리기

BOOL CPunchingTorqueManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_EDIT_UNIT)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_STATIC_UNIT)->ShowWindow(SW_HIDE);

	LoadIni();
	InitList();
	ModifyModelData();
	ModifyThicknessData();

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SelectString(0, m_sModel);
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetWindowText(m_sModel);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPunchingTorqueManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPunchingTorqueManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPunchingTorqueManagerDlg::OnBnClickedBtnExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// 아이디
	item.iItem = m_List.GetItemCount(); //5;
	item.iSubItem = 0;
	item.iImage = 1;
	item.pszText = _T("1등");
	m_List.InsertItem(&item);
	// 이름
	item.iSubItem = 1;
	item.pszText = _T("신용대");
	m_List.SetItem(&item);
	// 소속
	item.iSubItem = 2;
	item.pszText = _T("신용대의 프로그래밍");
	m_List.SetItem(&item);
	//item.state = LVIS_SELECTED | LVIS_FOCUSED;
	//m_List.InsertItem(&item);
*/
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // 격자 추가
	m_List.InsertColumn(0, _T("Idx"), LVCFMT_CENTER, 60);//LVCFMT_LEFT
	m_List.InsertColumn(1, _T("모델"), LVCFMT_CENTER, 140);//LVCFMT_LEFT
	m_List.InsertColumn(2, _T("두께구분"), LVCFMT_CENTER, 100);

	for (nIdxUnit = 0; nIdxUnit < m_stList.nTotalMachines; nIdxUnit++)
	{
		strItem.Format(_T("#%d 좌"), m_stList.pUnitList[nIdxUnit]);
		m_List.InsertColumn(3+(nIdxUnit *2), strItem, LVCFMT_CENTER, 100);
		strItem.Format(_T("#%d 우"), m_stList.pUnitList[nIdxUnit]);
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
		// 모델
		item.iSubItem = 1;
		_stprintf(tChar, _T("%s"), m_stList.pModel[nIdxModel].sModel);
		item.pszText = tChar;
		m_List.SetItem(&item);
		// 두께구분
		item.iSubItem = 2;
		_stprintf(tChar, _T("%s"), GetThicknessName(m_stList.pModel[nIdxModel].nThick));
		item.pszText = tChar;
		m_List.SetItem(&item);

		for (nIdxUnit = 0; nIdxUnit < m_stList.nTotalMachines; nIdxUnit++)
		{
			// "#%d 좌" - Torque
			item.iSubItem = 3+(nIdxUnit *2);
			sVal.Format(_T("%.3f"), m_stList.pThick[m_stList.pModel[nIdxModel].nThick].pUnit[nIdxUnit].Torq.dLeft);
			_stprintf(tChar, _T("%s"), sVal);
			item.pszText = tChar;
			m_List.SetItem(&item);
			// "#%d 우" - Torque
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

CString CPunchingTorqueManagerDlg::GetThicknessName(int nIdx) // 설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]
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
		AfxMessageBox(_T("CamMaster의 Spec디렉토리 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		m_sPathCamSpecDir = CString(_T(""));
	}
}

void CPunchingTorqueManagerDlg::OnSelchangeComboModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(nIndex, m_sModel); 
	}
}

void CPunchingTorqueManagerDlg::OnSelchangeComboThickModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	//	"[..]"를 제거 
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
		// 기종이름에서 "["를 제거 
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_sModel.IsEmpty())
	{
		AfxMessageBox(_T("Model이 선택되지 않았습니다."));
		return;
	}

	if (m_nThickModel < 1)
	{
		AfxMessageBox(_T("두께가 선택되지 않았습니다."));
		return;
	}

	CString sMsg;
	sMsg.Format(_T("%s 모델의 두께값을 %s로 변경하시겠습니까?"), m_sModel, GetThicknessName(m_nThickModel));
	if (IDNO == MessageBox(sMsg, _T("주의"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	UpdateModel(m_sModel, m_nThickModel);
	sMsg.Format(_T("%s 모델의 두께값을 %s로 변경"), m_sModel, GetThicknessName(m_nThickModel));
	Log(sMsg);
}


void CPunchingTorqueManagerDlg::OnBnClickedButtonSaveUnit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sUnit, sTorqueLeft, sTorqueRight;
	GetDlgItem(IDC_EDIT_UNIT)->GetWindowText(sUnit);
	GetDlgItem(IDC_EDIT_TORQUE_LEFT)->GetWindowText(sTorqueLeft);
	GetDlgItem(IDC_EDIT_TORQUE_RIGHT)->GetWindowText(sTorqueRight);

	if (sUnit.IsEmpty())
	{
		AfxMessageBox(_T("호기 번호가 선택되지 않았습니다."));
		return;
	}

	if (sTorqueLeft.IsEmpty())
	{
		AfxMessageBox(_T("왼쪽 토크값이 설정되지 않았습니다."));
		return;
	}

	if (sTorqueRight.IsEmpty())
	{
		AfxMessageBox(_T("오른쪽 토크값이 설정되지 않았습니다."));
		return;
	}

	if (m_nThickUnit < 1)
	{
		AfxMessageBox(_T("두께가 선택되지 않았습니다."));
		return;
	}

	int nUnit = _tstoi(sUnit);
	double dTorqL = _tstof(sTorqueLeft);
	double dTorqR = _tstof(sTorqueRight);

	CString sMsg;
	sMsg.Format(_T("#%d 호기의 %s의 토크값을 좌: %.3f , 우: %.3f 으로 변경하시겠습니까?"), nUnit, GetThicknessName(m_nThickUnit), dTorqL, dTorqR);
	if (IDNO == MessageBox(sMsg, _T("주의"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	UpdateUnit(nUnit, m_nThickUnit, dTorqL, dTorqR);
	sMsg.Format(_T("#%d 호기의 %s의 토크값을 좌: %.3f , 우: %.3f 으로 변경"), nUnit, GetThicknessName(m_nThickUnit), dTorqL, dTorqR);
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
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

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
