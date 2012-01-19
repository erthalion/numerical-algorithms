// diff_eq.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cdiff_eqApp:
// See diff_eq.cpp for the implementation of this class
//

class Cdiff_eqApp : public CWinApp
{
public:
	Cdiff_eqApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cdiff_eqApp theApp;