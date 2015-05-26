// DevAssistCore.h : main header file for the DevAssistCore DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDevAssistCoreApp
// See DevAssistCore.cpp for the implementation of this class
//

class CDevAssistCoreApp : public CWinApp
{
public:
	CDevAssistCoreApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
