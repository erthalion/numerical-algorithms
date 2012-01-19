// lastDlg.cpp : implementation file
//

#include "stdafx.h"
#include "last.h"
#include "lastDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define pi 3.14159265358979323846

// ClastDlg dialog




ClastDlg::ClastDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ClastDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_begin=0;
	m_end=0;
	m_eps1=0;
	m_eps2=0;
	m_dll_name="";
	m_b="";
	m_e="";
	my_integr=NULL;
	m_mult=20;
	m_value=0;
	m_pract_err=0;
	haccel = LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );

}

void ClastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_b);
	DDX_Text(pDX,IDC_EDIT2,m_e);
	DDX_Text(pDX,IDC_EDIT3,m_eps1);
	DDX_Text(pDX,IDC_EDIT4,m_eps2);
	DDX_Text(pDX,IDC_EDIT5,m_dll_name);
	DDX_Text(pDX,IDC_EDIT6,m_value);
	DDX_Text(pDX,IDC_EDIT7,m_pract_err);
	DDX_Text(pDX,IDC_EDIT8,log);

}

BEGIN_MESSAGE_MAP(ClastDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &ClastDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// ClastDlg message handlers

BOOL ClastDlg::OnInitDialog()
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

void ClastDlg::OnPaint()
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
HCURSOR ClastDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ClastDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	CClientDC dc(this);
	CRect rc;
	
	this->GetClientRect(&rc);

	UpdateData(TRUE);	

	if(m_begin>m_end)
	{
		MessageBoxA(NULL,"invalid data(begin>end)","error",MB_OK);
		return;
	}
	
/*	if(m_eps1>m_eps2)
	{
		MessageBoxA(NULL,"invalid data(eps for one > eps for all)","error",MB_OK);
		return;
	}*/

//	wchar_t dll_name[MAX_PATH]=L"sin";
	if(m_dll_name.GetBuffer()==NULL)
	{
		MessageBoxA(NULL,"dll not found","error",MB_OK);
		return;
	}

	HMODULE dll=LoadLibrary(m_dll_name);


	if(dll==NULL)
	{
		MessageBoxA(NULL,"load dll fail","error",MB_OK);
		return;
	}

	wchar_t* err;

	if(m_b.Compare(_T("i"))==0&&m_e.Compare(_T("i"))!=0)
	{
	m_begin=-100;
		m_end=wcstod(m_e,&err);
		if(*err)
		{
			MessageBoxA(NULL,"insert correct value(end)","error",MB_OK);
			return;
		}

	my_integr=new integr(rc.Width(),rc.Height(),m_mult,m_begin,m_end,dll,m_eps1,m_eps2);//width,height,mult_coeff,nodes,begin,end
	my_integr->rectangle(&dc,&rc,0,0);
	my_integr->paint_graph_orig(&dc,&rc,0,0);
	my_integr->flood_fill(&dc,&rc,0,0);
	m_value=my_integr->find_all_int("i",m_end,m_eps2);
//	m_pract_err=0;
	m_pract_err=my_integr->pract_error();
	UpdateData(FALSE);
	return;

	}

	if(m_e.Compare(_T("i"))==0&&m_b.Compare(_T("i"))!=0)
	{
	m_end=100;
		m_begin=wcstod(m_b,&err);
		if(*err)
		{
			MessageBoxA(NULL,"insert correct value(begin)","error",MB_OK);
			return;
		}

	my_integr=new integr(rc.Width(),rc.Height(),m_mult,m_begin,m_end,dll,m_eps1,m_eps2);//width,height,mult_coeff,nodes,begin,end
	my_integr->rectangle(&dc,&rc,0,0);
	my_integr->paint_graph_orig(&dc,&rc,0,0);
	my_integr->flood_fill(&dc,&rc,0,0);
	m_value=my_integr->find_all_int(m_begin,"i",m_eps2);
// 	m_value=m_value+my_integr->find_all_int("i",m_begin,m_eps2);
	m_pract_err=my_integr->pract_error();
	
	UpdateData(FALSE);
	return;
	}
	
	if(m_e.Compare(_T("i"))==0&&m_b.Compare(_T("i"))==0)
	{
	m_begin=-100;
	m_end=100;
	my_integr=new integr(rc.Width(),rc.Height(),m_mult,m_begin,m_end,dll,m_eps1,m_eps2);//width,height,mult_coeff,nodes,begin,end
	my_integr->rectangle(&dc,&rc,0,0);
	my_integr->paint_graph_orig(&dc,&rc,0,0);
	my_integr->flood_fill(&dc,&rc,0,0);
	m_value=my_integr->find_all_int(0,"i",m_eps2);
 	m_value=m_value+my_integr->find_all_int("i",0,m_eps2);
	m_pract_err=abs(m_value-my_integr->pract_error_inf());
	UpdateData(FALSE);
	return;
	}
	
	if(m_e.Compare(_T("i"))!=0&&m_b.Compare(_T("i"))!=0)
	{

		m_begin=wcstod(m_b,&err);
		if(*err)
		{
			MessageBoxA(NULL,"insert correct value(begin)","error",MB_OK);
			return;
		}
		m_end=wcstod(m_e,&err);
		if(*err)
		{
			MessageBoxA(NULL,"insert correct value(end)","error",MB_OK);
			return;
		}
	my_integr=new integr(rc.Width(),rc.Height(),m_mult,m_begin,m_end,dll,m_eps1,m_eps2);//width,height,mult_coeff,nodes,begin,end
	my_integr->rectangle(&dc,&rc,0,0);
	my_integr->paint_graph_orig(&dc,&rc,0,0);
	my_integr->flood_fill(&dc,&rc,0,0);

	m_value=my_integr->find_all_int(m_begin,m_end,m_eps2);
	m_pract_err=my_integr->pract_error();
	UpdateData(FALSE);
	return;
	}



}

void ClastDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	CRect rc;
	CString ox;
	CString oy;

	this->GetClientRect(&rc);

	if(nFlags==MK_LBUTTON)
	{
		my_integr->rectangle(&dc,&rc,-(point.x-old.x),-(point.y-old.y));
		my_integr->paint_graph_orig(&dc,&rc,(point.x-old.x),(point.y-old.y));
		my_integr->flood_fill(&dc,&rc,(point.x-old.x),(point.y-old.y));
/*		if(show_both==TRUE)
		{
		my_graph->paint_graph_orig(&dc,&rc,(point.x-old.x),(point.y-old.y));
		}*/

	}
	old.x=point.x;
	old.y=point.y;

	

	if(my_integr!=NULL)
	{
	//	m_Ox=(point.x-my_graph->get_o_x1())/my_graph->get_mult();
	//	m_Oy=-(point.y-my_graph->get_o_y1())/my_graph->get_mult();
	//UpdateData(FALSE);
//	ox.Format(_T("%.1f"),(point.x-my_integr->get_o_x1())/my_integr->get_mult());
//	oy.Format(_T("%.1f"),-(point.y-my_integr->get_o_y1())/my_integr->get_mult());

//	this->SetDlgItemTextW(IDC_EDIT14,ox);
//	this->SetDlgItemTextW(IDC_EDIT15,oy);

	}

	CDialog::OnMouseMove(nFlags, point);
}

void ClastDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);


	if(point.x<my_integr->get_o_x()*2&&point.y<my_integr->get_o_y()*2)
	{
		my_integr->set_mult(my_integr->get_mult()*2);
		my_integr->rectangle(&dc,&rc,0,0);
		my_integr->paint_graph_orig(&dc,&rc,0,0);
		my_integr->flood_fill(&dc,&rc,0,0);

	}


	CDialog::OnRButtonDown(nFlags, point);
}

BOOL ClastDlg::PreTranslateMessage(MSG* pMsg) 
  {
	// TODO: Add your specialized code here and/or call the base class
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);

  if ( TranslateAccelerator( m_hWnd, haccel, pMsg ) ) 
	{
		my_integr->set_mult(my_integr->get_mult()/2);
		my_integr->rectangle(&dc,&rc,0,0);
		my_integr->paint_graph_orig(&dc,&rc,0,0);
		my_integr->flood_fill(&dc,&rc,0,0);

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
  }
