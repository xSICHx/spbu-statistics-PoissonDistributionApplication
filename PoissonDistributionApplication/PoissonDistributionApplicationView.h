
// PoissonDistributionApplicationView.h : interface of the CPoissonDistributionApplicationView class
//

#pragma once


class CPoissonDistributionApplicationView : public CView
{
protected: // create from serialization only
	CPoissonDistributionApplicationView() noexcept;
	DECLARE_DYNCREATE(CPoissonDistributionApplicationView)

// Attributes
public:
	CPoissonDistributionApplicationDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CPoissonDistributionApplicationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PoissonDistributionApplicationView.cpp
inline CPoissonDistributionApplicationDoc* CPoissonDistributionApplicationView::GetDocument() const
   { return reinterpret_cast<CPoissonDistributionApplicationDoc*>(m_pDocument); }
#endif

