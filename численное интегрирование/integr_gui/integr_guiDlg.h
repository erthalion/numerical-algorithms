// integr_guiDlg.h : header file
//

#pragma once


// Cintegr_guiDlg dialog
class Cintegr_guiDlg : public CDialog
{
// Construction
public:
	Cintegr_guiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_INTEGR_GUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	double m_begin,m_end,m_eps;
	CString m_dll_name;
	bool show_both;
	CPoint old;
	HACCEL haccel;

	double m_f_v,m_f_t,m_f_p;
	double m_s_v,m_s_t,m_s_p;
	double m_t_v,m_t_t,m_t_p;


	integr *my_integr;


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
