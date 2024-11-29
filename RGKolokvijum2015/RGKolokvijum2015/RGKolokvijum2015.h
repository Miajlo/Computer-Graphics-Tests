
// RGKolokvijum2015.h : main header file for the RGKolokvijum2015 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGKolokvijum2015App:
// See RGKolokvijum2015.cpp for the implementation of this class
//

class CRGKolokvijum2015App : public CWinApp
{
public:
	CRGKolokvijum2015App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGKolokvijum2015App theApp;
