
// RGKolokvijum2019.h : main header file for the RGKolokvijum2019 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGKolokvijum2019App:
// See RGKolokvijum2019.cpp for the implementation of this class
//

class CRGKolokvijum2019App : public CWinApp
{
public:
	CRGKolokvijum2019App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGKolokvijum2019App theApp;
