#include "stdafx.h"
#include "PaintPanel.h"


CPaintPanel::CPaintPanel()
{
	m_bAutoMode = FALSE;
	m_bMouseDown = FALSE;
}


CPaintPanel::~CPaintPanel()
{
}

void CPaintPanel::OnPaint(CDCHandle dc)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(&ps);
	CDC cdc(hdc);

	HBRUSH hBrush;
	hBrush = CreateSolidBrush(RGB(26, 26, 26));
	cdc.SelectBrush(hBrush);

	CRect rect;
	GetClientRect(rect);

	cdc.Rectangle(rect);
	DeleteObject(hBrush);
	
	EndPaint(&ps);
}


void CPaintPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bMouseDown = FALSE;

	if (m_bAutoMode)
	{
		SendWriteInfo();
		InvalidateRect(NULL);
	}
	SetMsgHandled(FALSE);
}


void CPaintPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//SetCapture();
	m_bMouseDown = TRUE;
	m_ptOrgin = point;
	SetMsgHandled(FALSE);
}

void CPaintPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bMouseDown)
	{
		//int nXPos = (int)(point.x / m_xOffset);
		//int nYPos = (int)(point.y / m_yOffset);
		//m_arrayPos[nXPos % 200][nYPos % 200] = 1;
		//CRect rect(point.x - 5, point.y - 5, point.x + 5, point.y + 5);
		//InvalidateRect(rect);

		CDC dc(GetDC());

		CPen pen;
		pen.CreatePen(PS_SOLID, 13, RGB(240, 0, 0));

		dc.SelectPen(pen);

		dc.MoveTo(m_ptOrgin);
		dc.LineTo(point);

		m_ptOrgin = point;
	}
	SetMsgHandled(FALSE);
}

LRESULT CPaintPanel::OnClearPanel(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Invalidate();
	return 0;
}

LRESULT CPaintPanel::OnGetWriteInfo(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SendWriteInfo(HWND(wParam));
	return 0;
}

int CPaintPanel::SendWriteInfo(HWND hWnd /*= NULL*/)
{
	CRect rect;
	GetClientRect(rect);

	int nWidth = rect.Width();
	int nHeight = rect.Height();

	CClientDC  dc(this->m_hWnd);

	CDC dcMem;
	dcMem.CreateCompatibleDC(dc);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, 20, 20);
	dcMem.SelectBitmap(bmp);

	StretchBlt(dcMem, 0, 0, 20, 20, dc, 1, 1, nWidth - 2, nHeight - 2, SRCCOPY);
	//dcMem.BitBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY);

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			COLORREF pixel = dcMem.GetPixel(i, j);
			BYTE bRed = GetRValue(pixel);

			m_dParam[i * 20 + j] = bRed;
		}
	}

	if (NULL == hWnd)
	{
		hWnd = GetParent();
	}
	::SendMessage(hWnd, WM_SEND_WRITE, MAKEWPARAM(0, 0), (LPARAM)m_dParam);
	return 0;
}

int CPaintPanel::SetAutoMode(BOOL bAutoMode)
{
	m_bAutoMode = bAutoMode;
	return 0;
}
