
// 2015218737TCPClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMy2015218737TCPClientApp:
// See 2015218737TCPClient.cpp for the implementation of this class
//

class CMy2015218737TCPClientApp : public CWinApp
{
public:
	CMy2015218737TCPClientApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMy2015218737TCPClientApp theApp;