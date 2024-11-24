
// RGKolokvijum2023.h : main header file for the RGKolokvijum2023 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGKolokvijum2023App:
// See RGKolokvijum2023.cpp for the implementation of this class
//

class CRGKolokvijum2023App : public CWinApp
{
public:
	CRGKolokvijum2023App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGKolokvijum2023App theApp;
