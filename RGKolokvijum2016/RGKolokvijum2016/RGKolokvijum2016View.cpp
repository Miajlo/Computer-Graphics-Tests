
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
#include<cmath>
#include<corecrt_math_defines.h>
// CRGKolokvijum2016View

IMPLEMENT_DYNCREATE(CRGKolokvijum2016View, CView)

BEGIN_MESSAGE_MAP(CRGKolokvijum2016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
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

void CRGKolokvijum2016View::Rotate(CDC* pDC, float angle, bool right_mult) {
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

void CRGKolokvijum2016View::Scale(CDC* pDC, float dx, float dy, bool right_mult) {
	m_trans.eM11 = 1;
	m_trans.eM12 = 0;
	m_trans.eM21 = 0;
	m_trans.eM22 = 1;

	m_trans.eDx = dx;
	m_trans.eDy = dy;

	pDC->ModifyWorldTransform(&m_trans, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CRGKolokvijum2016View::DrawStick(CDC* pDC, int w) {

	std::vector<CPoint> stick_fhalf = {
		{0,0}, { w / 100, 0}, {w / 98, 2 * w / 3}, {-w / 98, 2 * w / 3}
	};
	CPoint shalf_start = { stick_fhalf[3].x, stick_fhalf[3].y };
	std::vector<CPoint> stick_shalf = {
		shalf_start, 
		{ shalf_start.x + w / 50, shalf_start.y},
		{shalf_start.x + w / 50, shalf_start.y + w / 3},
		{shalf_start.x, shalf_start.y + w / 3},
	};
	CBrush nova(RGB(255, 190, 128));

	int top_y = w;

	CRect elipse(0, w / 50, w / 50, 0);

	auto old_brush = pDC->SelectObject(&nova);

	pDC->Polygon(stick_fhalf.data(), stick_fhalf.size());

	pDC->SelectObject(&old_brush);

	CBrush nova_upper(RGB(73, 40, 10));

	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);

	pDC->SelectObject(&nova_upper);

	Translate(pDC, -w / 100, w - w / 100, right_mult);
	pDC->Ellipse(elipse);

	pDC->SetWorldTransform(&old_trans);

	pDC->Polygon(stick_shalf.data(), stick_shalf.size());
	

	pDC->SelectObject(&old_brush);

	CPen nova_olovka(BS_SOLID, 1, RGB(255, 255, 255));

	auto old_pen = pDC->SelectObject(&nova_olovka);

	pDC->MoveTo(2, 0);

	pDC->LineTo(2 - w / 100, w + w / 110);

	pDC->MoveTo(0, 0);

	pDC->SelectObject(&old_pen);


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

void CRGKolokvijum2016View::DrawBall(CDC* pDC, int r) {

	CRect elipse(-r, -r, r, r);
	CBrush nova(RGB(128, 0, 0));

	auto old_brush = pDC->SelectObject(&nova);



	pDC->Ellipse(elipse);

	pDC->SelectObject(old_brush);

}

void CRGKolokvijum2016View::StickBallMove(bool dir) {

	if (toggle_move && stick_y_offset - 4 <= -33) //fix hardcode later 
		toggle_move = !toggle_move;
	
	else if (!toggle_move && ball_y_offset == 0)
		toggle_move = !toggle_move;

	if (!toggle_move && !dir)
		ball_y_offset = 0;

	if (!toggle_move && dir)
		ball_y_offset -= y_mov;

	if (toggle_move && dir)
		stick_y_offset -= y_mov;

	if (toggle_move && !dir)
		stick_y_offset += y_mov;


}

void CRGKolokvijum2016View::OnDraw(CDC* pDC) {
	CRGKolokvijum2016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);
	int border_width = 40, ball_r = 10;
	float r = 40;
	

	int old_mode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old_trans;
	pDC->GetWorldTransform(&old_trans);

	DrawTable(pDC, clientRect);

	DrawBorder(pDC, clientRect, border_width);

	DrawHoles(pDC, clientRect, r, border_width);

	Translate(pDC, clientRect.Width() / 2, clientRect.Height() / 2, right_mult);
	Rotate(pDC, stick_angle, right_mult);
	
	Translate(pDC, 0, stick_y_offset + 4 * ball_r, right_mult);

	DrawStick(pDC, clientRect.Width() / 2);


	Translate(pDC, 0, -stick_y_offset - 4 * ball_r, right_mult);
	
	Translate(pDC, clientRect.Width() / 400, ball_y_offset, right_mult);

	DrawBall(pDC, ball_r);


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


void CRGKolokvijum2016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case VK_LEFT:
		stick_angle += rot_step;
		Invalidate();
		break;

	case VK_RIGHT:
		stick_angle -= rot_step;
		Invalidate();
		break;
	case VK_UP:
		StickBallMove(true);
		Invalidate();
		break;
	case VK_DOWN:
		StickBallMove(false);
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CRGKolokvijum2016View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
}
