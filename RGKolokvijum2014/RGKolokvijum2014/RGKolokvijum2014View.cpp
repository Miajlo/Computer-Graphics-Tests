
// RGKolokvijum2014View.cpp : implementation of the CRGKolokvijum2014View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RGKolokvijum2014.h"
#endif

#include "RGKolokvijum2014Doc.h"
#include "RGKolokvijum2014View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include<cmath>

// CRGKolokvijum2014View

IMPLEMENT_DYNCREATE(CRGKolokvijum2014View, CView)

BEGIN_MESSAGE_MAP(CRGKolokvijum2014View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CRGKolokvijum2014View construction/destruction

CRGKolokvijum2014View::CRGKolokvijum2014View() noexcept
{
	// TODO: add construction code here
	weel.Load(CString(weel_path.c_str()));
	car = GetEnhMetaFile(CString(car_path.c_str()));
	right_mult = false;
	slope_angle = 0;
	car_x = 0;
}

CRGKolokvijum2014View::~CRGKolokvijum2014View()
{
}

BOOL CRGKolokvijum2014View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGKolokvijum2014View drawing

void CRGKolokvijum2014View::DrawGround(CDC* pDC, float &angle) {
	CRect cln_rect;
	GetClientRect(&cln_rect);

	auto w = cln_rect.Width();
	auto h = cln_rect.Height();

	auto top_slope_pointY = std::tan(angle*TO_RAD) * w;

	CPoint top_slope(w, h - top_slope_pointY);

	//105, 39,29
	COLORREF slope_clr(RGB(105, 39, 29));
	CPen nPen(BS_SOLID, 5, slope_clr);
	CBrush nBrush(HS_FDIAGONAL, slope_clr);

	std::vector<CPoint> triangle_pts = { 
		{0, cln_rect.bottom}, {cln_rect.right, cln_rect.bottom}, top_slope
	};

	auto old_pen = pDC->SelectObject(&nPen);
	auto old_brush = pDC->SelectObject(&nBrush);

	pDC->Polygon(triangle_pts.data(), triangle_pts.size());

	pDC->SelectObject(old_pen);
	pDC->SelectObject(old_brush);
}

void CRGKolokvijum2014View::DrawBackground(CDC* pDC) {
	CRect cln_rect;
	GetClientRect(&cln_rect);

	//105, 39,29
	
	CPen nPen(BS_SOLID, 5, BG_COLOR);
	CBrush nBrush(BG_COLOR);

	auto old_pen = pDC->SelectObject(&nPen);
	auto old_brush = pDC->SelectObject(&nBrush);

	pDC->Rectangle(cln_rect);

	pDC->SelectObject(old_pen);
	pDC->SelectObject(old_brush);
}

void CRGKolokvijum2014View::DrawCar(CDC* pDC, int x, int y, int w, int h) {
	

	CRect cln_rect;
	GetClientRect(&cln_rect);

	Translate(pDC, x, -y, right_mult);
	pDC->PlayMetaFile(car, CRect(w , -h, 0, 0));
	Translate(pDC, -x, 0, right_mult);
	
}

void CRGKolokvijum2014View::DrawWeel(CDC* pDC, int x, int y, int r, float angle) {
	CRect cln_rect;
	GetClientRect(&cln_rect);
	
	CDC memDC;
	if (!memDC.CreateCompatibleDC(pDC)) {
		return; // Failed to create compatible DC
	}


	CBitmap* pBitmap = weel.GetBitmap();
	
	CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);


	pDC->SetStretchBltMode(HALFTONE); // Smooth the bitmap during stretching

	pDC->SetBrushOrg(0, 0);


	
	Translate(pDC, x + r, y + r, right_mult); //rotacija oko centra
	Rotate(pDC, -360*(car_x/(2*r*M_PI)), right_mult);
	Translate(pDC, -r, -r, right_mult);
	
	pDC->TransparentBlt(0, 0, r*2, r*2, &memDC, 52, 15, 376, 376, CLR_TRANSPARENT);
	
	Translate(pDC, r, r, right_mult);
	Rotate(pDC, 360 * (car_x / (2 * r * M_PI)), right_mult);
	Translate(pDC, -x - r, -y - r, right_mult);
	
	// Restore the old bitmap in the memory DC
	memDC.SelectObject(pOldBitmap);

	
}

void CRGKolokvijum2014View::Scale(CDC* pDC, float sx, float sy, bool right_mult) {
	m_trans.eM11 = sx;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = sy;
	
	m_trans.eDx = 0;
	m_trans.eDy = 0;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRGKolokvijum2014View::Rotate(CDC* pDC, float angle, bool right_mult) {
	
	auto rad_angle = angle * TO_RAD;
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

void CRGKolokvijum2014View::Translate(CDC* pDC, float dx, float dy, bool right_mult) {
	m_trans.eM11 = 1;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = 1;

	m_trans.eDx = dx;
	m_trans.eDy = dy;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CRGKolokvijum2014View::OnDraw(CDC* pDC)
{
	CRGKolokvijum2014Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect cln_rect;
	GetClientRect(&cln_rect);

	int weel1_x_offset = -155, weel2_x_offset = 135;
	int car_width = 450;
	int weel_r = 38;
	int car_y = cln_rect.Height() - weel_r/2, car_height = 450 / 2.5f;
	CPoint car_center = { car_x + car_width / 2, car_y - car_height / 2 };
	int weel1_x = car_center.x + weel1_x_offset - weel_r;
	int weel2_x = car_center.x + weel2_x_offset - weel_r;
	int weel_y = -car_height/2 + weel_r;
	
	CDC *memDC = new CDC();
	if (!memDC->CreateCompatibleDC(pDC))
		return;

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, cln_rect.Width(), cln_rect.Height());
	memDC->SelectObject(&memBitmap);

	// Fill the memory DC with white background
	memDC->FillSolidRect(0, 0, cln_rect.Width(), cln_rect.Height(), RGB(255, 255, 255));



	auto old_gm_mode = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old_transform;
	pDC->GetWorldTransform(&old_transform);

	DrawBackground(memDC);
	DrawGround(memDC, slope_angle);

	Translate(memDC, 0, cln_rect.Height(), right_mult);
	Rotate(memDC, slope_angle, right_mult);

	DrawCar(memDC, car_x, weel_r/2, car_width, car_height);
	
	DrawWeel(memDC, weel1_x, weel_y, weel_r, 0);
	DrawWeel(memDC, weel2_x, weel_y, weel_r, 0);

	memDC->SetWorldTransform(&old_transform);

	memDC->SetGraphicsMode(old_gm_mode);

	pDC->BitBlt(0, 0, cln_rect.Width(), cln_rect.Height(),
				memDC, 0, 0, SRCCOPY);

	// TODO: add draw code for native data here
}


// CRGKolokvijum2014View printing

BOOL CRGKolokvijum2014View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGKolokvijum2014View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGKolokvijum2014View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}




// CRGKolokvijum2014View diagnostics

#ifdef _DEBUG
void CRGKolokvijum2014View::AssertValid() const
{
	CView::AssertValid();
}

void CRGKolokvijum2014View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGKolokvijum2014Doc* CRGKolokvijum2014View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGKolokvijum2014Doc)));
	return (CRGKolokvijum2014Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGKolokvijum2014View message handlers


void CRGKolokvijum2014View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	switch (nChar) {
	case VK_LEFT:
		car_x -= 5;
		Invalidate();
		break;
	case VK_UP:
		if (slope_angle + rot_angle <= MAX_SLOPE_ANGLE) {
			slope_angle += rot_angle;
			Invalidate();
		}
		break;
	case VK_RIGHT:
		car_x += 5;
		Invalidate();
		break;
	case VK_DOWN:
		if (slope_angle - rot_angle >= MIN_SLOPE_ANGLE) {
			slope_angle -= rot_angle;
			Invalidate();
		}
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CRGKolokvijum2014View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
}
