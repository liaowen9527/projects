#pragma once
#include "STSession.h"

class DlgSession : public CDialog
{
public:
	DlgSession(CWnd* pParent = NULL);
	~DlgSession();

	enum { IDD = IDD_DLG_SESSION };

public:
	void InitSession(const STSession& session, BOOL enableSave);
	void GetSession(STSession& session);
	bool Connect();
	void OnShow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeProtocol();
	afx_msg void OnEnChangeEditHost();
	afx_msg void OnBtnConnect();
	afx_msg void OnBnClickedCheckSave();

protected:
	virtual void OnCancel();

protected:
	void InitCtrls(const STSession& sesson);

protected:
	STSession::EnumProtocol GetProtocolType();
	STSession::EnumProtocol GetProtocolType(const CString& strProtocol);
	CString GetProtocol(STSession::EnumProtocol emType);
	bool IsNeedChangePort(STSession::EnumProtocol type);

	BOOL CheckName(const CString& strName);
	BOOL CheckHostName(const CString& strHostName);
	int GetPort();

protected:
	void GetSessionFromUI(STSession& session);
	void SaveSession(const STSession& session);

protected:
	STSession m_defaultSession;
	CString m_strName;
	CString m_strHost;
	CString m_strPort;
	CComboBox m_cmbAccessMode;
	CEdit m_editUserName;
	CEdit m_editPassword;
	CButton m_chkSave;
	CButton m_btnConn;
	CEdit m_edtName;
	CEdit m_edtHost;
	CEdit m_edtPort;

	BOOL m_bEnableSave;

protected:
	STSession::EnumProtocol m_lastProtocol;
	STSession m_session;
};

