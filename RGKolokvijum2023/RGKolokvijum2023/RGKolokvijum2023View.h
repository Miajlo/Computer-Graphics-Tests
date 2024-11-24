
// RGKolokvijum2023View.h : interface of the CRGKolokvijum2023View class
//
#include"DImage.h"
#pragma once


class CRGKolokvijum2023View : public CView
{
protected: // create from serialization only
	CRGKolokvijum2023View() noexcept;
	DECLARE_DYNCREATE(CRGKolokvijum2023View)

// Attributes
public:
	CRGKolokvijum2023Doc* GetDocument() const;
	XFORM m_trans;
	bool right_mult;
	DImage glava, pozadina, nadlaktica, podlaktica; 
	DImage telo, saka, stopalo, podkolenica, nadkolenica;
	float curr_rot_angle = 0;
	const float rot_step = 5;
	
	const float hand_max_angle = 30;
	const float hand_min_angle = -10;
	const float pod_max_angle = 80;
	const float pod_min_angle = -10;
	const float nad_max_angle = 90;
	const float nad_min_angle = -10;

	float ASagnle = 0;
	float DFangle = 0;
	float GHangle = 0;
// Operations
public:
	void translate(CDC* pDC, float dx, float dy, bool rmult);
	void rotate(CDC* pDC, float angle, bool rmult);
	void scale(CDC* pDC, float sx, float sy, bool rmult);
	void draw_img_transparent(CDC* pDC, DImage* img);

	void draw_half(CDC* pDC);


// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnEraseBkgnd(CDC *pDC);
// Implementation
public:
	virtual ~CRGKolokvijum2023View();
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

#ifndef _DEBUG  // debug version in RGKolokvijum2023View.cpp
inline CRGKolokvijum2023Doc* CRGKolokvijum2023View::GetDocument() const
   { return reinterpret_cast<CRGKolokvijum2023Doc*>(m_pDocument); }
#endif

