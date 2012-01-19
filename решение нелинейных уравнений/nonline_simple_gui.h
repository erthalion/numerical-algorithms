// nonline_simple_gui.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cnonline_simple_guiApp:
// See nonline_simple_gui.cpp for the implementation of this class
//

class Cnonline_simple_guiApp : public CWinApp
{
public:
	Cnonline_simple_guiApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cnonline_simple_guiApp theApp;