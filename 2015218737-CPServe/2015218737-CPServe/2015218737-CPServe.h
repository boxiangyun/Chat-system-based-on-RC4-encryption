
// 2015218737-CPServe.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMy2015218737CPServeApp:
// See 2015218737-CPServe.cpp for the implementation of this class
//

class CMy2015218737CPServeApp : public CWinApp
{
public:
	CMy2015218737CPServeApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMy2015218737CPServeApp theApp;