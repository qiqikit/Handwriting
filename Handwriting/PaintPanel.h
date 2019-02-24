#pragma once

#define WM_SEND_WRITE (WM_USER+100)
#define WM_GET_WRITE (WM_USER+101)
#define WM_CLEANUP (WM_USER+102)

class CPaintPanel :
	public CWindowImpl<CPaintPanel, CStatic>
{
public:
	CPaintPanel();
	virtual ~CPaintPanel();
public:
	BEGIN_MSG_MAP_EX(CPaintPanel)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)

		MESSAGE_HANDLER(WM_CLEANUP, OnClearPanel)
		MESSAGE_HANDLER(WM_GET_WRITE, OnGetWriteInfo)
	END_MSG_MAP()
	
public:
	void OnPaint(CDCHandle dc);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);

	LRESULT OnClearPanel(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetWriteInfo(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	int SendWriteInfo(HWND hWnd = NULL);
	int SetAutoMode(BOOL bAutoMode);
private:
	BOOL m_bAutoMode;
	BOOL m_bMouseDown;
	double m_dParam[20 * 20];

	CPoint m_ptOrgin;
};

