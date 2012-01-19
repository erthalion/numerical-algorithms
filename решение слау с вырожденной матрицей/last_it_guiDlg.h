// last_it_guiDlg.h : header file
//

#pragma once


// Clast_it_guiDlg dialog
class Clast_it_guiDlg : public CDialog
{
// Construction
public:
	Clast_it_guiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LAST_IT_GUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	int n;
	double l;
	double eps;
	iter *I;
	HANDLE 	hthread_solve,hthread;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	iter* get_I();
};
