
// RGKolokvijum2019View.h : interface of the CRGKolokvijum2019View class
//
#include<corecrt_math_defines.h>
#include"DImage.h"
#include<cmath>
#pragma once
#define ROT_STEP 5


class CRGKolokvijum2019View : public CView
{
protected: // create from serialization only
	CRGKolokvijum2019View() noexcept;
	DECLARE_DYNCREATE(CRGKolokvijum2019View)

// Attributes
public:
	CRGKolokvijum2019Doc* GetDocument() const;
	XFORM m_trans;
	float curr_rot_angle = 0;
	DImage arm1, arm2, leg1, leg2, body1, background;
	const float TO_RAD = M_PI / 180;
	const float TO_DEG = 180 / M_PI;
	float right_mult = false;
	float body1_rot_angle = 0;
	float arm2_rot_angle = 0;
	float arms_rot_angle = 0;
// Operations
public:
	void DrawBackground(CDC* pdC, CRect &rect);
	void DrawTransparentImage(CDC* pDC, DImage* img);
	void Translate(CDC* pDC, float dx, float dy, bool right_mult);
	void Rotate(CDC* pDC, float angle, bool right_mult);
	void Scale(CDC* pDC, float sx, float sy, bool right_mult);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBody1(CDC* pDC);
	void DrawTransformers(CDC* pDC);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRGKolokvijum2019View();
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

#ifndef _DEBUG  // debug version in RGKolokvijum2019View.cpp
inline CRGKolokvijum2019Doc* CRGKolokvijum2019View::GetDocument() const
   { return reinterpret_cast<CRGKolokvijum2019Doc*>(m_pDocument); }
#endif

