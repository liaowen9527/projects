
// mfc_dialogDlg.h : header file
//

#pragma once
#include "terminal/Terminal.h"
#include "interaction.h"
#include "terminal_display.h"
#include "STSession.h"

class CDlgCliWindow : public CDialogEx
{
public:
	CDlgCliWindow(CWnd* pParent = NULL);
	~CDlgCliWindow();

	enum { IDD = IDD_DLG_CHILD };

public:
	void InitSession(const STSession& session);

	lw_live::InteractionPtr GetInteraction();

protected:
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();

protected:
	lw_ui::CTerminal m_terminal;
	lw_live::InteractionPtr m_interaction;
	TerminalDisplayPtr m_display;

	STSession m_session;
};