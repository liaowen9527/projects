
// mfc_dialogDlg.h : header file
//

#pragma once
#include "controls/DragTabCtrl.h"
#include "STSession.h"

// CmfcdialogDlg dialog
class CmfcdialogDlg : public CDialogEx
{
// Construction
public:
	CmfcdialogDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DLG_MAIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnLogin();
	afx_msg void OnLogout();
	afx_msg void OnExecutionLog();
	afx_msg void OnNewSession();
	afx_msg void OnReconnect();
	afx_msg void OnDisconnect();

protected:
	void InitCtrls();

	void OpenSession(const STSession& session);

protected:
	CDragTabCtrl m_tab;
	CMenu m_menu;
	CToolBar m_toolbar;
};
