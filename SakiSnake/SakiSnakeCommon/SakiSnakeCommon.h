// SakiSnakeCommon.h : main header file for the SakiSnakeCommon DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSakiSnakeCommonApp
// See SakiSnakeCommon.cpp for the implementation of this class
//

class CSakiSnakeCommonApp : public CWinApp
{
public:
	CSakiSnakeCommonApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
