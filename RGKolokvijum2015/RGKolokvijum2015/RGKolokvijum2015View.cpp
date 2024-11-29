
// RGKolokvijum2015View.cpp : implementation of the CRGKolokvijum2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RGKolokvijum2015.h"
#endif

#include "RGKolokvijum2015Doc.h"
#include "RGKolokvijum2015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include<cmath>
#include<corecrt_math_defines.h>

// CRGKolokvijum2015View

IMPLEMENT_DYNCREATE(CRGKolokvijum2015View, CView)

BEGIN_MESSAGE_MAP(CRGKolokvijum2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CRGKolokvijum2015View construction/destruction

CRGKolokvijum2015View::CRGKolokvijum2015View() noexcept
{
	// TODO: add construction code here
	right_mult = false;
	blue.Load(_T("res\\blue.png"));
}

CRGKolokvijum2015View::~CRGKolokvijum2015View()
{
}

BOOL CRGKolokvijum2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGKolokvijum2015View drawing

void CRGKolokvijum2015View::Rotate(CDC* pDC, float angle,  bool right_mult) {
	auto rad_angle = angle * M_PI / 180;

	auto cosin = std::cos(rad_angle);
	auto sinus = std::sin(rad_angle);

	m_trans.eM11 = cosin;
	m_trans.eM12 = -sinus;
	m_trans.eM21 = sinus;
	m_trans.eM22 = cosin;


	m_trans.eDx = 0;
	m_trans.eM11 = 0;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CRGKolokvijum2015View::Translate(CDC* pDC, float dx, float dy, bool right_mult) {

	m_trans.eM11 = 1;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = 1;


	m_trans.eDx = dx;
	m_trans.eM11 = dy;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRGKolokvijum2015View::Scale(CDC* pDC, float sx, float sy, bool right_mult) {

	m_trans.eM11 = sx;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = sy;


	m_trans.eDx = 0;
	m_trans.eM11 = 0;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRGKolokvijum2015View::DrawBackground(CDC* pDC, CRect rect) {
	blue.Draw(pDC, rect, rect);
}

void CRGKolokvijum2015View::OnDraw(CDC* pDC)
{
	CRGKolokvijum2015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	/*CRect rect(200, 200, 250, 300);
	auto dir = pDC->GetArcDirection();
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Arc(rect, { 200, 250 }, { 300, 250 });
	pDC->SetArcDirection(dir);*/

	CRect clnRect;
	GetClientRect(&clnRect);
	CDC* memDC = new CDC();

	if (!memDC->CreateCompatibleDC(pDC))
		return;

	CBitmap memMap;
	memMap.CreateCompatibleBitmap(pDC, clnRect.Width(), clnRect.Height());

	memDC->SelectObject(&memMap);

	memDC->FillSolidRect(0, 0, clnRect.Width(), clnRect.Height(), RGB(255,255,255));

	XFORM old_trans;
	auto old_mode = memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->GetWorldTransform(&old_trans);

	DrawBackground(memDC, clnRect);




	memDC->SetWorldTransform(&old_trans);
	memDC->SetGraphicsMode(old_mode);

	pDC->BitBlt(0, 0, clnRect.Width(), clnRect.Height(), memDC, 0, 0, SRCCOPY);



	memDC->DeleteDC();
	delete memDC;
	memDC = nullptr;
	

	// TODO: add draw code for native data here
}


// CRGKolokvijum2015View printing

BOOL CRGKolokvijum2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGKolokvijum2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGKolokvijum2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRGKolokvijum2015View diagnostics

#ifdef _DEBUG
void CRGKolokvijum2015View::AssertValid() const
{
	CView::AssertValid();
}

void CRGKolokvijum2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGKolokvijum2015Doc* CRGKolokvijum2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGKolokvijum2015Doc)));
	return (CRGKolokvijum2015Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGKolokvijum2015View message handlers
