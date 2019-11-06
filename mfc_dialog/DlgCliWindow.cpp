#include "DlgCliWindow.h"
#include "AppInstance.h"

using namespace lw_client;
using namespace lw_live;

CDlgCliWindow::CDlgCliWindow(CWnd* pParent /*= NULL*/)
{

}

CDlgCliWindow::~CDlgCliWindow()
{

}

BEGIN_MESSAGE_MAP(CDlgCliWindow, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CDlgCliWindow::OnInitDialog()
{
	__super::OnInitDialog();

	CRect rcClient;
	GetClientRect(&rcClient);
	m_terminal.Create(_T(""), WS_CHILD, rcClient, this, IDC_STATIC_TERMINAL);

	m_terminal.ShowWindow(SW_SHOW);

	m_display = std::make_shared<TerminalDisplay>();
	m_display->BindTerminal(&m_terminal);

	m_terminal.SetDelegate(m_display.get());

	DestinationPtr destPtr = AppInstance::Instance()->GetDestination(&m_session);

	m_interaction = std::make_shared<Interaction>();
	m_interaction->SetDestination(destPtr);
	//m_interaction->SetDisplay(m_display);

	m_display->BindInteraction(m_interaction.get());

	m_interaction->Connect();

	return TRUE;
}

void CDlgCliWindow::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);

	if (m_terminal.GetSafeHwnd())
	{
		m_terminal.MoveWindow(rcClient);
	}
}

void CDlgCliWindow::OnClose()
{
	m_display->BindTerminal(NULL);
	m_display->BindInteraction(nullptr);

	__super::OnCancel();
}

void CDlgCliWindow::InitSession(const STSession& session)
{
	m_session = session;
}

InteractionPtr CDlgCliWindow::GetInteraction()
{
	return m_interaction;
}