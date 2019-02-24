// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "NeuralNetwork.h"
#include "HandwritingDlg.h"

CHandwritingDlg::CHandwritingDlg()
{
	m_bAutoRecongnize = FALSE;
}

CHandwritingDlg::~CHandwritingDlg()
{

}

BOOL CHandwritingDlg::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	DoDataExchange(FALSE);
	return TRUE;
}

LRESULT CHandwritingDlg::OnBnClickedRecognize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	double *pParam = (double *)lParam;

	cv::Mat x = cv::Mat::zeros(1, 400, CV_64FC1);

	for (int i = 0; i < 400; i++)
	{
		x.at<double>(0, i) = pParam[i];
	}

	x = (x - m_nu) / m_sigma;

	cv::Mat a3;
	Predict(x, a3);

	int predict[2] = { 0, 0 };
	cv::minMaxIdx(a3, NULL, NULL, NULL, predict);

	//输出结果信息
	if (m_strResult.GetLength() >= 20)
	{
		m_strResult = _T("");
		m_strOutput = _T("");
	}

	////预测结果
	int nPredict = -1;

	int nResult = predict[1];
	if (nResult == 9)
	{
		nPredict = 0;
	}
	else
	{
		nPredict = nResult + 1;
	}

	////预测信心大于 40%才予以显示，否则显示为?
	if (a3.at<double>(0, nResult) > 0.4)
	{
		CString strTemp;

		strTemp.Format(_T("%d "), nPredict);
		m_strResult += strTemp;
	}
	else
	{
		CString strTemp;

		strTemp.Format(_T("? "));
		m_strResult += strTemp;
	}

	//预测结果概率
	CString strConfidence;

	for (int i = 0; i < 10; i++)
	{
		double dProbability = a3.at<double>(0, i);

		//0
		if (9 == i)
		{
			CString strTemp;
			strTemp.Format(_T("0=%2.0f%%  "), dProbability * 100);
			strConfidence += strTemp;
		}
		else
		{
			CString strTemp;
			strTemp.Format(_T("%d=%2.0f%%  "), i + 1, dProbability * 100);
			strConfidence += strTemp;
		}
	}

	CString strTemp2;
	strTemp2.Format(_T("[%d] :p{"), nPredict);
	m_strOutput += strTemp2;

	m_strOutput += strConfidence;
	m_strOutput += _T("}\r\n");

	double fPredictConfidence = a3.at<double>(0, nResult);
	if (fPredictConfidence < 0.4)
	{
		CString strTemp3;
		strTemp3.Format(_T("- - - notify: predict result (%d=%2.0f%%). confidence lower by 40%%\r\n"), nPredict, fPredictConfidence * 100);
		m_strOutput += strTemp3;

	}

	DoDataExchange(FALSE);
	CEdit edit = GetDlgItem(IDC_OUTPUT);

	edit.SetFocus();
	edit.SetSel(-1);

	return 0;
}

void CHandwritingDlg::OnAppAbout(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
}

void CHandwritingDlg::OnBnClickedRecongnize(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CWindow wnd = GetDlgItem(IDC_PAINT);
	::SendMessage(wnd, WM_GET_WRITE, (WPARAM)0, (LPARAM)0);
}

void CHandwritingDlg::OnBnClickedAutoRecongnize(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	DoDataExchange(TRUE);
	m_stPaint.SetAutoMode(m_bAutoRecongnize);
}

void CHandwritingDlg::OnBnClickedClear(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CWindow wnd = GetDlgItem(IDC_PAINT);
	::SendMessage(wnd, WM_CLEANUP, (WPARAM)0, (LPARAM)0);
}

void CHandwritingDlg::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	EndDialog(nID);
}
