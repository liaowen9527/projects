#pragma once
#include "display_buffer_limit.h"
#include "terminal/Terminal.h"

namespace lw_live {
	class Interaction;
}

class TerminalDisplay : public lw_live::DisplayBufferLimit, public lw_ui::TerminalDelegate
{
public:
	TerminalDisplay();
	~TerminalDisplay();

	void BindTerminal(lw_ui::CTerminal* terminal);
	void BindInteraction(lw_live::Interaction* interaction);

protected:
	virtual void SetModify_unsafe(int nRow, bool bValue = true);
	virtual void SyncSeekP(size_t nRow, size_t nCol);

protected:
	virtual CString GetLineText(int nLine);
	virtual void GetLine(int nLine, std::vector<lw_ui::TextBlock>& vecBlock);
	virtual int GetTotalLines();

	virtual CString GetWindowText(const CPoint& ptStart, const CPoint& ptEnd);

	virtual bool OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	bool DoKeyEnter();
	bool DoKeyDown(std::function<void(int, std::string&)> func);

protected:
	lw_ui::CTerminal* m_terminal;
	lw_live::Interaction* m_interaction;
};

typedef std::shared_ptr<TerminalDisplay> TerminalDisplayPtr;


