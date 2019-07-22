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
	std::lock_guard<std::mutex> lck(m_mutex);
	m_terminal = terminal;
}

void TerminalDisplay::BindInteraction(lw_live::Interaction* interaction)
{
	m_interaction = interaction;
}

void TerminalDisplay::SetModify_unsafe(int nRow, bool bValue)
{
	if (m_terminal)
	{
		m_terminal->Invalidate();
	}
}

void TerminalDisplay::SyncSeekP(size_t nRow, size_t nCol)
{
	__super::SyncSeekP(nRow, nCol);

	if (m_terminal)
	{
		m_terminal->SetCurPos(m_nIndexRow, m_nIndexCol);
		m_terminal->Invalidate();
	}
}

CString TerminalDisplay::GetLineText(int nLine)
{
	std::wstring wstr;
	ReadLine(nLine, wstr, false);

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
	size_t row = 0;
	size_t col = 0;
	GetEndPos(row, col);

	return row + 1;
}

CString TerminalDisplay::GetWindowText(const CPoint& ptStart, const CPoint& ptEnd)
{
	std::wstring strContent;
	Read(ptStart.y, ptStart.x, ptEnd.y, ptEnd.x, strContent);

	return CStringConverter::to_cstr(strContent);
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