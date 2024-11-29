
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
#include<utility>
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
	m_trans.eDy = 0;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CRGKolokvijum2015View::Translate(CDC* pDC, float dx, float dy, bool right_mult) {

	m_trans.eM11 = 1;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = 1;


	m_trans.eDx = dx;
	m_trans.eDy = dy;

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

void CRGKolokvijum2015View::DrawGhost(CDC* pDC, CRect rect) {
	CPoint bot_right = rect.TopLeft();
	CPoint arc_p1 = { bot_right.x, bot_right.y + rect.Height() / 2 },
		arc_p2 = { bot_right.x + rect.Width(), bot_right.y + rect.Height() / 2 };


	
	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);
	


	//pDC->MoveTo(0, 0);


	CPen olovka(PS_SOLID, 2, RGB(0, 0, 0));
	auto old_pen = pDC->SelectObject(&olovka);

	// Create the bounding rectangle for the tent
	CRect tent(0, 0, rect.Width() / 6, -rect.Height() / 4);

	// Set the arc direction

	// Start the path
	pDC->BeginPath();

	// Draw the first arc
	pDC->MoveTo(arc_p2);
	pDC->ArcTo(rect, arc_p2, arc_p1);
	pDC->SetArcDirection(AD_CLOCKWISE);

	// Draw the first line
	pDC->LineTo({ arc_p1.x, arc_p1.y + rect.Height() });

	// Translate and loop for multiple arcs
	Translate(pDC, rect.left - 4, rect.bottom + rect.Height() / 2, right_mult);
	for (int i = 0; i < rect.Width() / tent.Width(); ++i) {
		if (i == 0) {
			// First arc
			pDC->ArcTo(tent, { tent.right, tent.bottom - tent.Height() / 2 }, { tent.left, tent.top + 10 });
		}
		else {
			// Adjust points for the next arcs
			CPoint p1 = { tent.left, tent.bottom - tent.Height() / 2 + 3 };
			CPoint p2 = { tent.right, tent.bottom - tent.Height() / 2 + 3 };
			if (i % 2 == 0) {
				std::swap(p1, p2);
			}
			// Add the arc to the path
			pDC->ArcTo(tent, p1, p2);
		}

		// Translate for the next arc
		Translate(pDC, tent.Width(), 0, right_mult);
	}

	// Add the final arc
	pDC->Arc(tent, { tent.right - tent.Width() + 5, tent.top }, { tent.left, tent.top + tent.Height() / 2 });

	pDC->SetWorldTransform(&old_trans);

	// Draw the line to close the shape
	pDC->MoveTo({ arc_p2.x, arc_p2.y + rect.Height() });
	pDC->LineTo(arc_p2);

	// End the path
	pDC->EndPath();

	// Set the fill color to red
	CBrush nova(RGB(255, 0, 0));
	auto old_bruush = pDC->SelectObject(&nova);

	// Stroke and fill the path
	pDC->StrokeAndFillPath();

	int out_r = 12, in_r = 4;
	CRect outer_circle(-out_r, -out_r, out_r, out_r);
	CRect in_circle(-in_r, -in_r, in_r, in_r);

	pDC->SetWorldTransform(&old_trans);

	Translate(pDC, rect.left + rect.Width() / 2, rect.top + rect.Height() / 2, right_mult);

	CBrush white(RGB(255, 255, 255));
	CBrush blue(RGB(0, 0, 255));

	pDC->SelectObject(&white);
	pDC->Ellipse(outer_circle);
	pDC->SelectObject(&blue);
	pDC->Ellipse(in_circle);

	Translate(pDC, 30, 0, right_mult);

	pDC->SelectObject(&white);
	pDC->Ellipse(outer_circle);
	pDC->SelectObject(&blue);
	pDC->Ellipse(in_circle);


	pDC->SelectObject(&old_pen);
	pDC->SelectObject(&old_bruush);

	pDC->SetWorldTransform(&old_trans);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
}

void CRGKolokvijum2015View::DrawPackman(CDC* pDC, CRect rect, float angle) {
	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);

	CBrush yellowBrush(RGB(255, 255, 0));
	CBrush* pOldBrush = pDC->SelectObject(&yellowBrush);

	// Calculate the center and dimensions of the bounding rectangle
	float centerX = rect.left + rect.Width() / 2.0f;
	float centerY = rect.top + rect.Height() / 2.0f;
	float w = rect.Width() / 2.0f;  // Half width (radius in x-direction)
	float h = rect.Height() / 2.0f; // Half height (radius in y-direction)

	// Convert the angle from degrees to radians
	float rad_angle = angle * M_PI / 180.0f;

	// Calculate points for the start and end of the arc
	CPoint p1(
		static_cast<int>(centerX + w * cos(rad_angle)),  // x = cx + r * cos(angle)
		static_cast<int>(centerY - h * sin(rad_angle))  // y = cy - r * sin(angle)
	);
	CPoint p2(
		static_cast<int>(centerX + w * cos(-rad_angle)), // x = cx + r * cos(-angle)
		static_cast<int>(centerY - h * sin(-rad_angle)) // y = cy - r * sin(-angle)
	);


	int r = 7;
	CRect elipse(-r, -r, r, r);

	// Draw the Pac-Man using the Pie function
	pDC->Pie(rect, p1, p2);

	pDC->SetWorldTransform(&old_trans);

	CBrush black(RGB(0, 0, 0));

	pDC->SelectObject(&black);

	rad_angle = (angle + 10) * M_PI / 180;
	

	Translate(pDC, centerX, centerY, right_mult);
	Rotate(pDC, angle + 20, right_mult);
	Translate(pDC, 2 * w / 3, 0, right_mult);

	pDC->Ellipse(elipse);

	// Restore the previous brush and transformation
	pDC->SelectObject(pOldBrush);
	pDC->SetWorldTransform(&old_trans);

}

void CRGKolokvijum2015View::OnDraw(CDC* pDC)
{
	CRGKolokvijum2015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CRect clnRect;
	CRect ghostRect(100, 120, 196, 216);
	GetClientRect(&clnRect);
	CDC* memDC = new CDC();

	if (!memDC->CreateCompatibleDC(pDC))
		return;

	CBitmap memMap;
	CRect packman(0, 0, 100, 100);
	memMap.CreateCompatibleBitmap(pDC, clnRect.Width(), clnRect.Height());

	memDC->SelectObject(&memMap);

	memDC->FillSolidRect(0, 0, clnRect.Width(), clnRect.Height(), RGB(255,255,255));

	XFORM old_trans;
	auto old_mode = memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->GetWorldTransform(&old_trans);


	DrawBackground(memDC, clnRect);

	Translate(memDC, 200, 200, right_mult);

	
	DrawGhost(memDC, ghostRect);

	DrawPackman(memDC, packman, 30);

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
