// paint_graphDlg.h : header file
//

#pragma once


// Cpaint_graphDlg dialog
class Cpaint_graphDlg : public CDialog
{
// Construction
public:
	Cpaint_graphDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PAINT_GRAPH_DIALOG };

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	double m_begin,m_end,m_nodes,m_mult;
	double m_x,m_x_pract,m_x_theor;
//	float m_Ox,m_Oy;
	CString m_dll_name;
	bool show_both;
	CPoint old;
	HACCEL haccel;

	Graph *my_graph;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
