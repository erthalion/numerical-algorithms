// it_1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "it_1.h"
#include "it_1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cit_1Dlg dialog



Cit_1Dlg::Cit_1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cit_1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	n=0;l=0;eps=0;norm_diff=0;D=NULL;count=0;th_count=0;
}

void Cit_1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,n);
	DDX_Text(pDX,IDC_EDIT2,l);
	DDX_Text(pDX,IDC_EDIT3,eps);
	DDX_Text(pDX,IDC_EDIT4,norm_diff);
	DDX_Text(pDX,IDC_EDIT5,count);
	DDX_Text(pDX,IDC_EDIT6,th_count);

}

BEGIN_MESSAGE_MAP(Cit_1Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cit_1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Cit_1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Cit_1Dlg message handlers

BOOL Cit_1Dlg::OnInitDialog()
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

void Cit_1Dlg::OnPaint()
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
HCURSOR Cit_1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cit_1Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	UpdateData(TRUE);
	D=new iter(n,l,eps);
	norm_diff=D->solve();
	count=D->get_count();
//	th_count=D->solve_simple_tau();
	D->output_u();
	UpdateData(FALSE);
//	D->~iter();
}

void Cit_1Dlg::OnBnClickedButton1()
{
	wchar_t *mode=L"open";
	wchar_t *path=L"output.txt";
	ShellExecute(NULL,mode,path,NULL,NULL,SW_SHOWNORMAL) ;
	// TODO: Add your control notification handler code here
}
