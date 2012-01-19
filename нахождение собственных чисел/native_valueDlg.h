// native_valueDlg.h : header file
//

#pragma once


// Cnative_valueDlg dialog
class Cnative_valueDlg : public CDialog
{
// Construction
public:
	Cnative_valueDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NATIVE_VALUE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	int n;
	double eps;
	double lenght;
	native *N;
	HANDLE hthread,hthread_solve;
	//bool check;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	native* get_N();
};
