
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
	ON_WM_KEYDOWN()
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

	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);
	int old_mode = pDC->SetGraphicsMode(GM_ADVANCED);

	DrawTransformers(pDC);

	pDC->SetWorldTransform(&old_trans);
	pDC->SetGraphicsMode(old_mode);

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

	CDC* memDC = new CDC();
	if (!memDC->CreateCompatibleDC(pDC)) {
		delete memDC;
		return;
	}

	CBitmap* bitmap = img->GetBitmap();
	
	auto old_bitmap = memDC->SelectObject(bitmap);

	memDC->SetStretchBltMode(HALFTONE);

	memDC->SetBrushOrg(0, 0);

	pDC->TransparentBlt(0, 0, img->Width(), img->Height(),
						memDC, 0, 0, img->Width(),img->Height(), memDC->GetPixel(0,0));

	memDC->SelectObject(old_bitmap);

	old_bitmap = bitmap = nullptr;

	delete memDC;
	memDC = nullptr;


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
	m_trans.eM21 = sinus;
	m_trans.eM22 = cosin;

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
	
	XFORM old;


	//Translate(pDC, -27, -17, right_mult);
	Rotate(pDC, arms_rot_angle, right_mult);
	Rotate(pDC, body1_rot_angle, right_mult);
	Translate(pDC, -34, -31, right_mult);
	
	DrawTransparentImage(pDC, &arm1);

	Translate(pDC, 34, 31, right_mult);
}

void CRGKolokvijum2019View::DrawArm2(CDC* pDC) {
	
	
	XFORM old;
	pDC->GetWorldTransform(&old);
	Rotate(pDC, arms_rot_angle, right_mult);
	Rotate(pDC, body1_rot_angle, right_mult);
	Translate(pDC, 176, 71, right_mult);
	Rotate(pDC, 180 + arm2_rot_angle, right_mult);
	Translate(pDC, -23, -61, right_mult);
	
	
	DrawTransparentImage(pDC, &arm2);

	pDC->SetWorldTransform(&old);
	
	
}

void CRGKolokvijum2019View::DrawLeg1(CDC* pDC) {
	Rotate(pDC, curr_rot_angle, right_mult);
	Translate(pDC, -30, -125, right_mult);
	DrawTransparentImage(pDC, &leg1);
	Translate(pDC, 30, 125, right_mult);
	
	
}

void CRGKolokvijum2019View::DrawLeg2(CDC* pDC) {
	Rotate(pDC, -curr_rot_angle, right_mult);
	Translate(pDC, -35, -60, right_mult);
	
	DrawTransparentImage(pDC, &leg2);
		
	Translate(pDC, 35, 60, right_mult);
	Rotate(pDC, curr_rot_angle, right_mult);

	
}

void CRGKolokvijum2019View::DrawBody1(CDC* pDC) {
	
	Translate(pDC, arm1.Width() - 30, 3, right_mult);
	Rotate(pDC, arms_rot_angle, right_mult);
	Translate(pDC, -26, -133, right_mult);
	
	
	DrawTransparentImage(pDC, &body1);
	

	Translate(pDC, 26, 133, right_mult);
	//Rotate(pDC, -arms_rot_angle, right_mult);
	Translate(pDC, 185, -50, right_mult);
	
}

void CRGKolokvijum2019View::DrawTransformers(CDC* pDC) {
	CRect bg;
	GetClientRect(&bg);

	Translate(pDC, 250, 2 * bg.Height() / 3, right_mult);


	/*DrawBody1(pDC);
	DrawArm1(pDC);*/
	//DrawArm2(pDC);
	DrawLeg2(pDC);
	
	DrawLeg1(pDC);
	
	DrawBody1(pDC);
	
	DrawArm2(pDC);
	
	DrawArm1(pDC);
	
	
	
}


void CRGKolokvijum2019View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {

	case VK_LEFT:
		curr_rot_angle += ROT_STEP;
		Invalidate();
		break;
	case VK_RIGHT:
		curr_rot_angle -= ROT_STEP;
		Invalidate();
		break;
	case 'W':
		body1_rot_angle += ROT_STEP;
		Invalidate();
		break;
	case 'S':
		body1_rot_angle -= ROT_STEP;
		Invalidate();
		break;
	case 'T':
		arm2_rot_angle += ROT_STEP;
		Invalidate();
		break;
	case 'G':
		arm2_rot_angle -= ROT_STEP;
		Invalidate();
		break;
	case 'Q':
		arms_rot_angle += ROT_STEP;
		Invalidate();
		break;
	case 'E':
		arms_rot_angle -= ROT_STEP;
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
