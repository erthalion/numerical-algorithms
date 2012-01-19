// last_it_gui.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Clast_it_guiApp:
// See last_it_gui.cpp for the implementation of this class
//

class Clast_it_guiApp : public CWinApp
{
public:
	Clast_it_guiApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Clast_it_guiApp theApp;