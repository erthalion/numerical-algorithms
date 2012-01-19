// last_it_guiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "last_it_gui.h"
#include "last_it_guiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clast_it_guiDlg dialog




Clast_it_guiDlg::Clast_it_guiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Clast_it_guiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	n=0;l=0;eps=0;I=NULL;
}

void Clast_it_guiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,n);
	DDX_Text(pDX,IDC_EDIT2,l);
	DDX_Text(pDX,IDC_EDIT3,eps);
}

BEGIN_MESSAGE_MAP(Clast_it_guiDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Clast_it_guiDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Clast_it_guiDlg message handlers

BOOL Clast_it_guiDlg::OnInitDialog()
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

void Clast_it_guiDlg::OnPaint()
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
HCURSOR Clast_it_guiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool check=true;

UINT thread_solve(LPVOID param)
{
	((iter*)param)->solve();
	check=false;
	return 0;
}


UINT thread(LPVOID param)
{

	double mod;
	iter *I;
	I=((Clast_it_guiDlg*)param)->get_I();
	CString str;
	while(check)
	{
		mod=I->get_mod();
		str.Format(_T("%10.10f"),mod);
		((Clast_it_guiDlg*)param)->SetDlgItemTextW(IDC_EDIT4,str.GetBuffer());
		mod=I->get_n();
		str.Format(_T("%10.10f"),mod);
		((Clast_it_guiDlg*)param)->SetDlgItemTextW(IDC_EDIT5,str.GetBuffer());
		mod=I->get_n0();
		str.Format(_T("%10.10f"),mod);
		((Clast_it_guiDlg*)param)->SetDlgItemTextW(IDC_EDIT6,str.GetBuffer());

	}
	check=true;
	return 0;
}


void Clast_it_guiDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	UpdateData(TRUE);
	I=new iter(n,l,eps);
	DWORD id_thread,id_thread_solve;
	hthread_solve=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)thread_solve,I,0,&id_thread_solve);
	hthread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)thread,this,0,&id_thread);

}

iter* Clast_it_guiDlg::get_I()
{
	return I;
}