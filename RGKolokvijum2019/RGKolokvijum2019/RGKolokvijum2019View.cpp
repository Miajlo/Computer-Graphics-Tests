
// RGKolokvijum2019View.cpp : implementation of the CRGKolokvijum2019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RGKolokvijum2019.h"
#endif

#include "RGKolokvijum2019Doc.h"
#include "RGKolokvijum2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRGKolokvijum2019View

IMPLEMENT_DYNCREATE(CRGKolokvijum2019View, CView)

BEGIN_MESSAGE_MAP(CRGKolokvijum2019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CRGKolokvijum2019View construction/destruction

CRGKolokvijum2019View::CRGKolokvijum2019View() noexcept
{
	// TODO: add construction code here

}

CRGKolokvijum2019View::~CRGKolokvijum2019View()
{
}

BOOL CRGKolokvijum2019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGKolokvijum2019View drawing

void CRGKolokvijum2019View::OnDraw(CDC* /*pDC*/)
{
	CRGKolokvijum2019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CRGKolokvijum2019View printing

BOOL CRGKolokvijum2019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGKolokvijum2019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGKolokvijum2019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRGKolokvijum2019View diagnostics

#ifdef _DEBUG
void CRGKolokvijum2019View::AssertValid() const
{
	CView::AssertValid();
}

void CRGKolokvijum2019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGKolokvijum2019Doc* CRGKolokvijum2019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGKolokvijum2019Doc)));
	return (CRGKolokvijum2019Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGKolokvijum2019View message handlers
