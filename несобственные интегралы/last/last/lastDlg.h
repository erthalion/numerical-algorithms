// lastDlg.h : header file
//

#pragma once


// ClastDlg dialog
class ClastDlg : public CDialog
{
// Construction
public:
	ClastDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LAST_DIALOG };
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	double m_begin,m_end,m_eps1,m_eps2,m_mult;
	double m_value,m_pract_err;
	CString m_dll_name;
	CString m_b,m_e;
	integr* my_integr;
	CPoint old;
	HACCEL haccel;
	double log;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
