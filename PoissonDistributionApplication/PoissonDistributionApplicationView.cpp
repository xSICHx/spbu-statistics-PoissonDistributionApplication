
// PoissonDistributionApplicationView.cpp : implementation of the CPoissonDistributionApplicationView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PoissonDistributionApplication.h"
#endif

#include "PoissonDistributionApplicationDoc.h"
#include "PoissonDistributionApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPoissonDistributionApplicationView

IMPLEMENT_DYNCREATE(CPoissonDistributionApplicationView, CView)

BEGIN_MESSAGE_MAP(CPoissonDistributionApplicationView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CPoissonDistributionApplicationView construction/destruction

CPoissonDistributionApplicationView::CPoissonDistributionApplicationView() noexcept
{
	// TODO: add construction code here

}

CPoissonDistributionApplicationView::~CPoissonDistributionApplicationView()
{
}

BOOL CPoissonDistributionApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPoissonDistributionApplicationView drawing

void CPoissonDistributionApplicationView::OnDraw(CDC* /*pDC*/)
{
	CPoissonDistributionApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CPoissonDistributionApplicationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPoissonDistributionApplicationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPoissonDistributionApplicationView diagnostics

#ifdef _DEBUG
void CPoissonDistributionApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CPoissonDistributionApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPoissonDistributionApplicationDoc* CPoissonDistributionApplicationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPoissonDistributionApplicationDoc)));
	return (CPoissonDistributionApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


// CPoissonDistributionApplicationView message handlers
