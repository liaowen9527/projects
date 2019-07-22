#include "stdafx.h"
#include "DlgSession.h"
#include "AppInstance.h"
#include "STSessionMgr.h"
#include "cstring_convert.h"

#define TEXT_TELNET _T("Telnet")
#define TEXT_SSH _T("SSH")
#define PORT_TELNET 23
#define PORT_SSH 22

DlgSession::DlgSession(CWnd* pParent/* = NULL*/)
	: CDialog(DlgSession::IDD, pParent)
	, m_bEnableSave(FALSE)
{
}

DlgSession::~DlgSession()
{
}

void DlgSession::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_HOST, m_strHost);
	DDX_Text(pDX, IDC_EDIT_PORT, m_strPort);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edtName);
	DDX_Control(pDX, IDC_EDIT_HOST, m_edtHost);
	DDX_Control(pDX, IDC_EDIT_PORT, m_edtPort);
	DDX_Control(pDX, IDC_COMBO_PROTOCOL, m_cmbAccessMode);
	DDX_Control(pDX, IDC_EDIT_USER, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_CHECK_SAVE, m_chkSave);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnConn);
}

BEGIN_MESSAGE_MAP(DlgSession, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_CBN_SELCHANGE(IDC_COMBO_PROTOCOL, OnChangeProtocol)
	ON_EN_CHANGE(IDC_EDIT_HOST, OnEnChangeEditHost)
	ON_BN_CLICKED(IDC_CHECK_SAVE, &DlgSession::OnBnClickedCheckSave)
END_MESSAGE_MAP()

BOOL DlgSession::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateData(FALSE);

	m_chkSave.SetCheck(FALSE);
	m_cmbAccessMode.AddString(TEXT_TELNET);
	m_cmbAccessMode.AddString(TEXT_SSH);

	if (m_bEnableSave)
	{
		m_btnConn.SetWindowText(_T("Save"));
		m_chkSave.EnableWindow(FALSE);
	}

	if (m_defaultSession.GetId().empty())
	{
		m_cmbAccessMode.SelectString(0, TEXT_TELNET);
	}
	else
	{
		InitCtrls(m_defaultSession);
	}

	OnBnClickedCheckSave();
	OnChangeProtocol();

	GetDlgItem(IDC_EDIT_HOST)->SetFocus();

	return FALSE;
}

void DlgSession::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
}

void DlgSession::InitSession(const STSession& session, BOOL enableSave)
{
	m_defaultSession = session;
	m_bEnableSave = enableSave;
}

void DlgSession::GetSession(STSession& session)
{
	session = m_session;
	if (session.GetName().empty())
	{
		session.SetName(session.GetHost());
	}
}

bool DlgSession::Connect()
{
	UpdateData(TRUE);

	m_strName.Trim();
	m_strHost.Trim();

	auto saveSession = m_chkSave.GetCheck();
	if (saveSession && m_strName.IsEmpty())
	{
		MessageBox(_T("Please enter a connection name."), NULL, MB_ICONWARNING);
		return FALSE;
	}

	if (!CheckName(m_strName) || !CheckHostName(m_strHost))
	{
		return false;
	}

	std::string strNewName = CStringConverter::to_str(m_strName);
	if (m_defaultSession.GetName() != strNewName)
	{
		if (saveSession && AppInstance::Instance()->GetSessionMgr()->FindByName(strNewName))
		{
			MessageBox(_T("The connection name already exists."), nullptr, MB_ICONWARNING);
			return false;
		}
	}

	if (m_strPort.GetLength() > 6 || m_strPort.SpanIncluding(_T("0123456789")) != m_strPort)
	{
		MessageBox(_T("Invalid port."), nullptr, MB_ICONWARNING);
		return false;
	}

	GetSessionFromUI(m_session);
	if (!m_bEnableSave)
	{
		m_session.GenerateId();
		if (saveSession)
		{
			SaveSession(m_session);
		}
	}

	return true;
}

void DlgSession::OnShow()
{
	m_edtName.SetFocus();
}

void DlgSession::OnBtnConnect()
{
	if (!Connect())
	{
		return;
	}

	__super::OnOK();
}

void DlgSession::OnChangeProtocol()
{
	UpdateData(TRUE);

	STSession::EnumProtocol emProtocol = GetProtocolType();
	if (IsNeedChangePort(emProtocol))
	{
		if (emProtocol == STSession::emTelnet)
		{
			m_strPort.Format(_T("%d"), PORT_TELNET);
		}
		else if (emProtocol == STSession::emSsh)
		{
			m_strPort.Format(_T("%d"), PORT_SSH);
		}
	}

	m_lastProtocol = emProtocol;

	UpdateData(FALSE);
}

void DlgSession::InitCtrls(const STSession& sesson)
{
	m_strName = CStringConverter::to_cstr(sesson.GetName());
	m_strHost = CStringConverter::to_cstr(sesson.GetHost());
	if (m_strHost.IsEmpty())
	{
		m_strHost = CStringConverter::to_cstr(sesson.GetIp());
	}
	m_strPort.Format(_T("%d"), sesson.GetPort());

	CString strProtocol = GetProtocol(sesson.GetProtocol());
	m_cmbAccessMode.SelectString(0, strProtocol);

	CString strUser = CStringConverter::to_cstr(sesson.GetUser());
	CString strPassword = CStringConverter::to_cstr(sesson.GetPassword());
	m_editUserName.SetWindowText(strUser);
	m_editPassword.SetWindowText(strPassword);

	UpdateData(FALSE);
}

STSession::EnumProtocol DlgSession::GetProtocolType()
{
	if (!m_cmbAccessMode.GetSafeHwnd())
	{
		return STSession::emTelnet;
	}
	CString str;
	m_cmbAccessMode.GetWindowText(str);
	
	return GetProtocolType(str);
}

STSession::EnumProtocol DlgSession::GetProtocolType(const CString& strProtocol)
{
	if (strProtocol == TEXT_SSH)
	{
		return STSession::emSsh;
	}

	return STSession::emTelnet;
}

CString DlgSession::GetProtocol(STSession::EnumProtocol emType)
{
	if (emType == STSession::emTelnet)
	{
		return TEXT_TELNET;
	}

	return TEXT_SSH;
}

bool DlgSession::IsNeedChangePort(STSession::EnumProtocol type)
{
	if (m_strPort.IsEmpty())
	{
		return true;
	}

	if (m_lastProtocol == type)
	{
		return false;
	}

	int nPort = GetPort();
	if (STSession::emTelnet == m_lastProtocol && PORT_TELNET == nPort)
	{
		return true;
	}
	if (STSession::emSsh == m_lastProtocol && PORT_SSH == nPort)
	{
		return true;
	}

	return false;
}

BOOL DlgSession::CheckName(const CString& strName)
{
	if (strName.GetLength() > 128) 
	{
		MessageBox(_T("A connection name cannot exceed 128 characters."), NULL, MB_ICONWARNING);
		return FALSE;
	}
	
	/*CString strSensitive = _T(",\\/:*?\"<>|'");
	if (strName.FindOneOf(strSensitive))
	{
		CString strMsg;
		strMsg.Format(_T("The name cannot contain any of the following characters: %s"), strSensitive);
		MessageBox(strMsg, NULL, MB_ICONWARNING);
		return FALSE;
	}*/

	return TRUE;
}

BOOL DlgSession::CheckHostName(const CString& strHostName)
{
	if (strHostName.IsEmpty())
	{
		MessageBox(_T("Please enter a hostname."), NULL, MB_ICONWARNING);
		return FALSE;
	}

	if (strHostName.GetLength() > 128)
	{
		MessageBox(_T("A hostname cannot exceed 128 characters."), NULL, MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}

int DlgSession::GetPort()
{
	return _ttoi(m_strPort);
}

void DlgSession::GetSessionFromUI(STSession& session)
{
	session = m_defaultSession;
	session.SetId("");

	session.SetName(CStringConverter::to_str(m_strName));
	session.SetHost(CStringConverter::to_str(m_strHost));
 	session.SetLocal(false);	
	session.SetIp(CStringConverter::to_str(m_strHost));
	session.SetPort(GetPort());

	CString strUser, strPassword, strProtocol, strKeyfile;
	m_cmbAccessMode.GetWindowText(strProtocol);
	m_editUserName.GetWindowText(strUser);
	m_editPassword.GetWindowText(strPassword);

	session.SetProtocol(GetProtocolType(strProtocol));
	session.SetUser(CStringConverter::to_str(strUser));
	session.SetPassword(CStringConverter::to_str(strPassword));
	//session.SetPrivateKey(StringConverter::Cstr2Ansi(strKeyfile));
}

void DlgSession::SaveSession(const STSession& session)
{
	AppInstance::Instance()->GetSessionMgr()->Add(session);
}

void DlgSession::OnEnChangeEditHost()
{
	auto oldHost = m_strHost;
	UpdateData(TRUE);
	if (m_strName.IsEmpty() || m_strName == oldHost)
	{
		if (m_chkSave.GetCheck())
		{
			m_strName = m_strHost;
		}
	}
	UpdateData(FALSE);
}

void DlgSession::OnCancel()
{
	__super::OnCancel();
}

void DlgSession::OnBnClickedCheckSave()
{
	auto checked = m_chkSave.GetCheck();
	m_edtName.EnableWindow(checked);
	m_edtName.SetWindowText(_T(""));
}
