// tauDlg.h : header file
//

#pragma once


// CtauDlg dialog
class CtauDlg : public CDialog
{
// Construction
public:
	CtauDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TAU_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	int n;
	double eps;
	double tau;
	int mode_solve;
	int mode_appr;
	double d1,d2;	//d1-max d2-min
	double norm_w0,norm_wn,norm_rel;
	double **A;
	double *u;
	iter *I;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	double** get_matr(double **A);
	double* get_rand_vect(double *u);
	double* get_null_vect(double *u);
	double* get_func_vect(double *u);
};
