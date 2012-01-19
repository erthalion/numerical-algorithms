// nonline_simple_guiDlg.h : header file
//

#pragma once


// Cnonline_simple_guiDlg dialog
class Cnonline_simple_guiDlg : public CDialog
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	Cnonline_simple_guiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NONLINE_SIMPLE_GUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	double begin,end,eps;
	int c_s,c_n;

	nonline *F;
	Graph *G;
	CString dll_name;
	double s_s;
	double s_n;

	CPoint old;
	HACCEL haccel;


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
