// gaussDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gauss.h"
#include "gaussDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CgaussDlg dialog




CgaussDlg::CgaussDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CgaussDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	dimension=0;
	old_dimension=0;
	orig_vector="";
	err_rev=0;
	err_vect=0;
	u_solve=NULL;
	A=NULL;
	u=NULL;
//	A=new double*[dimension];
//	for(int i=0;i<dimension;i++)
//	{
//		A[i]=new double[dimension];
//	}

//	u=new double[dimension];

}

void CgaussDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,dimension);
	DDX_Text(pDX,IDC_EDIT2,orig_vector);
	DDX_Text(pDX,IDC_EDIT3,err_rev);
	DDX_Text(pDX,IDC_EDIT4,err_vect);
}

BEGIN_MESSAGE_MAP(CgaussDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CgaussDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CgaussDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CgaussDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CgaussDlg message handlers

BOOL CgaussDlg::OnInitDialog()
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

void CgaussDlg::OnPaint()
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
HCURSOR CgaussDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CgaussDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	if(u==NULL) return;
	CWnd* pWnd = GetDlgItem(IDC_EDIT2);
//	pWnd->SetWindowText(_T(""));

	UpdateData(TRUE);

	matr=this->GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2);
	vect=this->GetCheckedRadioButton(IDC_RADIO3,IDC_RADIO5);
	mode=this->GetCheckedRadioButton(IDC_RADIO6,IDC_RADIO9);

	A=new double*[dimension];
	for(int i=0;i<dimension;i++)
	{
		A[i]=new double[dimension];
	}

//	char str[50];
//	MessageBoxA(NULL,itoa(matr,str,10),"",MB_OK);
//	MessageBoxA(NULL,itoa(vect,str,10),"",MB_OK);
//	MessageBoxA(NULL,itoa(mode,str,10),"",MB_OK);
	switch(matr)
	{
	case 1000:	A=get_diag_matr(A);
		break;
	case 1001:	A=get_gilbert_matr(A);
		break;
	default: return;
	}

	if(old_dimension!=dimension)
	{
	switch(vect)
	{
	case 1002:	u=get_rand_vect(u);
		break;
	case 1003:	u=get_null_vect(u);
		break;
	case 1004:	u=get_func_vect(u);
		break;
	default: return;
	}

	}
/*	switch(vect)
	{
	case 1002:	u=get_rand_vect(u);
		break;
	case 1003:	u=get_null_vect(u);
		break;
	case 1004:	u=get_func_vect(u);
		break;
	default: return;
	}*/

	switch(mode)
	{
	case 1005:	mode=1;
		break;
	case 1006:	mode=2;
		break;
	case 1007:	mode=3;
		break;
	case 1008:  mode=4;
		break;
	default: return;
	}

	char str[10];
/*	CString buff;
	for(int i=0;i<dimension;i++)
	{
	buff.Empty();
	buff.Format(_T("%f"),u[i]);
	orig_vector+=buff;	//ltoa(u[i],str,10);
	orig_vector+=" ";
//		orig_vector.Format(_T("%f"),u[i]);
	}*/

//for(int i=0;i<dimension;i++)
//{
//	MessageBoxA(NULL,itoa(u[i],str,10),"",MB_OK);
//}
	m_g=new method_gauss(A,u,dimension);
	u_solve=m_g->get_solve(mode);
	err_vect=m_g->get_err();
	err_rev=m_g->get_err_revmatr(m_g->get_mult(m_g->get_A(),m_g->get_reverse_matr()));
	UpdateData(FALSE);

	m_g->~method_gauss();
	old_dimension=dimension;
//	delete(u);

//	for(int i=0;i<dimension;i++)
//	{
//		delete(A[i]);
//	}
//	orig_vector.Empty();	
//	err_vect=0;
//	err_rev=0;

	//this->SetWindowTextW
	//OnOK();
}

double** CgaussDlg::get_diag_matr(double **A)
{
//	double **A=new double*[dimension];
//	for(int i=0;i<dimension;i++)
//	{
//		A[i]=new double[dimension];
//	}

	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<dimension;j++)
		{
		//	A[i][j]=1.0/(i+j+1);
			A[i][j]=0;
		}
	}
	A[0][0]=2.0;
	A[0][1]=1.0;
	for(int i=1;i<dimension-1;i++)
	{
		A[i][i]=2.0;
		A[i][i+1]=1.0;
		A[i][i-1]=1.0;
	}
	A[dimension-1][dimension-1]=2.0;
	A[dimension-1][dimension-2]=1.0;


	return A;
}

double** CgaussDlg::get_gilbert_matr(double **A)
{
//double **A=new double*[dimension];
//	for(int i=0;i<dimension;i++)
//	{
//		A[i]=new double[dimension];
//	}

	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<dimension;j++)
		{
			A[i][j]=1.0/(i+j+1);
		//	A[i][j]=0;
		}
	}

	return A;
}

double* CgaussDlg::get_rand_vect(double *u)
{
//double *u=new double[dimension];
	for(int i=0;i<dimension;i++)
	{
		u[i]=(double)rand();///100.0;
		//u[i]=1;
	}

	return u;
}

double* CgaussDlg::get_null_vect(double *u)
{
//double *u=new double[dimension];
	for(int i=0;i<dimension;i++)
	{
		u[i]=(double)rand()*0.00000001;
		//u[i]=0.001;
	}

	return u;
}

double* CgaussDlg::get_func_vect(double *u)
{
//double *u=new double[dimension];
	for(int i=0;i<dimension;i++)
	{
		u[i]=sin((double)i);
	}

	return u;
}

void CgaussDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_EDIT2);
	pWnd->SetWindowText(_T(""));

	UpdateData(TRUE);
	matr=this->GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2);
	vect=this->GetCheckedRadioButton(IDC_RADIO3,IDC_RADIO5);
	mode=this->GetCheckedRadioButton(IDC_RADIO6,IDC_RADIO9);
	u=new double[dimension];
	switch(vect)
	{
	case 1002:	u=get_rand_vect(u);
		break;
	case 1003:	u=get_null_vect(u);
		break;
	case 1004:	u=get_func_vect(u);
		break;
	default: return;
	}

	CString buff;
	for(int i=0;i<dimension;i++)
	{
	buff.Empty();
	buff.Format(_T("%f"),u[i]);
	orig_vector+=buff;	//ltoa(u[i],str,10);
	orig_vector+=" ";
//		orig_vector.Format(_T("%f"),u[i]);
	}

	UpdateData(FALSE);
}

void CgaussDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	delete(u);
	for(int i=0;i<dimension;i++)
	{
		delete(A[i]);
	}

	OnCancel();
}
