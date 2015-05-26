// DevAssistIF.h : main header file for the DevAssistIF DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "..\resource.h"		// main symbols


// CDevAssistIFApp
// See DevAssistIF.cpp for the implementation of this class
//

class CDevAssistIFApp : public CWinApp
{
public:
	CDevAssistIFApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
