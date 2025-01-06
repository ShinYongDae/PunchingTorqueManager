
// PunchingTorqueManagerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "PunchingTorqueManager.h"
#include "PunchingTorqueManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPunchingTorqueManagerDlg 대화 상자



CPunchingTorqueManagerDlg::CPunchingTorqueManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PUNCHINGTORQUEMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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

	InitList();

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
	int i;
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
	m_List.InsertColumn(0, _T("Idx"), LVCFMT_CENTER, 40);//LVCFMT_LEFT
	m_List.InsertColumn(1, _T("모델"), LVCFMT_CENTER, 140);//LVCFMT_LEFT
	m_List.InsertColumn(2, _T("두께구분"), LVCFMT_CENTER, 80);

	for (i = 0; i < m_stList.nTotalMachines; i++)
	{
		strItem.Format(_T("#%d 좌"), m_stList.pUnitList[i]);
		m_List.InsertColumn(3+(i*2), strItem, LVCFMT_CENTER, 60);
		strItem.Format(_T("#%d 우"), m_stList.pUnitList[i]);
		m_List.InsertColumn(4+(i*2), strItem, LVCFMT_CENTER, 60);
	}

	TCHAR tChar[200];
	CString sVal;
	for (i = 0; i < m_stList.nTotalModels; ++i)
	{
		LVITEM item;
		::ZeroMemory(&item, sizeof(item));
		item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		// Index
		item.iItem = m_List.GetItemCount();
		item.iSubItem = 0;
		item.iImage = 1;
		sVal.Format(_T("%d"), i);
		_stprintf(tChar, _T("%s"), sVal);
		item.pszText = tChar;
		m_List.InsertItem(&item);
		// 모델
		item.iSubItem = 1;
		sVal.Format(_T("%d"), i);
		_stprintf(tChar, _T("%s"), m_stList.pModel[i].sModel);
		item.pszText = tChar;
		m_List.SetItem(&item);
/*		// 두께구분
		item.iSubItem = 2;
		item.pszText = _T("신용대의 프로그래밍");
		m_List.SetItem(&item);
		// "#%d 좌"
		item.iSubItem = 2;
		item.pszText = _T("신용대의 프로그래밍");
		m_List.SetItem(&item);
		// "#%d 우"
		item.iSubItem = 2;
		item.pszText = _T("신용대의 프로그래밍");
		m_List.SetItem(&item);
*/
		//item.state = LVIS_SELECTED | LVIS_FOCUSED;
		//m_List.InsertItem(&item);
		//m_List.InsertItem(i, m_stList.pModel[i].sModel, 0);
	}

	m_List.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}