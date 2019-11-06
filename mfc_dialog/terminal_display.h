#pragma once
#include "terminal/Terminal.h"

namespace lw_live {
	class Interaction;
}

class TerminalDisplay : public lw_ui::TerminalDelegate
{
public:
	TerminalDisplay();
	~TerminalDisplay();

	void BindTerminal(lw_ui::CTerminal* terminal);
	void BindInteraction(lw_live::Interaction* interaction);

protected:
	virtual CString GetLineText(int nLine);
	virtual void GetLine(int nLine, std::vector<lw_ui::TextBlock>& vecBlock);
	virtual int GetTotalLines();

	virtual CString GetWindowText(const CPoint& ptStart, const CPoint& ptEnd);
	virtual void GetCursorPos(int& row, int& col);
	virtual int GetVScrollBottom();
	virtual void SetVScrollBottom(int bottom);

	virtual bool OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	int ToRow_ui(int row);
	int ToRow_term(int row);

protected:
	bool DoKeyEnter();
	bool DoKeyDown(std::function<void(int, std::string&)> func);

protected:
	lw_ui::CTerminal* m_terminal;
	lw_live::Interaction* m_interaction;
};

typedef std::shared_ptr<TerminalDisplay> TerminalDisplayPtr;


