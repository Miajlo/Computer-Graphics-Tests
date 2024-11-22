
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
	arm1.Load(_T("res\\arm1.png"));
	arm2.Load(_T("res\\arm2.png"));
	leg1.Load(_T("res\\leg1.png"));
	leg2.Load(_T("res\\leg2.png"));
	body1.Load(_T("res\\body1.png"));
	background.Load(_T("res\\background.jpg"));
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

void CRGKolokvijum2019View::OnDraw(CDC* pDC)
{
	CRGKolokvijum2019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CRect bg;
	GetClientRect(&bg);

	DrawBackground(pDC, bg);


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

void CRGKolokvijum2019View::DrawBackground(CDC* pDC, CRect& rect) {

	background.Draw(pDC, rect, rect);

}

void CRGKolokvijum2019View::DrawTransparentImage(CDC* pDC, DImage* img) {
}

void CRGKolokvijum2019View::Translate(CDC* pDC, float dx, float dy, bool right_mult) {
	m_trans.eM11 = 1;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = 1;
	
	m_trans.eDx = dx;
	m_trans.eDy = dy;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRGKolokvijum2019View::Rotate(CDC* pDC, float angle, bool right_mult) {
	auto rad_angle = angle * TO_RAD;

	auto cosin = std::cos(rad_angle);
	auto sinus = std::sin(rad_angle);
	m_trans.eM11 = cosin;
	m_trans.eM12 = -sinus;
	m_trans.eM21 = cosin;
	m_trans.eM22 = sinus;

	m_trans.eDx = 0;
	m_trans.eDy = 0;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRGKolokvijum2019View::Scale(CDC* pDC, float sx, float sy, bool right_mult) {
	m_trans.eM11 = sx;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = sy;

	m_trans.eDx = 0;
	m_trans.eDy = 0;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRGKolokvijum2019View::DrawArm1(CDC* pDC) {

}

void CRGKolokvijum2019View::DrawArm2(CDC* pDC)
{
}

void CRGKolokvijum2019View::DrawBody1(CDC* pDC)
{
}

void CRGKolokvijum2019View::DrawTransformers(CDC* pDC)
{
}
