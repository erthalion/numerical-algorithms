// native_valueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "native_value.h"
#include "native_valueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cnative_valueDlg dialog




Cnative_valueDlg::Cnative_valueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cnative_valueDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	n=0;lenght=0;eps=0;N=NULL;
}

void Cnative_valueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,n);
	DDX_Text(pDX,IDC_EDIT2,lenght);
	DDX_Text(pDX,IDC_EDIT3,eps);
}

BEGIN_MESSAGE_MAP(Cnative_valueDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cnative_valueDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Cnative_valueDlg message handlers

BOOL Cnative_valueDlg::OnInitDialog()
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

void Cnative_valueDlg::OnPaint()
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
HCURSOR Cnative_valueDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool check=true;

UINT thread_solve(LPVOID param)
{
	((native*)param)->find();
	check=false;
	return 0;
}


UINT thread(LPVOID param)
{

	double mod1,mod2;
	native *N;
	N=((Cnative_valueDlg*)param)->get_N();
	CString str;
	while(check)
	{
		mod1=N->get_mod1();
		mod2=N->get_mod2();
		str.Format(_T("%10.10f"),mod1);
		((Cnative_valueDlg*)param)->SetDlgItemTextW(IDC_EDIT4,str.GetBuffer());
		str.Format(_T("%10.10f"),mod2);
		((Cnative_valueDlg*)param)->SetDlgItemTextW(IDC_EDIT5,str.GetBuffer());
		
	}
	check=true;
	return 0;
}


void Cnative_valueDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	UpdateData(TRUE);
	double h=lenght/(n+1);
	double l_th=4*sin(Pi*h*(n)/2)*sin(Pi*h*(n)/2)/(h*h);
	N=new native(n,eps,lenght,l_th);

	DWORD id_thread,id_thread_solve;
	hthread_solve=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)thread_solve,N,0,&id_thread_solve);
	hthread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)thread,this,0,&id_thread);
//	CString str;
//	N->find();
//	str.Format(_T("%f"),N->get_mod1());
//	this->SetDlgItemTextW(IDC_EDIT4,str.GetBuffer());
//	str.Format(_T("%f"),N->get_mod2());
//	this->SetDlgItemTextW(IDC_EDIT5,str.GetBuffer());
//
//	UpdateData(FALSE);
}

native* Cnative_valueDlg::get_N()
{
	return N;
}