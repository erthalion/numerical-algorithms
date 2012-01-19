// tau_minDlg.h : header file
//

#pragma once


// Ctau_minDlg dialog
class Ctau_minDlg : public CDialog
{
// Construction
public:
	Ctau_minDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TAU_MIN_DIALOG };

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
	int count_step_min,count_step_simple;
	double norm_rn,norm_r0,norm_rel_r;
	double norm_wn_r,norm_w0_r,norm_rel_w_r;
	double d1,d2;	//d1-max d2-min
	double norm_w0,norm_wn,norm_rel;
	double **A;
	double *u;
	tau_min *I;
	iter *I1;


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

	afx_msg void OnBnClickedOk2();
};
