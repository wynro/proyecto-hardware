// DemoNoDllDlg.h : header file
//

#if !defined(AFX_DEMONODLLDLG_H__1BB086D2_6C9B_4CA8_AC60_1C02D8483070__INCLUDED_)
#define AFX_DEMONODLLDLG_H__1BB086D2_6C9B_4CA8_AC60_1C02D8483070__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDemoNoDllDlg dialog

class CDemoNoDllDlg : public CDialog
{
// Construction
public:
	CDemoNoDllDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDemoNoDllDlg)
	enum { IDD = IDD_DEMONODLL_DIALOG };
	CString	m_strOutBuf;
	CString	m_strInBuf;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoNoDllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDemoNoDllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTransferBtn();
	afx_msg void OnBtnClearout();
	afx_msg void OnBtnClearin();
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMONODLLDLG_H__1BB086D2_6C9B_4CA8_AC60_1C02D8483070__INCLUDED_)
