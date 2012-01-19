// polynom_gaussDlg.h : header file
//

#pragma once


// Cpolynom_gaussDlg dialog
class Cpolynom_gaussDlg : public CDialog
{
// Construction
public:
	Cpolynom_gaussDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_POLYNOM_GAUSS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:
	HICON m_hIcon;
	double m_begin,m_end,m_x,m_max;
	double m_pract_err;
	int m_mult;
	CString m_dll_name;
	Graph *my_graph;
	bool show_both;
	CPoint old;
	HACCEL haccel;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
