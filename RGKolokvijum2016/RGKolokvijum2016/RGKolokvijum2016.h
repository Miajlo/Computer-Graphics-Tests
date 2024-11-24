
// RGKolokvijum2016.h : main header file for the RGKolokvijum2016 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGKolokvijum2016App:
// See RGKolokvijum2016.cpp for the implementation of this class
//

class CRGKolokvijum2016App : public CWinApp
{
public:
	CRGKolokvijum2016App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGKolokvijum2016App theApp;
