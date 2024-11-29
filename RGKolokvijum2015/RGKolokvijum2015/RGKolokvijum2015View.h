
// RGKolokvijum2015View.h : interface of the CRGKolokvijum2015View class
//

#pragma once
#include "DImage.h"


class CRGKolokvijum2015View : public CView
{
protected: // create from serialization only
	CRGKolokvijum2015View() noexcept;
	DECLARE_DYNCREATE(CRGKolokvijum2015View)

// Attributes
public:
	CRGKolokvijum2015Doc* GetDocument() const;
	bool right_mult;
	XFORM m_trans;
	DImage blue;
// Operations
public:
	void Rotate(CDC* pDC, float angle,  bool right_mult);
	void Translate(CDC* pDC, float dx, float dy, bool right_mult);
	void Scale(CDC* pDC, float sx, float xy, bool right_mult);

	void DrawBackground(CDC* pDC, CRect rect);
	void DrawGhost(CDC* pDC, CRect rect);
	void DrawPackman(CDC* pDC, CRect rect, float angle);

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
	virtual ~CRGKolokvijum2015View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RGKolokvijum2015View.cpp
inline CRGKolokvijum2015Doc* CRGKolokvijum2015View::GetDocument() const
   { return reinterpret_cast<CRGKolokvijum2015Doc*>(m_pDocument); }
#endif

