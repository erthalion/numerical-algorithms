// gaussDlg.h : header file
//

#pragma once


// CgaussDlg dialog
class CgaussDlg : public CDialog
{
// Construction
public:
	CgaussDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GAUSS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	int dimension;
	int old_dimension;
	int matr,vect,mode;
	CString orig_vector;
	double err_rev;
	double err_vect;
	double **A;
	double *u;
	double *u_solve;
	method_gauss *m_g;

	// Generated message map functions
	double **get_diag_matr(double **A);
	double **get_gilbert_matr(double **A);
	double *get_rand_vect(double *u);
	double *get_null_vect(double *u);
	double *get_func_vect(double *u);

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
};
