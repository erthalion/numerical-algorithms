// paint_graph.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
//#include <math.h>
//#include "graph.h"
//#include "graph.cpp"


// Cpaint_graphApp:
// See paint_graph.cpp for the implementation of this class
//

class Cpaint_graphApp : public CWinApp
{
public:
	Cpaint_graphApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cpaint_graphApp theApp;