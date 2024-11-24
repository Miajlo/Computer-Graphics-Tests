
// RGKolokvijum2016View.cpp : implementation of the CRGKolokvijum2016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RGKolokvijum2016.h"
#endif

#include "RGKolokvijum2016Doc.h"
#include "RGKolokvijum2016View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRGKolokvijum2016View

IMPLEMENT_DYNCREATE(CRGKolokvijum2016View, CView)

BEGIN_MESSAGE_MAP(CRGKolokvijum2016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CRGKolokvijum2016View construction/destruction

CRGKolokvijum2016View::CRGKolokvijum2016View() noexcept
{
	// TODO: add construction code here

}

CRGKolokvijum2016View::~CRGKolokvijum2016View()
{
}

BOOL CRGKolokvijum2016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGKolokvijum2016View drawing

void CRGKolokvijum2016View::DrawStick(CDC* pDC, int w) {

	std::vector<CPoint> stick_points = {
		{40,0}, { 40 + w / 100, 0}, {40 + w / 95, w}, {-w / 95 + 40, w}
	};

	pDC->Polygon(stick_points.data(), stick_points.size());
}

void CRGKolokvijum2016View::OnDraw(CDC* pDC)
{
	CRGKolokvijum2016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	DrawStick(pDC, 500);

	// TODO: add draw code for native data here
}


// CRGKolokvijum2016View printing

BOOL CRGKolokvijum2016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGKolokvijum2016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGKolokvijum2016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRGKolokvijum2016View diagnostics

#ifdef _DEBUG
void CRGKolokvijum2016View::AssertValid() const
{
	CView::AssertValid();
}

void CRGKolokvijum2016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGKolokvijum2016Doc* CRGKolokvijum2016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGKolokvijum2016Doc)));
	return (CRGKolokvijum2016Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGKolokvijum2016View message handlers
