// diff_eqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "diff_eq.h"
#include "diff_eqDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cdiff_eqDlg dialog




Cdiff_eqDlg::Cdiff_eqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cdiff_eqDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	n=0;begin=0;end=0;dll_name="";x=0;D=NULL;G=NULL;
	err_euler=0;err_bound=0;y_0=0;y_1=0;fly=false;x_0=0;x_1=0;
	haccel = LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );

}

void Cdiff_eqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,n);
	DDX_Text(pDX,IDC_EDIT2,begin);
	DDX_Text(pDX,IDC_EDIT3,end);
	DDX_Text(pDX,IDC_EDIT4,x);
	DDX_Text(pDX,IDC_EDIT5,dll_name);
	DDX_Text(pDX,IDC_EDIT6,err_euler);
	DDX_Text(pDX,IDC_EDIT7,err_bound);

	DDX_Text(pDX,IDC_EDIT10,y_0);
	DDX_Text(pDX,IDC_EDIT11,y_1);

	DDX_Text(pDX,IDC_EDIT12,x_0);
	DDX_Text(pDX,IDC_EDIT13,x_1);

}

BEGIN_MESSAGE_MAP(Cdiff_eqDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cdiff_eqDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &Cdiff_eqDlg::OnBnClickedOk2)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDOK3, &Cdiff_eqDlg::OnBnClickedOk3)
END_MESSAGE_MAP()


// Cdiff_eqDlg message handlers

BOOL Cdiff_eqDlg::OnInitDialog()
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

void Cdiff_eqDlg::OnPaint()
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
HCURSOR Cdiff_eqDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cdiff_eqDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	UpdateData(TRUE);
	CClientDC dc(this);
	CRect rc;
	fly=false;
	
	if(x<begin||x>end)
	{
		MessageBoxA(NULL,"insert correct value","error",MB_ICONSTOP);
		return;
	}

	this->GetClientRect(&rc);


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

	D=new diff(begin,end,n,dll);
	G=new Graph(rc.Width(),rc.Height(),30,begin,end,dll,D->get_solve_euler(),D->get_h());
	G->rectangle(&dc,&rc,0,0);
	G->paint_graph(&dc,&rc,0,0);
	G->paint_orig(&dc,&rc,0,0);
	err_euler=G->get_err(x);
	UpdateData(FALSE);

}

void Cdiff_eqDlg::OnBnClickedOk2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CClientDC dc(this);
	CRect rc;
	fly=false;
	
	this->GetClientRect(&rc);

	if(x<begin||x>end)
	{
		MessageBoxA(NULL,"insert correct value","error",MB_ICONSTOP);
		return;
	}

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

	D=new diff(begin,end,n,dll);
	G=new Graph(rc.Width(),rc.Height(),30,begin,end,dll,D->get_solve_boundary(),D->get_h());
	G->rectangle(&dc,&rc,0,0);
	G->paint_graph(&dc,&rc,0,0);
	G->paint_orig(&dc,&rc,0,0);
	err_bound=G->get_err(x);
	UpdateData(FALSE);

}

void Cdiff_eqDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CRect rc;
	CString ox;
	CString oy;

	this->GetClientRect(&rc);

	if(nFlags==MK_LBUTTON)
	{
		G->rectangle(&dc,&rc,(point.x-old.x),(point.y-old.y));
		if(fly==true)
		{
		G->paint_graph(&dc,&rc,(point.x-old.x)/G->get_mult(),(point.y-old.y)/G->get_mult(),0);
		//G->paint_orig(&dc,&rc,(point.x-old.x)/G->get_mult(),(point.y-old.y)/G->get_mult());
		}
		else
		{
		G->paint_graph(&dc,&rc,(point.x-old.x)/G->get_mult(),(point.y-old.y)/G->get_mult());
		G->paint_orig(&dc,&rc,(point.x-old.x)/G->get_mult(),(point.y-old.y)/G->get_mult());
		}

		//G->line(&dc,&rc,(point.x-old.x),(point.y-old.y),F->get_begin1(),F->get_end1());
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
	this->SetDlgItemTextW(IDC_EDIT8,oy);

	}

	CDialog::OnMouseMove(nFlags, point);
}

void Cdiff_eqDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);


	if(point.x<G->get_o_x()*2&&point.y<G->get_o_y()*2)
	{
		G->set_mult(G->get_mult()*2);
		G->rectangle(&dc,&rc,0,0);
		if(fly=true)
		{
		G->paint_graph(&dc,&rc,0,0,0);
		//G->paint_orig(&dc,&rc,0,0);
		}
		else
		{
		G->paint_graph(&dc,&rc,0,0);
		G->paint_orig(&dc,&rc,0,0);
		}

		//G->line(&dc,&rc,0,0,F->get_begin1(),F->get_end1());
	}


	CDialog::OnRButtonDown(nFlags, point);
}

BOOL Cdiff_eqDlg::PreTranslateMessage(MSG* pMsg) 
  {
	// TODO: Add your specialized code here and/or call the base class
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);

  if ( TranslateAccelerator( m_hWnd, haccel, pMsg ) ) 
	{
		G->set_mult(G->get_mult()/2);
		G->rectangle(&dc,&rc,0,0);
		if(fly=true)
		{
		G->paint_graph(&dc,&rc,0,0,0);
		//G->paint_orig(&dc,&rc,0,0);
		}
		else
		{
		G->paint_graph(&dc,&rc,0,0);
		G->paint_orig(&dc,&rc,0,0);
		}
		//G->line(&dc,&rc,0,0,F->get_begin1(),F->get_end1());
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
  }

void Cdiff_eqDlg::OnBnClickedOk3()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CClientDC dc(this);
	CRect rc;
	fly=true;
	
	this->GetClientRect(&rc);

	if(x<begin||x>end)
	{
		MessageBoxA(NULL,"insert correct value","error",MB_ICONSTOP);
		return;
	}

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

	D=new diff(begin,end,n,dll);
	D->set_gamma(y_0,y_1);
	G=new Graph(rc.Width(),rc.Height(),5,begin,end,dll,D->get_solve_boundary(),D->get_h());
	G->rectangle(&dc,&rc,0,0);
	G->paint_graph(&dc,&rc,0,0,100);
	//G->paint_orig(&dc,&rc,0,0);
	err_bound=G->get_err(x);
	UpdateData(FALSE);


}
