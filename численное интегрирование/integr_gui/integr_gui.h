// integr_gui.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cintegr_guiApp:
// See integr_gui.cpp for the implementation of this class
//

class Cintegr_guiApp : public CWinApp
{
public:
	Cintegr_guiApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cintegr_guiApp theApp;