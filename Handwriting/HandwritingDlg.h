// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PaintPanel.h"

class CHandwritingDlg : 
	public CDialogImpl<CHandwritingDlg>,
	public CWinDataExchange<CHandwritingDlg>
{
public:
	enum { IDD = IDD_HANDWRITING };

	CHandwritingDlg();
	~CHandwritingDlg();

	BEGIN_MSG_MAP_EX(CHandwritingDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_SEND_WRITE, OnBnClickedRecognize)

		COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER_EX(IDC_RECONGNIZE, OnBnClickedRecongnize)
		COMMAND_ID_HANDLER_EX(IDC_AUTO_RECONGNIZE, OnBnClickedAutoRecongnize)
		COMMAND_ID_HANDLER_EX(IDC_CLEAR, OnBnClickedClear)
		//COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CHandwritingDlg)
		DDX_CONTROL(IDC_PAINT, m_stPaint)
		DDX_TEXT(IDC_RESULT, m_strResult)
		DDX_TEXT(IDC_OUTPUT, m_strOutput)
		DDX_CHECK(IDC_AUTO_RECONGNIZE, m_bAutoRecongnize)
	END_DDX_MAP()
public:

	//Handler prototypes (uncomment arguments if needed):
	//LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	LRESULT OnBnClickedRecognize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);

	void OnAppAbout(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnBnClickedRecongnize(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnBnClickedAutoRecongnize(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnBnClickedClear(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);
private:
	const double m_nu = 26.7133988;
	const double m_sigma = 202.4355778543416;

	CPaintPanel m_stPaint;
	CString m_strResult;
	CString m_strOutput;
	BOOL m_bAutoRecongnize;
};
