
// RGKolokvijum2014.h : main header file for the RGKolokvijum2014 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGKolokvijum2014App:
// See RGKolokvijum2014.cpp for the implementation of this class
//

class CRGKolokvijum2014App : public CWinApp
{
public:
	CRGKolokvijum2014App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGKolokvijum2014App theApp;
