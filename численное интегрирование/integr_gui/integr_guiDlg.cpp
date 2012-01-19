// integr_guiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "integr_gui.h"
#include "integr_guiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cintegr_guiDlg dialog




Cintegr_guiDlg::Cintegr_guiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cintegr_guiDlg::IDD, pParent)
{
	m_begin=0;
	m_end=0;
	m_eps=0;
	m_f_v=0;m_f_t=0;m_f_p=0;
	m_s_v=0;m_s_t=0;m_s_p=0;
	m_t_v=0;m_t_t=0;m_t_p=0;

	m_dll_name=L"";
	my_integr=NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cintegr_guiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_begin);
	DDX_Text(pDX,IDC_EDIT2,m_end);
	DDX_Text(pDX,IDC_EDIT3,m_eps);
	DDX_Text(pDX,IDC_EDIT4,m_dll_name);

	DDX_Text(pDX,IDC_EDIT5,m_f_v);
	DDX_Text(pDX,IDC_EDIT6,m_f_t);
	DDX_Text(pDX,IDC_EDIT7,m_f_p);

	DDX_Text(pDX,IDC_EDIT8,m_s_v);
	DDX_Text(pDX,IDC_EDIT9,m_s_t);
	DDX_Text(pDX,IDC_EDIT10,m_s_p);

	DDX_Text(pDX,IDC_EDIT11,m_t_v);
	DDX_Text(pDX,IDC_EDIT12,m_t_t);
	DDX_Text(pDX,IDC_EDIT13,m_t_p);

}

BEGIN_MESSAGE_MAP(Cintegr_guiDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cintegr_guiDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Cintegr_guiDlg message handlers

BOOL Cintegr_guiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cintegr_guiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cintegr_guiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cintegr_guiDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//	OnOK();
	CClientDC dc(this);
	CRect rc;

	this->GetClientRect(&rc);

	UpdateData(TRUE);	

	if(m_begin>m_end)
	{
		MessageBoxA(NULL,"invalid data(begin>end)","error",MB_OK);
		return;
	}
	
	if(m_dll_name.GetBuffer()==NULL)
	{
		MessageBoxA(NULL,"dll not found","error",MB_OK);
		return;
	}
	HMODULE dll=LoadLibrary(m_dll_name);

	typedef char* (WINAPI *GetFormula)();

	
	if(dll==NULL)
	{
		MessageBoxA(NULL,"load dll fail","error",MB_OK);
		return;
	}
	GetFormula function=(GetFormula)GetProcAddress(dll,"GetFormula");
	
	if(function==NULL)
	{
		MessageBoxA(NULL,"import function fail(GetFormula)","error",MB_OK);
		return;
	}

	double* first;
	double* second;
	double* third;

	first=new double[2];
	second=new double[2];
	third=new double[2];


	my_integr=new integr(rc.Width(),rc.Height(),20,m_begin,m_end,dll,m_eps);//width,height,mult_coeff,nodes,begin,end
	my_integr->rectangle(&dc,&rc,0,0);
	my_integr->paint_graph_orig(&dc,&rc,0,0);
	first=my_integr->first_formula();
	second=my_integr->second_formula();
	third=my_integr->third_formula();
	m_f_v=first[0];
	m_f_t=first[1];
	m_f_p=first[2];

	m_s_v=second[0];
	m_s_t=second[1];
	m_s_p=second[2];

	m_t_v=third[0];
	m_t_t=third[1];
	m_t_p=third[2];
	UpdateData(FALSE);


}
