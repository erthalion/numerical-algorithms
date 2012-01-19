// polynom_gauss.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cpolynom_gaussApp:
// See polynom_gauss.cpp for the implementation of this class
//

class Cpolynom_gaussApp : public CWinApp
{
public:
	Cpolynom_gaussApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cpolynom_gaussApp theApp;