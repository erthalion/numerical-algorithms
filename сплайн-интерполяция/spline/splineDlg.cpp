// splineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "spline.h"
#include "splineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsplineDlg dialog




CsplineDlg::CsplineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsplineDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nodes=0;
	m_begin=0;
	m_end=0;
	m_mult=40;
	my_spline=NULL;
	m_eps=0.1;
	m_delta=0;
	m_x=0;
	m_x_pract=0;
	m_x_theor=0;
	old.x=0;
	old.y=0;
	m_dll_name=L"";
	m_dif1_p=0;
	m_dif1_t=0;
	m_dif2_p=0;
	m_dif2_t=0;
	haccel = LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );

}

void CsplineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_begin);
	DDX_Text(pDX,IDC_EDIT2,m_end);
	DDX_Text(pDX,IDC_EDIT3,m_x);
	DDX_Text(pDX,IDC_EDIT4,m_eps);

	DDX_Text(pDX,IDC_EDIT5,m_x_theor);
	DDX_Text(pDX,IDC_EDIT6,m_x_pract);
	DDX_Text(pDX,IDC_EDIT7,m_dll_name);
	DDX_Text(pDX,IDC_EDIT8,m_delta);

	DDX_Text(pDX,IDC_EDIT11,m_dif1_t);
	DDX_Text(pDX,IDC_EDIT12,m_dif1_p);
	DDX_Text(pDX,IDC_EDIT13,m_dif2_t);
	DDX_Text(pDX,IDC_EDIT14,m_dif2_p);

}

BEGIN_MESSAGE_MAP(CsplineDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CsplineDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON1, &CsplineDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CsplineDlg::OnBnClickedButton2)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDCANCEL, &CsplineDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CsplineDlg message handlers

BOOL CsplineDlg::OnInitDialog()
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

void CsplineDlg::OnPaint()
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
HCURSOR CsplineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CsplineDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	CClientDC dc(this);
	CRect rc;

	
	
	this->GetClientRect(&rc);

//	char str[100];

	UpdateData(TRUE);	

	if(m_begin>m_end)
	{
		MessageBoxA(NULL,"invalid data(begin>end)","error",MB_OK);
		return;
	}

	
//	wchar_t dll_name[MAX_PATH]=L"sin";
	if(m_dll_name.GetBuffer()==NULL)
	{
		MessageBoxA(NULL,"dll is not null!","error",MB_OK);
		return;
	}
	HMODULE dll=LoadLibrary(m_dll_name);

	if(dll==NULL)
	{
		MessageBoxA(NULL,"dll not found","error",MB_OK);
		return;
	}

	/*typedef char* (WINAPI *GetFormula)();

	
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
	*/

	my_spline=new Spline(rc.Width(),rc.Height(),m_mult,m_begin,m_end,dll,m_eps,m_x);//width,height,mult_coeff,begin,end,dll,epsilon,x*

	if(m_x<my_spline->get_begin()||m_x>my_spline->get_end())
	{
		MessageBoxA(NULL,"invalid value x","error",MB_OK);
		return;
	}


	my_spline->paint_graph(&dc,&rc,0,0);
	if(show_both==TRUE)
	{
	my_spline->paint_graph_orig(&dc,&rc,0,0);
	}

	m_delta=my_spline->get_delta();
	m_x_pract=my_spline->pract(m_x);
	m_x_theor=my_spline->theor(m_x);
	m_dif1_t=my_spline->theor_dif(m_x,1);
	m_dif2_t=my_spline->theor_dif(m_x,2);
	m_dif1_p=my_spline->pract_dif(m_x,1);
	m_dif2_p=my_spline->pract_dif(m_x,2);

	UpdateData(FALSE);


}

void CsplineDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CRect rc;
	CString ox;
	CString oy;

	this->GetClientRect(&rc);

	if(nFlags==MK_LBUTTON)
	{
		my_spline->paint_graph(&dc,&rc,(point.x-old.x),(point.y-old.y));
		if(show_both==TRUE)
		{
		my_spline->paint_graph_orig(&dc,&rc,(point.x-old.x),(point.y-old.y));
		}

	}
	old.x=point.x;
	old.y=point.y;

	

	if(my_spline!=NULL)
	{
	//	m_Ox=(point.x-my_graph->get_o_x1())/my_graph->get_mult();
	//	m_Oy=-(point.y-my_graph->get_o_y1())/my_graph->get_mult();
	//UpdateData(FALSE);
	ox.Format(_T("%.1f"),(point.x-my_spline->get_o_x1())/my_spline->get_mult());
	oy.Format(_T("%.1f"),-(point.y-my_spline->get_o_y1())/my_spline->get_mult());

	this->SetDlgItemTextW(IDC_EDIT9,ox);
	this->SetDlgItemTextW(IDC_EDIT10,oy);

	}

	CDialog::OnMouseMove(nFlags, point);
}

void CsplineDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
		CClientDC dc(this);
		CRect rc;
	
		this->GetClientRect(&rc);

		my_spline->paint_graph(&dc,&rc,0,0);
		my_spline->paint_graph_orig(&dc,&rc,0,0);
		show_both=1;

}

void CsplineDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
		CClientDC dc(this);
		CRect rc;
	
		this->GetClientRect(&rc);

		my_spline->paint_graph(&dc,&rc,0,0);
//		my_graph->paint_graph_orig(&dc,&rc,0,0);
		show_both=0;

}

void CsplineDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);


	if(point.x<my_spline->get_o_x()*2&&point.y<my_spline->get_o_y()*2)
	{
		my_spline->set_mult(my_spline->get_mult()*2);
		my_spline->paint_graph(&dc,&rc,0,0);
		if(show_both==TRUE)
		{
		my_spline->paint_graph_orig(&dc,&rc,0,0);
		}


	}

	CDialog::OnRButtonDown(nFlags, point);
}

void CsplineDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	delete my_spline;
	OnCancel();
}

BOOL CsplineDlg::PreTranslateMessage(MSG* pMsg) 
  {
	// TODO: Add your specialized code here and/or call the base class
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);

  if ( TranslateAccelerator( m_hWnd, haccel, pMsg ) ) 
	{
		my_spline->set_mult(my_spline->get_mult()/2);
		my_spline->paint_graph(&dc,&rc,0,0);
		if(show_both==TRUE)
		{
		my_spline->paint_graph_orig(&dc,&rc,0,0);
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
  }
