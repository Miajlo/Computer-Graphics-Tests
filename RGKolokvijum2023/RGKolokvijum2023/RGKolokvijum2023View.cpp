
// RGKolokvijum2023View.cpp : implementation of the CRGKolokvijum2023View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RGKolokvijum2023.h"
#endif
#include<corecrt_math_defines.h>
#include<cmath>
#include <algorithm>
#include "RGKolokvijum2023Doc.h"
#include "RGKolokvijum2023View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRGKolokvijum2023View

IMPLEMENT_DYNCREATE(CRGKolokvijum2023View, CView)

BEGIN_MESSAGE_MAP(CRGKolokvijum2023View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CRGKolokvijum2023View construction/destruction

CRGKolokvijum2023View::CRGKolokvijum2023View() noexcept
{
	// TODO: add construction code here

	glava.Load(_T("res\\glava.png"));
	telo.Load(_T("res\\telo.png"));
	pozadina.Load(_T("res\\pozadina.jpg"));
	nadkolenica.Load(_T("res\\nadkolenica.png"));
	podkolenica.Load(_T("res\\podkolenica.png"));
	pozadina.Load(_T("res\\pozadina.png"));
	saka.Load(_T("res\\saka.png"));
	nadlaktica.Load(_T("res\\nadlaktica.png"));
	podlaktica.Load(_T("res\\podlaktica.png"));
	stopalo.Load(_T("res\\stopalo.png"));

	right_mult = false;
}

CRGKolokvijum2023View::~CRGKolokvijum2023View()
{
}

BOOL CRGKolokvijum2023View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGKolokvijum2023View drawing

void CRGKolokvijum2023View::translate(CDC* pDC, float dx, float dy, bool rmult) {
	m_trans.eM11 = 1;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = 1;

	m_trans.eDx = dx;
	m_trans.eDy = dy;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CRGKolokvijum2023View::rotate(CDC* pDC, float angle, bool rmult) {
	auto rad_angle = angle * M_PI / 180;

	auto cosin = std::cos(rad_angle);
	auto sinus = std::sin(rad_angle);
	m_trans.eM11 = cosin;
	m_trans.eM12 = sinus;
	m_trans.eM21 = -sinus;
	m_trans.eM22 = cosin;

	m_trans.eDx = 0;
	m_trans.eDy = 0;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CRGKolokvijum2023View::scale(CDC* pDC, float sx, float sy, bool rmult) {
	m_trans.eM11 = sx;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = sy;

	m_trans.eDx = 0;
	m_trans.eDy = 0;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CRGKolokvijum2023View::draw_img_transparent(CDC* pDC, DImage* img) {
	CBitmap* bit_mp = img->GetBitmap();

	CDC* memDC = new CDC();

	if (!memDC->CreateCompatibleDC(pDC))
		return;



	auto old_bmp = memDC->SelectObject(bit_mp);

	auto color = memDC->GetPixel(0, 0);

	pDC->SetStretchBltMode(HALFTONE);

	pDC->SetBrushOrg(0, 0);

	pDC->TransparentBlt(0, 0, img->Width(), img->Height(),
		memDC, 0, 0, img->Width(), img->Height(), color);

	memDC->SelectObject(old_bmp);

	memDC->DeleteDC();
	delete memDC;
	memDC = nullptr;
}

void CRGKolokvijum2023View::draw_half(CDC* pDC) {
	XFORM old_trans;
	

	pDC->GetWorldTransform(&old_trans);
	
	translate(pDC, -telo.Width(), -telo.Height(), right_mult);


	draw_img_transparent(pDC, &telo);

	
	translate(pDC, -nadlaktica.Width()/6, 30, right_mult);
	translate(pDC, 35, 35, right_mult);
	rotate(pDC, GHangle, right_mult);
	translate(pDC, -35, -35, right_mult);

	draw_img_transparent(pDC, &nadlaktica);


	translate(pDC, -podlaktica.Width()/6, 2 * nadlaktica.Height() / 3, right_mult);
	translate(pDC, 30, 33, right_mult);
	rotate(pDC, DFangle, right_mult);
	translate(pDC, -30, -33, right_mult);

	draw_img_transparent(pDC, &podlaktica);

	translate(pDC, 30, 33, right_mult);
	
	translate(pDC, -saka.Width()/2, podlaktica.Height() / 2 + saka.Height() / 5, right_mult);
	translate(pDC, 25, 3, right_mult);
	rotate(pDC, ASagnle, right_mult);
	translate(pDC, -25, -3, right_mult);

	draw_img_transparent(pDC, &saka);

	

	pDC->SetWorldTransform(&old_trans);

	translate(pDC, -nadkolenica.Width()-10, -80, right_mult);

	draw_img_transparent(pDC, &nadkolenica);

	translate(pDC, 0, nadkolenica.Height() - 70, right_mult);

	draw_img_transparent(pDC, &podkolenica);

	translate(pDC, 0, nadkolenica.Height()+stopalo.Height()/4, right_mult);

	draw_img_transparent(pDC, &stopalo);

	
	pDC->SetWorldTransform(&old_trans);
}

void CRGKolokvijum2023View::draw_head(CDC* pDC) {
	translate(pDC, 0, -telo.Height(), right_mult);
	translate(pDC, -glava.Width() / 2, -5 * glava.Height() / 6, right_mult);

	draw_img_transparent(pDC, &glava);

	translate(pDC, glava.Width() / 2, 5 * glava.Height() / 6, right_mult);
}

void CRGKolokvijum2023View::draw_robot(CDC* pDC) {
	int old_gm = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);

	CRect rGlava(0, 0, glava.Width(), glava.Height());

	CRect pozadinaRect(0, 0, pozadina.Width(), pozadina.Height());


	pozadina.Draw(pDC, pozadinaRect, pozadinaRect);

	XFORM center_trans;

	//do it here to stop multiple calls in draw_half method
	translate(pDC, pozadina.Width() / 2, pozadina.Height() / 2, right_mult);
	rotate(pDC, curr_rot_angle, right_mult);

	pDC->GetWorldTransform(&center_trans);


	draw_half(pDC);

	//drawing mirrored left side
	scale(pDC, -1, 1, right_mult);

	draw_half(pDC);

	scale(pDC, -1, 1, right_mult);


	pDC->SetWorldTransform(&center_trans);


	draw_head(pDC);

	pDC->SetWorldTransform(&old_trans);
	pDC->SetGraphicsMode(old_gm);
}





void CRGKolokvijum2023View::OnDraw(CDC* pDC) {
	CRGKolokvijum2023Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();

	if (!memDC->CreateCompatibleDC(pDC))
		return;

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC->SelectObject(&memBitmap);

	// Fill the memory DC with white background
	memDC->FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));

	
	draw_robot(pDC);

	//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	// TODO: add draw code for native data here
}


// CRGKolokvijum2023View printing

BOOL CRGKolokvijum2023View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGKolokvijum2023View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGKolokvijum2023View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRGKolokvijum2023View diagnostics

#ifdef _DEBUG
void CRGKolokvijum2023View::AssertValid() const
{
	CView::AssertValid();
}

void CRGKolokvijum2023View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGKolokvijum2023Doc* CRGKolokvijum2023View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGKolokvijum2023Doc)));
	return (CRGKolokvijum2023Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGKolokvijum2023View message handlers


void CRGKolokvijum2023View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case '1':
		curr_rot_angle -= rot_step;
		Invalidate();
		break;
	case '2':
		curr_rot_angle += rot_step;
		Invalidate();
		break;
	case 'A':
		ASagnle -= rot_step;
		ASagnle = std::max<float>(ASagnle, hand_min_angle);
		Invalidate();
		break;
	case 'S':
		ASagnle += rot_step;
		ASagnle = std::min<float>(ASagnle, hand_max_angle);
		Invalidate();
		break;
	case 'D':
		DFangle -= rot_step;
		DFangle = std::max<float>(DFangle, pod_min_angle);
		Invalidate();
		break;
	case 'F':
		DFangle+= rot_step;
		DFangle = std::min<float>(DFangle, pod_max_angle);
		Invalidate();
		break;
	case 'G':
		GHangle-= rot_step;
		GHangle = std::max<float>(GHangle, nad_min_angle);
		Invalidate();
		break;
	case 'H':
		GHangle += rot_step;
		GHangle = std::min<float>(GHangle, nad_max_angle);
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CRGKolokvijum2023View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
