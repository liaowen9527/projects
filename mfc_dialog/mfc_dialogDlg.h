
// mfc_dialogDlg.h : header file
//

#pragma once
#include "terminal/Terminal.h"
#include "interaction.h"
#include "terminal_display.h"

// CmfcdialogDlg dialog
class CmfcdialogDlg : public CDialogEx
{
// Construction
public:
	CmfcdialogDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_DIALOG_DIALOG };
#endif

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

protected:
	virtual void OnCancel();
	void TestCli();

protected:
	lw_ui::CTerminal m_terminal;
	lw_live::InteractionPtr m_interaction;
	TerminalDisplayPtr m_display;
};
