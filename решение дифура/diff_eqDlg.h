// diff_eqDlg.h : header file
//

#pragma once


// Cdiff_eqDlg dialog
class Cdiff_eqDlg : public CDialog
{
// Construction
public:
	Cdiff_eqDlg(CWnd* pParent = NULL);	// standard constructor
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Dialog Data
	enum { IDD = IDD_DIFF_EQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CString dll_name;
	int n;
	double begin,end;
	double x;
	double err_euler,err_bound;
	diff *D;
	Graph *G;
	double y_0,y_1;
	double x_0,x_1;
	bool fly;

	CPoint old;
	HACCEL haccel;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk3();
};
