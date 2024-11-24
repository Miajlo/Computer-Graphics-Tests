
// RGKolokvijum2016View.h : interface of the CRGKolokvijum2016View class
//

#pragma once
#include<vector>
#include"DImage.h"

class CRGKolokvijum2016View : public CView {
protected: // create from serialization only
	CRGKolokvijum2016View() noexcept;
	DECLARE_DYNCREATE(CRGKolokvijum2016View)

// Attributes
public:
	CRGKolokvijum2016Doc* GetDocument() const;
	DImage table, border;
	XFORM m_trans;
	bool right_mult = false;
// Operations
public:
	void Translate(CDC* pDC, float dx, float dy, bool right_mult);
	
	void DrawStick(CDC* pDC, int w);
	void DrawBorder(CDC* pDC, CRect rect, int w);
	void DrawTable(CDC* pDC, CRect rect);
	void DrawHoles(CDC* pDC, CRect rect, float r, int w); //w za velicinu bordera


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
	virtual ~CRGKolokvijum2016View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RGKolokvijum2016View.cpp
inline CRGKolokvijum2016Doc* CRGKolokvijum2016View::GetDocument() const
   { return reinterpret_cast<CRGKolokvijum2016Doc*>(m_pDocument); }
#endif

