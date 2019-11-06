#include "terminal_display.h"
#include "cstring_convert.h"
#include "interaction.h"

TerminalDisplay::TerminalDisplay()
	: m_terminal(NULL)
	, m_interaction(NULL)
{

}

TerminalDisplay::~TerminalDisplay()
{

}

void TerminalDisplay::BindTerminal(lw_ui::CTerminal* terminal)
{
	//std::lock_guard<std::mutex> lck(m_mutex);
	m_terminal = terminal;
}

void TerminalDisplay::BindInteraction(lw_live::Interaction* interaction)
{
	m_interaction = interaction;
	m_interaction->GetTerminal()->set_callback([this]() {
		this->m_terminal->SyncFromDelegate();
	});
}

CString TerminalDisplay::GetLineText(int nLine)
{
	std::wstring wstr = m_interaction->GetTerminal()->get_text(ToRow_term(nLine));

	return CStringConverter::to_cstr(wstr);
}

void TerminalDisplay::GetLine(int nLine, std::vector<lw_ui::TextBlock>& vecBlock)
{
	if (m_terminal->IsSelected(nLine))
	{
		CString strLine = GetLineText(nLine);
		CString strTemp;
		BOOL bSelected = FALSE;
		for (int i = 0; i < strLine.GetLength(); ++i)
		{
			BOOL bSelectedTemp = m_terminal->IsSelected(nLine, i);
			BOOL bChanged = bSelected != bSelectedTemp && !strTemp.IsEmpty();
			if (bChanged)
			{
				lw_ui::TextBlock text;
				text.nRow = nLine;
				text.clrForeground = bSelected ? RGB(0, 0, 0) : RGB(255, 255, 255);
				text.strContext = strTemp;
				vecBlock.push_back(text);

				strTemp.Empty();
			}
			
			strTemp.AppendChar(strLine.GetAt(i));
			bSelected = bSelectedTemp;
		}

		lw_ui::TextBlock text;
		text.nRow = nLine;
		text.clrForeground = bSelected ? RGB(0, 0, 0) : RGB(255, 255, 255);
		text.strContext = strTemp;
		vecBlock.push_back(text);
	}
	else
	{
		lw_ui::TextBlock text;
		text.nRow = nLine;
		text.clrForeground = RGB(255, 255, 255);
		text.strContext = GetLineText(nLine);
		vecBlock.push_back(text);
	}
}

int TerminalDisplay::GetTotalLines()
{
	return m_interaction->GetTerminal()->get_lines();
}

CString TerminalDisplay::GetWindowText(const CPoint& ptStart, const CPoint& ptEnd)
{
	std::wstring strContent;
	//Read(ptStart.y, ptStart.x, ptEnd.y, ptEnd.x, strContent);

	return CStringConverter::to_cstr(strContent);
}

void TerminalDisplay::GetCursorPos(int& row, int& col)
{
	m_interaction->GetTerminal()->get_cursor(row, col);
	row = ToRow_ui(row);
}

int TerminalDisplay::GetVScrollBottom()
{
	int top = m_interaction->GetTerminal()->get_vscrollbar();
	int rows = 0;
	int cols = 0;
	m_interaction->GetTerminal()->get_size(rows, cols);

	int bottom = top + rows;
	return ToRow_ui(bottom);
}

void TerminalDisplay::SetVScrollBottom(int bottom)
{
	bottom = ToRow_term(bottom);
	vt_terminal* vtterm = m_interaction->GetTerminal();

	int rows = 0;
	int cols = 0;
	vtterm->get_size(rows, cols);

	int top = bottom - rows;
	vtterm->set_vscrollbar(top);
}

bool TerminalDisplay::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nullptr == m_interaction)
	{
		return false;
	}

	if (nChar == '\r')
	{
		return DoKeyEnter();
	}

	CString cstr;
	cstr.Format(_T("%c"), nChar);
	std::string str = CStringConverter::to_str(cstr);

	m_interaction->SendData(str, false);

	return true;
}

bool TerminalDisplay::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nullptr == m_interaction)
	{
		return false;
	}

	std::function<void(int, std::string&)> func = nullptr;
	switch (nChar)
	{
	case VK_UP:
	{
		func = [this](int nRow, std::string& str) {
			str = "\x1b[A";
		};
		break;
	}
	case VK_DOWN:
	{
		func = [this](int nRow, std::string& str) {
			str = "\x1b[B";
		};
		break;
	}
	case VK_LEFT:
	{
		func = [this](int nRow, std::string& str) {
			str = "\x1b[D";
		};
		break;
	}
	case VK_RIGHT:
	{
		func = [this](int nRow, std::string& str) {
			str = "\x1b[C";
		};
		break;
	}
	case VK_BACK:
	{
		func = [this](int nRow, std::string& str) {
			short keyStata = ::GetKeyState(VK_SHIFT);
			if (keyStata < 0)
			{
				//shift
				str = "\b";
			}
			else
			{
				char ch = 0x7F;	//\b
				str = std::string(1, ch);
			}
		};
		break;
	}
	case VK_LSHIFT:
	case VK_RSHIFT:
	{
		break;
	}
	default:
		break;
	}

	if (nullptr == func)
	{
		return false;
	}

	return DoKeyDown(func);
}

int TerminalDisplay::ToRow_ui(int row)
{
	vt_terminal* vtterm = m_interaction->GetTerminal();
	int sblines = vtterm->get_sblines();
	return row + sblines;
}

int TerminalDisplay::ToRow_term(int row)
{
	vt_terminal* vtterm = m_interaction->GetTerminal();
	int sblines = vtterm->get_sblines();
	return row - sblines;
}

bool TerminalDisplay::DoKeyEnter()
{
	if (nullptr == m_interaction)
	{
		return false;
	}

	m_interaction->SendData("\r", false);

	return true;
}

bool TerminalDisplay::DoKeyDown(std::function<void(int, std::string&)> func)
{
	if (nullptr == m_interaction)
	{
		return false;
	}

	std::string str;
	func(0, str);

	if (!str.empty())
	{
		m_interaction->SendData(str, false);
	}

	return true;
}