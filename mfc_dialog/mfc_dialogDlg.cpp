
// mfc_dialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfc_dialog.h"
#include "mfc_dialogDlg.h"
#include "afxdialogex.h"
#include "DlgSession.h"
#include "DlgCliWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmfcdialogDlg dialog



CmfcdialogDlg::CmfcdialogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CmfcdialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmfcdialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB_CLI, m_tab);
}

BEGIN_MESSAGE_MAP(CmfcdialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()

	ON_COMMAND(ID_LOGIN, OnLogin)
	ON_COMMAND(ID_LOGOUT, OnLogout)
	ON_COMMAND(ID_AUTO_LOG, OnExecutionLog)
	ON_COMMAND(ID_NEW_SESSION, OnNewSession)
	ON_COMMAND(ID_RECONCT, OnReconnect)
	ON_COMMAND(ID_DISCONNCT, OnDisconnect)
END_MESSAGE_MAP()


// CmfcdialogDlg message handlers

BOOL CmfcdialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitCtrls();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CmfcdialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CmfcdialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CmfcdialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CmfcdialogDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, 0, 0, &rcClient);
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rcClient, &rcClient);
	
	if (m_tab.GetSafeHwnd())
	{
		m_tab.MoveWindow(rcClient);
	}
	
}

void CmfcdialogDlg::OnLogin()
{

}

void CmfcdialogDlg::OnLogout()
{

}

void CmfcdialogDlg::OnExecutionLog()
{

}

void CmfcdialogDlg::OnNewSession()
{
	STSession session;
	session.GenerateId();
	session.SetHost("10.10.5.141");
	session.SetProtocol(STSession::emSsh);

	DlgSession dlg;
	dlg.InitSession(session, FALSE);
	if (IDCANCEL == dlg.DoModal())
	{
		return;
	}

	dlg.GetSession(session);

	OpenSession(session);
}

void CmfcdialogDlg::OnReconnect()
{
	/*std::string str = "\r\n";
	m_interaction->WriteData(str.c_str(), str.length());
	m_interaction->Connect();*/
}

void CmfcdialogDlg::OnDisconnect()
{
	//m_interaction->DisConnect();
}

void CmfcdialogDlg::InitCtrls()
{
	VERIFY(m_toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP /*| CBRS_GRIPPER*/ | CBRS_TOOLTIPS, CRect(4, 4, 0, 0)));
	VERIFY(m_toolbar.LoadToolBar(IDR_TOOLBAR_MAIN));

	m_toolbar.ShowWindow(SW_SHOW);
	//RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	m_menu.LoadMenu(IDR_MENU_MAIN);
	SetMenu(&m_menu);

	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("Tab #1");

	//m_tab.InsertItem(0, &tcItem);
	int nItem = m_tab.InsertItem(0, _T("aaa"), 0);
}

void CmfcdialogDlg::OpenSession(const STSession& session)
{
	int nItem = m_tab.InsertItem(0, _T("aaa"), 0);

	/*CDlgCliWindow* pCliWindow = new CDlgCliWindow(&m_tab);
	pCliWindow->Create(CDlgCliWindow::IDD, &m_tab);*/
	
	//pCliWindow->MoveWindow();

	//m_tab.SetItemData(nItem, pCliWindow);
}

