// paint_graphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "paint_graph.h"
#include "paint_graphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cpaint_graphDlg dialog


Cpaint_graphDlg::Cpaint_graphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cpaint_graphDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nodes=0;
	m_begin=0;
	m_end=0;
	m_mult=1;
	my_graph=NULL;
	m_x=0;
	m_x_pract=0;
	m_x_theor=0;
	old.x=0;
	old.y=0;
	m_dll_name=L"";
	haccel = LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );

//	my_graph=new Graph(0,0,0,0,0,0,0);//width,height,mult_coeff,nodes,begin,end

}

void Cpaint_graphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_nodes);
	DDX_Text(pDX,IDC_EDIT2,m_begin);
	DDX_Text(pDX,IDC_EDIT3,m_end);
	DDX_Text(pDX,IDC_EDIT4,m_mult);

	DDX_Text(pDX,IDC_EDIT5,m_x);
	DDX_Text(pDX,IDC_EDIT6,m_x_pract);
	DDX_Text(pDX,IDC_EDIT7,m_x_theor);
//	DDX_Text(pDX,IDC_EDIT9,m_Ox);
//	DDX_Text(pDX,IDC_EDIT10,m_Oy);
	DDX_Text(pDX,IDC_EDIT11,m_dll_name);

}

BEGIN_MESSAGE_MAP(Cpaint_graphDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cpaint_graphDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Cpaint_graphDlg::OnBnClickedButton1)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON2, &Cpaint_graphDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Cpaint_graphDlg::OnBnClickedButton3)
	ON_WM_RBUTTONDOWN()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// Cpaint_graphDlg message handlers

BOOL Cpaint_graphDlg::OnInitDialog()
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

void Cpaint_graphDlg::OnPaint()
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
HCURSOR Cpaint_graphDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cpaint_graphDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//double next_x,next_y,prev_x,prev_y;
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
	this->SetDlgItemTextW(IDC_EDIT8,buf2);
	
	my_graph=new Graph(rc.Width(),rc.Height(),m_mult,m_nodes,m_begin,m_end,dll);//width,height,mult_coeff,nodes,begin,end
	my_graph->paint_graph(&dc,&rc,0,0);
	if(show_both==TRUE)
	{
	my_graph->paint_graph_orig(&dc,&rc,0,0);
	}

}

void Cpaint_graphDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if(m_x<my_graph->get_begin()||m_x>my_graph->get_end())
	{
		MessageBoxA(NULL,"invalid value x","error",MB_OK);
		return;
	}
	//m_x_pract=abs(sin(m_x)-my_graph->func_polynom(m_x));
	m_x_pract=my_graph->pract(m_x);
	m_x_theor=my_graph->theor(m_x);
	UpdateData(FALSE);
	// TODO: Add your control notification handler code here
}

//CPoint old(0,0);

void Cpaint_graphDlg::OnMouseMove(UINT nFlags, CPoint point)
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

	this->SetDlgItemTextW(IDC_EDIT9,ox);
	this->SetDlgItemTextW(IDC_EDIT10,oy);

	}

	CDialog::OnMouseMove(nFlags, point);
}


void Cpaint_graphDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
		CClientDC dc(this);
		CRect rc;
	
		this->GetClientRect(&rc);

		my_graph->paint_graph(&dc,&rc,0,0);
		my_graph->paint_graph_orig(&dc,&rc,0,0);
		show_both=1;
}

void Cpaint_graphDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
		CClientDC dc(this);
		CRect rc;
	
		this->GetClientRect(&rc);

		my_graph->paint_graph(&dc,&rc,0,0);
//		my_graph->paint_graph_orig(&dc,&rc,0,0);
		show_both=0;

}

void Cpaint_graphDlg::OnRButtonDown(UINT nFlags, CPoint point)
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

BOOL Cpaint_graphDlg::PreTranslateMessage(MSG* pMsg) 
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


