// ZPByPassAll.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CZPByPassAllApp:
// See ZPByPassAll.cpp for the implementation of this class
//

class CZPByPassAllApp : public CWinApp
{
public:
	CZPByPassAllApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CZPByPassAllApp theApp;