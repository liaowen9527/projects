
// mfc_dialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfc_dialog.h"
#include "mfc_dialogDlg.h"
#include "afxdialogex.h"
#include "ssh_client_param.h"
#include "client_factory.h"
#include "terminal_display.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace lw_client;
using namespace lw_live;

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
	: CDialogEx(IDD_MFC_DIALOG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmfcdialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TERMINAL, m_terminal);
}

BEGIN_MESSAGE_MAP(CmfcdialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
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
	m_display = std::make_shared<TerminalDisplay>();
	m_display->BindTerminal(&m_terminal);

	m_terminal.SetDelegate(m_display.get());

	ClientFactory::InitEnv();
	TestCli();

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

void CmfcdialogDlg::OnCancel()
{
	m_display->BindTerminal(NULL);
	m_display->BindInteraction(nullptr);

	__super::OnCancel();
}

void CmfcdialogDlg::TestCli()
{
	SshClientParamPtr clientParam = std::make_shared<SshClientParam>("10.10.3.253");
	clientParam->SetPort(22);
	clientParam->SetUsername("");
	clientParam->SetPassword("");
	clientParam->SetSshVersion(ssh2);

	LiveParamPtr liveParam = std::make_shared<LiveParam>();
	liveParam->SetClientParam(clientParam);

	DestinationPtr destPtr = std::make_shared<Destination>();
	lw_util::Tree<LiveParamPtr>& clients = destPtr->m_clients;
	clients.InsertItem(liveParam);

	m_interaction = std::make_shared<Interaction>();
	m_interaction->SetDestination(destPtr);
	m_interaction->SetDisplay(m_display);

	m_display->BindInteraction(m_interaction.get());

	m_interaction->Connect();
}

