// nonline_simple_guiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nonline_simple_gui.h"
#include "nonline_simple_guiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cnonline_simple_guiDlg dialog




Cnonline_simple_guiDlg::Cnonline_simple_guiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cnonline_simple_guiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	begin=0;end=0;eps=0;c_s=0;c_n=0;F=NULL;G=NULL;s_s=0;s_n=0;
	dll_name="";
	haccel = LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );

}

void Cnonline_simple_guiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,begin);
	DDX_Text(pDX,IDC_EDIT2,end);
	DDX_Text(pDX,IDC_EDIT3,eps);
	DDX_Text(pDX,IDC_EDIT4,c_s);
	DDX_Text(pDX,IDC_EDIT5,c_n);
	DDX_Text(pDX,IDC_EDIT6,dll_name);
	DDX_Text(pDX,IDC_EDIT7,s_s);
	DDX_Text(pDX,IDC_EDIT8,s_n);

}

BEGIN_MESSAGE_MAP(Cnonline_simple_guiDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cnonline_simple_guiDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// Cnonline_simple_guiDlg message handlers

BOOL Cnonline_simple_guiDlg::OnInitDialog()
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

void Cnonline_simple_guiDlg::OnPaint()
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
HCURSOR Cnonline_simple_guiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cnonline_simple_guiDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	CClientDC dc(this);
	CRect rc;
	
	this->GetClientRect(&rc);

	UpdateData(TRUE);

	if(dll_name.GetBuffer()==NULL)
	{
		MessageBoxA(NULL,"dll not found","error",MB_OK);
		return;
	}
	HMODULE dll=LoadLibrary(dll_name.GetBuffer());
	if(dll==NULL)
	{
		MessageBoxA(NULL,"load dll fail","error",MB_OK);
		return;
	}

	G=new Graph(rc.Width(),rc.Height(),30,begin,end,dll);
	G->rectangle(&dc,&rc,0,0);
	G->paint_graph(&dc,&rc,0,0);
	
	F=new nonline(begin,end,eps,dll);
	F->simple();
	s_s=F->get_err();
	F->newton();
	s_n=F->get_err();
	c_s=F->get_simple_n();
	c_n=F->get_newton_n();

	G->line(&dc,&rc,0,0,F->get_begin1(),F->get_end1());

	UpdateData(FALSE);
}

void Cnonline_simple_guiDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CRect rc;
	CString ox;
	CString oy;

	this->GetClientRect(&rc);

	if(nFlags==MK_LBUTTON)
	{
		G->paint_graph(&dc,&rc,(point.x-old.x),(point.y-old.y));
		G->line(&dc,&rc,(point.x-old.x),(point.y-old.y),F->get_begin1(),F->get_end1());
	}
	old.x=point.x;
	old.y=point.y;

	

	if(G!=NULL)
	{
	//	m_Ox=(point.x-my_graph->get_o_x1())/my_graph->get_mult();
	//	m_Oy=-(point.y-my_graph->get_o_y1())/my_graph->get_mult();
	//UpdateData(FALSE);
	ox.Format(_T("%.1f"),(point.x-G->get_o_x1())/G->get_mult());
	oy.Format(_T("%.1f"),-(point.y-G->get_o_y1())/G->get_mult());

	this->SetDlgItemTextW(IDC_EDIT9,ox);
	this->SetDlgItemTextW(IDC_EDIT10,oy);

	}

	CDialog::OnMouseMove(nFlags, point);
}

void Cnonline_simple_guiDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);


	if(point.x<G->get_o_x()*2&&point.y<G->get_o_y()*2)
	{
		G->set_mult(G->get_mult()*2);
		G->paint_graph(&dc,&rc,0,0);
		G->line(&dc,&rc,0,0,F->get_begin1(),F->get_end1());
	}


	CDialog::OnRButtonDown(nFlags, point);
}

BOOL Cnonline_simple_guiDlg::PreTranslateMessage(MSG* pMsg) 
  {
	// TODO: Add your specialized code here and/or call the base class
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);

  if ( TranslateAccelerator( m_hWnd, haccel, pMsg ) ) 
	{
		G->set_mult(G->get_mult()/2);
		G->paint_graph(&dc,&rc,0,0);
		G->line(&dc,&rc,0,0,F->get_begin1(),F->get_end1());
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
  }
