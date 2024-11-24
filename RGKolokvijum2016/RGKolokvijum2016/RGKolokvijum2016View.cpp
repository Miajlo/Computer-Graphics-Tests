
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
	border.Load(_T("res\\wood.jpg"));
	table.Load(_T("res\\felt2.jpg"));

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

void CRGKolokvijum2016View::Translate(CDC* pDC, float dx, float dy, bool right_mult) {
	m_trans.eM11 = 1;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = 1;

	m_trans.eDx = dx;
	m_trans.eDy = dy;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRGKolokvijum2016View::DrawStick(CDC* pDC, int w) {

	std::vector<CPoint> stick_points = {
		{40,0}, { 40 + w / 100, 0}, {40 + w / 95, w}, {-w / 95 + 40, w}
	};

	pDC->Polygon(stick_points.data(), stick_points.size());
}

void CRGKolokvijum2016View::DrawBorder(CDC* pDC, CRect rect, int w) {

	CRect imgRect(0, 0, w, w);
	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);
		
	for (int i = 0; i <= rect.Height() / w; ++i) {
		border.Draw(pDC, imgRect, imgRect);

		Translate(pDC, rect.Width() - w, 0, right_mult);

		border.Draw(pDC, imgRect, imgRect);

		Translate(pDC, -rect.Width() + w, w, right_mult);

	}
	pDC->SetWorldTransform(&old_trans);
	for (int i = 0; i <= rect.Width() / w; ++i) {
		border.Draw(pDC, imgRect, imgRect);
		
		Translate(pDC, 0, rect.Height() - w, right_mult);

		border.Draw(pDC, imgRect, imgRect);

		Translate(pDC, w, -rect.Height() + w, right_mult);

	}

	pDC->SetWorldTransform(&old_trans);
	//Translate(pDC, w, w, right_mult);
}

void CRGKolokvijum2016View::DrawTable(CDC* pDC, CRect rect) {

	CRect imgRect(0, 0, table.Width(), table.Height());

	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);

	for (int y = 0; y <= rect.Height() / table.Height()+ 1; ++y) {
		for (int x = 0; x <= rect.Width() / table.Width() + 1; ++x) {

			table.Draw(pDC, imgRect, imgRect);
			Translate(pDC, imgRect.Width(), 0, right_mult);
		}
		pDC->SetWorldTransform(&old_trans);
		Translate(pDC, 0, imgRect.Height()*y, right_mult);
	}

	pDC->SetWorldTransform(&old_trans);
}

void CRGKolokvijum2016View::DrawHoles(CDC* pDC, CRect rect, float r, int w) {
	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);
	CRect elipse(-r/2,-r/2,r/2,r/2);
	CBrush cetka(RGB(0, 0, 0));

	auto old_brush = pDC->SelectObject(&cetka);

	Translate(pDC, w, w, right_mult);
	pDC->Ellipse(elipse);

	Translate(pDC, (rect.Width()-2*w)/2, 0, right_mult);
	pDC->Ellipse(elipse);

	Translate(pDC, (rect.Width() - 2 * w) / 2, 0, right_mult);
	pDC->Ellipse(elipse);

	Translate(pDC, 0, rect.Height()-2*w, right_mult);
	pDC->Ellipse(elipse);

	Translate(pDC, -(rect.Width() - 2 * w) / 2, 0, right_mult);
	pDC->Ellipse(elipse);

	Translate(pDC, -(rect.Width() - 2 * w) / 2, 0, right_mult);
	pDC->Ellipse(elipse);

	
	

	pDC->SelectObject(&old_brush);
	pDC->SetWorldTransform(&old_trans);
}

void CRGKolokvijum2016View::OnDraw(CDC* pDC)
{
	CRGKolokvijum2016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);
	int border_width = 40;
	float r = 40;
	

	int old_mode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);

	DrawTable(pDC, clientRect);

	DrawBorder(pDC, clientRect, border_width);

	DrawHoles(pDC, clientRect, r, border_width);

	pDC->SetWorldTransform(&old_trans);
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
