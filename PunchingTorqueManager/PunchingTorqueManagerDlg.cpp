
// PunchingTorqueManagerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "PunchingTorqueManager.h"
#include "PunchingTorqueManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPunchingTorqueManagerDlg ��ȭ ����



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

	InitList();

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
	int i;
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
	m_List.InsertColumn(0, _T("Idx"), LVCFMT_CENTER, 40);//LVCFMT_LEFT
	m_List.InsertColumn(1, _T("��"), LVCFMT_CENTER, 140);//LVCFMT_LEFT
	m_List.InsertColumn(2, _T("�β�����"), LVCFMT_CENTER, 80);

	for (i = 0; i < m_stList.nTotalMachines; i++)
	{
		strItem.Format(_T("#%d ��"), m_stList.pUnitList[i]);
		m_List.InsertColumn(3+(i*2), strItem, LVCFMT_CENTER, 60);
		strItem.Format(_T("#%d ��"), m_stList.pUnitList[i]);
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
		// ��
		item.iSubItem = 1;
		sVal.Format(_T("%d"), i);
		_stprintf(tChar, _T("%s"), m_stList.pModel[i].sModel);
		item.pszText = tChar;
		m_List.SetItem(&item);
/*		// �β�����
		item.iSubItem = 2;
		item.pszText = _T("�ſ���� ���α׷���");
		m_List.SetItem(&item);
		// "#%d ��"
		item.iSubItem = 2;
		item.pszText = _T("�ſ���� ���α׷���");
		m_List.SetItem(&item);
		// "#%d ��"
		item.iSubItem = 2;
		item.pszText = _T("�ſ���� ���α׷���");
		m_List.SetItem(&item);
*/
		//item.state = LVIS_SELECTED | LVIS_FOCUSED;
		//m_List.InsertItem(&item);
		//m_List.InsertItem(i, m_stList.pModel[i].sModel, 0);
	}

	m_List.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}