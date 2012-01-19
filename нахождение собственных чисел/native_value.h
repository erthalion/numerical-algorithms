// native_value.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cnative_valueApp:
// See native_value.cpp for the implementation of this class
//

class Cnative_valueApp : public CWinApp
{
public:
	Cnative_valueApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cnative_valueApp theApp;