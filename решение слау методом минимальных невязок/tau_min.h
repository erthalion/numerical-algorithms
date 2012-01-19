// tau_min.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ctau_minApp:
// See tau_min.cpp for the implementation of this class
//

class Ctau_minApp : public CWinApp
{
public:
	Ctau_minApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Ctau_minApp theApp;