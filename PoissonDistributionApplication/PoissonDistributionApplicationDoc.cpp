
// PoissonDistributionApplicationDoc.cpp : implementation of the CPoissonDistributionApplicationDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PoissonDistributionApplication.h"
#endif

#include "PoissonDistributionApplicationDoc.h"
#include "MainFrm.h"
#include "SettingsDialog.h"
#include "PoissonDistributionApplicationView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPoissonDistributionApplicationDoc

IMPLEMENT_DYNCREATE(CPoissonDistributionApplicationDoc, CDocument)

BEGIN_MESSAGE_MAP(CPoissonDistributionApplicationDoc, CDocument)
	ON_COMMAND(ID_SETTINGS, &CPoissonDistributionApplicationDoc::OnSettings)
END_MESSAGE_MAP()


// CPoissonDistributionApplicationDoc construction/destruction

//CPoissonDistributionApplicationDoc::CPoissonDistributionApplicationDoc() noexcept
//{
//	rnd_gen = mt19937(100);
//	d0 = Distribution(5);
//	d1 = Distribution(6);
//	mod_meth = ModelingMethod::inverse;
//	ps = new PoissonSampleInverse(rnd_gen, d0, 100);
//	chi = Chi2Histortam(d0, *ps);
//
//}

CPoissonDistributionApplicationDoc::~CPoissonDistributionApplicationDoc()
{
	delete[] p_array;
	delete ps;
}

BOOL CPoissonDistributionApplicationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CMainFrame* p_Frame = (CMainFrame*)AfxGetMainWnd();
	SetTitle(L"Poisson distribution");
	p_Frame->SetWindowText(L"Poisson distribution");

	return TRUE;
}




// CPoissonDistributionApplicationDoc serialization

void CPoissonDistributionApplicationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CPoissonDistributionApplicationDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CPoissonDistributionApplicationDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPoissonDistributionApplicationDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPoissonDistributionApplicationDoc diagnostics

#ifdef _DEBUG
void CPoissonDistributionApplicationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPoissonDistributionApplicationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPoissonDistributionApplicationDoc commands


void CPoissonDistributionApplicationDoc::OnSettings()
{
	int mod;
	if (mod_meth == ModelingMethod::inverse)
		mod = 0;
	else if (mod_meth == ModelingMethod::random_variables)
		mod = 1;
	else
		mod = 2;

	SettingsDialog d(chi, d0, d1, ps, mod, N_p_values, p_partition);
	if (d.DoModal() == IDOK) {
		if (mod != d.m_mod_method) {
			delete ps;
			if (d.m_mod_method == (int)ModelingMethod::inverse)
				ps = new PoissonSampleInverse(rnd_gen, d0, d.m_sample_size);
			else if (d.m_mod_method == (int)ModelingMethod::random_variables)
				ps = new PoissonSampleRandomVariables(rnd_gen, d0, d.m_sample_size);
			else
				ps = new PoissonSampleInverseTable(rnd_gen, d0, d.m_sample_size);
		}


		d0.set_lambda(d.m_lambda_h0);
		d1.set_lambda(d.m_lambda_h1);
		ps->set_N(d.m_sample_size);
		chi.set_data(d0, *ps);
		mod_meth = static_cast<ModelingMethod>(d.m_mod_method);
		p_partition = d.m_p_level_partition;
		N_p_values = d.m_n_iterations;

		POSITION pos = GetFirstViewPosition();
		if (pos != nullptr) {
			CPoissonDistributionApplicationView* pView = static_cast<CPoissonDistributionApplicationView*>(GetNextView(pos));
			pView->draw_state = CPoissonDistributionApplicationView::DrawState::nothing;
		}
		UpdateAllViews(nullptr);
	}
}
