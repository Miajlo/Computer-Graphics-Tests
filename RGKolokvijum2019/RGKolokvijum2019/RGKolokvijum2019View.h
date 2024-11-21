
// RGKolokvijum2019View.h : interface of the CRGKolokvijum2019View class
//

#pragma once


class CRGKolokvijum2019View : public CView
{
protected: // create from serialization only
	CRGKolokvijum2019View() noexcept;
	DECLARE_DYNCREATE(CRGKolokvijum2019View)

// Attributes
public:
	CRGKolokvijum2019Doc* GetDocument() const;

// Operations
public:

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
};

#ifndef _DEBUG  // debug version in RGKolokvijum2019View.cpp
inline CRGKolokvijum2019Doc* CRGKolokvijum2019View::GetDocument() const
   { return reinterpret_cast<CRGKolokvijum2019Doc*>(m_pDocument); }
#endif

