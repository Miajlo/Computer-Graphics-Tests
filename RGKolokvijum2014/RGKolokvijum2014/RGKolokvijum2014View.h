
// RGKolokvijum2014View.h : interface of the CRGKolokvijum2014View class
//

#pragma once
#include"DImage.h"
#include<string>
#include<vector>
#include<memory>
#include <corecrt_math_defines.h>

#define MAX_SLOPE_ANGLE 80
#define MIN_SLOPE_ANGLE -10



class CRGKolokvijum2014View : public CView
{
protected: // create from serialization only
	CRGKolokvijum2014View() noexcept;
	DECLARE_DYNCREATE(CRGKolokvijum2014View)

// Attributes
public:
	CRGKolokvijum2014Doc* GetDocument() const;
	bool right_mult;
	XFORM m_trans;
	float slope_angle;
	DImage weel;
	HENHMETAFILE car;
	int car_x;
	const std::string car_path = "res\\clio.emf";
	const std::string weel_path = "res\\Wheel.png";
	const double TO_RAD = M_PI / 180.0; 
	const double TO_DEG = 180.0 / M_PI; 
	const COLORREF BG_COLOR = RGB(119, 219, 254);
	const COLORREF CLR_TRANSPARENT = RGB(255, 255, 255);
	const int mov_dst = 7;
	const int rot_angle = 5;
	// Operations
public:
	void DrawGround(CDC* pDC, float &angle);
	void DrawBackground(CDC* pDC);
	void DrawCar(CDC* pDC, int x, int y, int w, int h);
	void DrawWeel(CDC* pDC, int x, int y, int r, float angle);
	void Scale(CDC* pDC, float sx, float sy, bool right_mult);
	void Rotate(CDC* pDC, float angle, bool right_mult);
	void Translate(CDC* pDC, float dx, float dy, bool right_mult);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRGKolokvijum2014View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in RGKolokvijum2014View.cpp
inline CRGKolokvijum2014Doc* CRGKolokvijum2014View::GetDocument() const
   { return reinterpret_cast<CRGKolokvijum2014Doc*>(m_pDocument); }
#endif

