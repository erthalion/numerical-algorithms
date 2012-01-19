// splineDlg.h : header file
//

#pragma once


// CsplineDlg dialog
class CsplineDlg : public CDialog
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CsplineDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SPLINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	double m_begin,m_end,m_nodes,m_mult;
	double m_x,m_x_pract,m_x_theor;
	double m_eps,m_delta;

	/////changed!!!
	double m_dif1_p,m_dif1_t;
	double m_dif2_p,m_dif2_t;
	//////////////

//	float m_Ox,m_Oy;
	CString m_dll_name;
	bool show_both;
	CPoint old;
	HACCEL haccel;

	Spline *my_spline;


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCancel();
};
