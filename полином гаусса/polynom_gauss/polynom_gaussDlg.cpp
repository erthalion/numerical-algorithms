// polynom_gaussDlg.cpp : implementation file
//

#include "stdafx.h"
#include "polynom_gauss.h"
#include "polynom_gaussDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cpolynom_gaussDlg dialog




Cpolynom_gaussDlg::Cpolynom_gaussDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cpolynom_gaussDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_begin=0;
	m_end=0;
	m_mult=0;
	m_x=0;
	m_max=0;
	m_dll_name=L"";
	show_both=0;
	my_graph=NULL;
	m_pract_err=0;
}

void Cpolynom_gaussDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_begin);
	DDX_Text(pDX,IDC_EDIT2,m_end);
	//DDX_Text(pDX,IDC_EDIT3,m_mult);
	DDX_Text(pDX,IDC_EDIT4,m_x);
	DDX_Text(pDX,IDC_EDIT5,m_max);
	DDX_Text(pDX,IDC_EDIT6,m_dll_name);
	DDX_Text(pDX,IDC_EDIT3,m_pract_err);

}

BEGIN_MESSAGE_MAP(Cpolynom_gaussDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cpolynom_gaussDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Cpolynom_gaussDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cpolynom_gaussDlg::OnBnClickedButton2)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// Cpolynom_gaussDlg message handlers

BOOL Cpolynom_gaussDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	haccel = LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cpolynom_gaussDlg::OnPaint()
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
HCURSOR Cpolynom_gaussDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cpolynom_gaussDlg::OnBnClickedOk()
{
	CClientDC dc(this);
	CRect rc;
	
	this->GetClientRect(&rc);

	char str[100];

	UpdateData(TRUE);	

	if(m_begin>m_end)
	{
		MessageBoxA(NULL,"invalid data(begin>end)","error",MB_OK);
		return;
	}

	if(m_x<m_begin||m_x>m_end)
	{
		MessageBoxA(NULL,"invalid data(x not in [begin,end])","error",MB_OK);
		return;
	}

	if(m_max<=0)
	{
		MessageBoxA(NULL,"invalid data(max(R)<=0)","error",MB_OK);
		return;
	}
	
//	wchar_t dll_name[MAX_PATH]=L"sin";
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


	static wchar_t buf2[MAX_PATH * 2];
	MultiByteToWideChar( CP_ACP, 0, function(), strlen(function() ) + 1, buf2, MAX_PATH );
	this->SetDlgItemTextW(IDC_EDIT7,buf2);


	my_graph=new Graph(rc.Width(),rc.Height(),1,&m_max,m_begin,m_end,m_x,dll);//width,height,mult_coeff,nodes,begin,end

	m_pract_err=my_graph->pract(m_x);
		UpdateData(FALSE);
//	MessageBoxA(NULL,itoa(my_graph->get_h()*100,str,10),"h",MB_OK);

	double delta=my_graph->get_delta();
	double x=my_graph->get_x_q();
	double y1=my_graph->func_polynom_gauss(x);
	double y2=my_graph->func_polynom_gauss(x-delta);
	double y3=my_graph->func_polynom_gauss(x+delta);
	double m1,m2,m3;
//	double x_1,y_1;

	m1=(rc.Width()-80)/(2*delta);
	m2=min((rc.Height()-160)/(2*abs(y1-y2)),(rc.Height()-160)/(2*abs(y3-y1)));
	m3=min(m1,m2);
//	MessageBoxA(NULL,itoa((rc.Width()-80)*100,str,10),"width",MB_OK);,(rc.Height()-160)/(2*(y3-y1)
//	MessageBoxA(NULL,itoa(2*delta*100,str,10),"delta",MB_OK);
//	MessageBoxA(NULL,itoa(m*100,str,10),"m",MB_OK);
	my_graph->set_mult(m3);

	my_graph->paint_graph(&dc,&rc,-x*m3,y1*m3);
	if(show_both==TRUE)
	{
	my_graph->paint_graph_orig(&dc,&rc,-x*m3,y1*m3);
	}

	CString a;
	a.Format(_T("%.4f"),my_graph->get_delta());
	this->SetDlgItemTextW(IDC_EDIT8,a.GetBuffer());

	CString b;
	b.Format(_T("%.4f"),my_graph->get_x_q()-my_graph->get_delta());
	this->SetDlgItemTextW(IDC_EDIT9,b.GetBuffer());

	CString c;
	c.Format(_T("%.4f"),my_graph->get_x_q());
	this->SetDlgItemTextW(IDC_EDIT10,c.GetBuffer());

	CString d;
	d.Format(_T("%.4f"),my_graph->get_x_q()+my_graph->get_delta());
	this->SetDlgItemTextW(IDC_EDIT11,d.GetBuffer());

	// TODO: Add your control notification handler code here
	//OnOK();
}

void Cpolynom_gaussDlg::OnBnClickedButton1()
{
		CClientDC dc(this);
		CRect rc;
	
		this->GetClientRect(&rc);

		double x=my_graph->get_x_q();
		double y=my_graph->func_polynom_gauss(x);

		my_graph->paint_graph(&dc,&rc,0,0);
		my_graph->paint_graph_orig(&dc,&rc,0,0);
		show_both=1;

	// TODO: Add your control notification handler code here
}

void Cpolynom_gaussDlg::OnBnClickedButton2()
{
		CClientDC dc(this);
		CRect rc;
	
		this->GetClientRect(&rc);

		double x=my_graph->get_x_q();
		double y=my_graph->func_polynom_gauss(x);

		my_graph->paint_graph(&dc,&rc,0,0);
//		my_graph->paint_graph_orig(&dc,&rc,0,0);
		show_both=0;

	// TODO: Add your control notification handler code here
}

void Cpolynom_gaussDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CRect rc;
	CString ox;
	CString oy;

	this->GetClientRect(&rc);

	if(nFlags==MK_LBUTTON)
	{
		my_graph->paint_graph(&dc,&rc,(point.x-old.x),(point.y-old.y));
		if(show_both==TRUE)
		{
		my_graph->paint_graph_orig(&dc,&rc,(point.x-old.x),(point.y-old.y));
		}

	}
	old.x=point.x;
	old.y=point.y;

	

	if(my_graph!=NULL)
	{
	//	m_Ox=(point.x-my_graph->get_o_x1())/my_graph->get_mult();
	//	m_Oy=-(point.y-my_graph->get_o_y1())/my_graph->get_mult();
	//UpdateData(FALSE);
	ox.Format(_T("%.1f"),(point.x-my_graph->get_o_x1())/my_graph->get_mult());
	oy.Format(_T("%.1f"),-(point.y-my_graph->get_o_y1())/my_graph->get_mult());

	this->SetDlgItemTextW(IDC_EDIT12,ox);
	this->SetDlgItemTextW(IDC_EDIT13,oy);

	}

	CDialog::OnMouseMove(nFlags, point);
}

void Cpolynom_gaussDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);


	if(point.x<my_graph->get_o_x()*2&&point.y<my_graph->get_o_y()*2)
	{
		my_graph->set_mult(my_graph->get_mult()*2);
		my_graph->paint_graph(&dc,&rc,0,0);
		if(show_both==TRUE)
		{
			my_graph->paint_graph_orig(&dc,&rc,0,0);
		}


	}
		

	CDialog::OnRButtonDown(nFlags, point);
}

BOOL Cpolynom_gaussDlg::PreTranslateMessage(MSG* pMsg) 
  {
	// TODO: Add your specialized code here and/or call the base class
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);

  if ( TranslateAccelerator( m_hWnd, haccel, pMsg ) ) 
	{
		my_graph->set_mult(my_graph->get_mult()/2);
		my_graph->paint_graph(&dc,&rc,0,0);
		if(show_both==TRUE)
		{
		my_graph->paint_graph_orig(&dc,&rc,0,0);
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
  }
