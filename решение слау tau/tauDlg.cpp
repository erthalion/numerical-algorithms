// tauDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tau.h"
#include "tauDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtauDlg dialog




CtauDlg::CtauDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtauDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	n=0;eps=0;tau=0;d1=0;d2=0;norm_w0=0;norm_wn=0;A=NULL;u=NULL;I=NULL;norm_rel=0;
}

void CtauDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,n);
	DDX_Text(pDX,IDC_EDIT2,eps);
	DDX_Text(pDX,IDC_EDIT3,norm_rel);
	DDX_Text(pDX,IDC_EDIT4,norm_wn);
	DDX_Text(pDX,IDC_EDIT5,norm_w0);
	DDX_Text(pDX,IDC_EDIT6,tau);
	DDX_Text(pDX,IDC_EDIT7,d1);
	DDX_Text(pDX,IDC_EDIT8,d2);


}

BEGIN_MESSAGE_MAP(CtauDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CtauDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CtauDlg message handlers

BOOL CtauDlg::OnInitDialog()
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

void CtauDlg::OnPaint()
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
HCURSOR CtauDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CtauDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	CWnd* pWnd = GetDlgItem(IDC_EDIT2);

	UpdateData(TRUE);

	if(n<3){MessageBoxA(NULL,"insert correct n","error",MB_ICONSTOP);return;}
	if(eps<=0){MessageBoxA(NULL,"insert correct epsilon","error",MB_ICONSTOP);return;}


	mode_solve=this->GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO3);	//1002-1004
	mode_appr=this->GetCheckedRadioButton(IDC_RADIO4,IDC_RADIO6);	//1005-1007

	u=new double[n];
	A=new double*[n];
	for(int i=0;i<n;i++)
	{
		A[i]=new double[n];
	}

	A=get_matr(A);

	switch(mode_solve)
	{
	case 1002:	u=get_rand_vect(u);
		break;
	case 1003:	u=get_null_vect(u);
		break;
	case 1004:	u=get_func_vect(u);
		break;
	default: return;
	}

	I=new iter(A,u,n,eps);

	switch(mode_appr)
	{
	case 1005:	I->set_approx_rand();
		break;
	case 1006:	I->set_approx_null();
		break;
	case 1007:	I->set_approx_near();
		break;
	default: return;
	}
	I->get_solve();
	norm_wn=I->get_wn();
	norm_w0=I->get_w0();
	norm_rel=norm_wn/norm_w0;
	tau=I->get_tau();
	d1=I->get_delta_max();
	d2=I->get_delta_min();

	UpdateData(FALSE);
	I->~iter();
}

double** CtauDlg::get_matr(double **A)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			A[i][j]=0;
		}
	}

	A[0][0]=2;A[0][1]=-1;
	A[n-1][n-2]=-1;A[n-1][n-1]=2;
	for(int i=1;i<n-1;i++)
	{
		A[i][i]=2;
		A[i][i-1]=-1;
		A[i][i+1]=-1;
	}
return A;
}

double* CtauDlg::get_rand_vect(double *u)
{
	for(int i=1;i<n-1;i++)
	{
		u[i]=(double)rand();///100.0;
		//u[i]=1;
	}
	u[0]=0;
	u[n-1]=0;

	return u;
}

double* CtauDlg::get_null_vect(double *u)
{
	for(int i=1;i<n-1;i++)
	{
		u[i]=(double)rand()*0.00000001;
		//u[i]=0.001;
	}
	u[0]=0;
	u[n-1]=0;

	return u;
}

double* CtauDlg::get_func_vect(double *u)
{
	for(int i=1;i<n-1;i++)
	{
		u[i]=sin((double)i);
	}
	u[0]=0;
	u[n-1]=0;

	return u;
}

