
// PoissonDistributionApplicationView.cpp : implementation of the CPoissonDistributionApplicationView class
//
#pragma once
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
	ON_COMMAND(ID_GENERATESAMPLE, &CPoissonDistributionApplicationView::OnGeneratesample)
	ON_COMMAND(ID_P, &CPoissonDistributionApplicationView::OnP)
	ON_COMMAND(ID_POWERDISTRIBUTION, &CPoissonDistributionApplicationView::OnPowerdistribution)
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

double findMax(double* arr, size_t n) {

    double maxVal = arr[0];
    for (size_t i = 1; i < n; ++i) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }

    return maxVal;
}

// CPoissonDistributionApplicationView drawing
void DrawHistogram(CDC* pDC, double* th_values, double* sample_values, double* indexes, int distr_len, CRect rect)
{
    // ��������� ������� ��� ���� x � y
    double min_y = 0.0;
    double max_y = max(findMax(th_values, distr_len), findMax(sample_values, distr_len));
    //double max_y = 1;


    // ��������� ������� ������� ���������
    int x_axis_length = rect.Width() - 200; // ����� ��� x
    int y_axis_length = rect.Height() - 200; // ����� ��� y




    // ��������� ����
    pDC->MoveTo(rect.left + 50, rect.top + y_axis_length + 50); // ������ ��� x
    pDC->LineTo(rect.left + x_axis_length + 50, rect.top + y_axis_length + 50); // ����� ��� x
    pDC->MoveTo(rect.left + 50, rect.top + 50); // ������ ��� y
    pDC->LineTo(rect.left + 50, rect.top + y_axis_length + 50); // ����� ��� y


    // ��������� �������� �� ����
    CString min_y_str, max_y_str;
    min_y_str.Format(L"%.2f", min_y);
    max_y_str.Format(L"%.2f", max_y);
    pDC->TextOutW(rect.left + 10, rect.top + y_axis_length + 40, min_y_str); // ����������� �������� �� ��� y
    pDC->TextOutW(rect.left + 10, rect.top + 30, max_y_str); // ������������ �������� �� ��� y




    // ��������� �������� ��� th_values
    double full_bar_width = (x_axis_length) / distr_len;
    double bar_width = full_bar_width * 0.8;
    double shift = full_bar_width * 0.2;
    for (size_t i = 0; i < distr_len; ++i)
    {
        int bar_height = static_cast<int>((th_values[i] - min_y) / (max_y - min_y) * y_axis_length);


        CRect bar_rect(rect.left + 50 + static_cast<int>(i * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50 - bar_height,
            rect.left + 50 + static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50);
        pDC->FillSolidRect(&bar_rect, RGB(255, 192, 203)); // ������� ���� ��� th_values
    }


    bar_width = full_bar_width * 0.6;
    shift = full_bar_width * 0.4;
    // ��������� �������� ��� sample_values
    for (size_t i = 0; i < distr_len; ++i)
    {
        int bar_height = static_cast<int>((sample_values[i] - min_y) / (max_y - min_y) * y_axis_length);
        CRect bar_rect(rect.left + 50 + static_cast<int>(i * bar_width) + static_cast<int>(i * shift) + static_cast<int>(shift * 0.25),
            rect.top + y_axis_length + 50 - bar_height,
            rect.left + 50 + static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift) + static_cast<int>(shift * 0.25),
            rect.top + y_axis_length + 50);
        pDC->FillSolidRect(&bar_rect, RGB(0, 100, 0)); // Ҹ���-������ ���� ��� sample_values
    }


    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkColor(RGB(255, 255, 255));


    // ��������� �������� �� ��� x
    for (size_t i = 0; i < distr_len; ++i)
    {
        CString index_str;
        if (indexes[i] == (int)indexes[i])
            index_str.Format(L"%.0f", indexes[i]);
        else
            index_str.Format(L"%.3f", indexes[i]);
        pDC->TextOutW(rect.left + 50 + static_cast<int>(i * bar_width) + static_cast<int>(i * shift) +
            static_cast<int>(shift * 0.25),
            rect.top + y_axis_length + 60, index_str); // �������� �� ��� x
    }

    // ������� ����
    pDC->TextOutW(rect.left + x_axis_length / 2, rect.top + y_axis_length + 80, L"Index");
    pDC->TextOutW(rect.left + 10, rect.top + 30 - 20, L"Value");
}

void DrawSampleHist(CDC* pDC, CPoissonDistributionApplicationDoc* pDoc, CRect rect) {
    
    Chi2Histortam& chi = pDoc->getChi();
    PoissonSample* ps = pDoc->GetPs();
    int distr_len = chi.get_distr_len();
    double* th_freq = chi.get_th_freq();
    double* sample_freq = chi.get_sample_freq();
    double* indexes = new double[distr_len];
    for (size_t i = 0; i < distr_len; i++)
    {
        indexes[i] = i;
    }
    DrawHistogram(pDC, th_freq, sample_freq, indexes, distr_len, rect);

    int x_axis_length = rect.Width() - 200; // ����� ��� x
    int y_axis_length = rect.Height() - 200; // ����� ��� y

    std::wstring result = L"Chi^2 = " + std::to_wstring(chi.get_chi()) +
        L"; d.f. = " + std::to_wstring((int)chi.get_df()) +
        L"; p-value = " + std::to_wstring(chi.get_p()) +
        L"; Sample size = " + std::to_wstring(ps->get_N()) +
        L"; H0: lambda = " + std::to_wstring(pDoc->GetD0().get_lambda()) +
        L"; Method: " + ((int)pDoc->getMod_meth() == 0 ? L"inverse." :
            ((int)pDoc->getMod_meth() == 1 ? L"random variables." : L"inverse table"));
    CString res(result.c_str());
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 120, L"Sample of poisson distribution.");
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 160, res);



    // �������
    int rectWidth = 10;
    int rectHeight = 10;
    int rectX = rect.Width() - rectWidth - 200; // 10 - ������ �� ������ �������
    int rectY = 10; // 10 - ������ �� ������� �������
    // ������� ����� � ������� ������ (RGB(255, 192, 203))
    CBrush pinkBrush(RGB(255, 192, 203));
    // ��������� ������� �����
    CBrush* pOldBrush = pDC->SelectObject(&pinkBrush);
    // ������ ������� �������������
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // ��������������� ���������� �����
    pDC->SelectObject(pOldBrush);
    // ������� �����
    CString text = L"Theoretical";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    // �������
    rectWidth = 10;
    rectHeight = 10;
    rectX = rect.Width() - rectWidth - 200; // 10 - ������ �� ������ �������
    rectY = 40; // 10 - ������ �� ������� �������
    // ������� ����� � ������� ������ (RGB(255, 192, 203))
    CBrush greenBrush(RGB(0, 100, 0));
    // ��������� ������� �����
    pOldBrush = pDC->SelectObject(&greenBrush);
    // ������ ������� �������������
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // ��������������� ���������� �����
    pDC->SelectObject(pOldBrush);
    // ������� �����
    text = L"Sample";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    delete[] indexes;
}




void DrawLines(CDC* pDC, double* th_values, double* sample_values, int distr_len, CRect rect) {
    // ��������� ������� ��� ���� x � y
    double min_y = 0.0;
    double max_y = max(findMax(th_values, distr_len), findMax(sample_values, distr_len));
    //double max_y = 1;


    // ��������� ������� ������� ���������
    int x_axis_length = rect.Width() - 200; // ����� ��� x
    int y_axis_length = rect.Height() - 200; // ����� ��� y




    // ��������� ����
    pDC->MoveTo(rect.left + 50, rect.top + y_axis_length + 50); // ������ ��� x
    pDC->LineTo(rect.left + x_axis_length + 50, rect.top + y_axis_length + 50); // ����� ��� x
    pDC->MoveTo(rect.left + 50, rect.top + 50); // ������ ��� y
    pDC->LineTo(rect.left + 50, rect.top + y_axis_length + 50); // ����� ��� y


    // ��������� �������� �� ����
    CString min_y_str, max_y_str;
    min_y_str.Format(L"%.2f", min_y);
    max_y_str.Format(L"%.2f", max_y);
    pDC->TextOutW(rect.left + 10, rect.top + y_axis_length + 40, min_y_str); // ����������� �������� �� ��� y
    pDC->TextOutW(rect.left + 10, rect.top + 30, max_y_str); // ������������ �������� �� ��� y




    // ��������� �������� ��� th_values
    double full_bar_width = (x_axis_length) / distr_len;
    double bar_width = full_bar_width * 0.8;
    double shift = full_bar_width * 0.2;
    double left_bottom_prev = 0;
    for (size_t i = 0; i < distr_len; ++i)
    {
        int bar_height_prev;
        if (i == 0)
            bar_height_prev = 0;
        else
            bar_height_prev = static_cast<int>((th_values[i-1] - min_y) / (max_y - min_y) * y_axis_length);
        int bar_height = static_cast<int>((th_values[i] - min_y) / (max_y - min_y) * y_axis_length);

        CPen pen(PS_SOLID, 2, RGB(255, 192, 203)); 
        CPen* pOldPen = pDC->SelectObject(&pen);
        
        pDC->MoveTo(rect.left + 50 + left_bottom_prev, rect.top + y_axis_length + 50 - bar_height_prev);
        pDC->LineTo(rect.left + 50 + static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50 - bar_height);

        left_bottom_prev = static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift);
        pDC->SelectObject(pOldPen);
        //CRect bar_rect(rect.left + 50 + static_cast<int>(i * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50 - bar_height,
        //    rect.left + 50 + static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50);
        //pDC->FillSolidRect(&bar_rect, RGB(255, 192, 203)); // ������� ���� ��� th_values
    }

    
    left_bottom_prev = 0;
    // ��������� �������� ��� sample_values
    for (size_t i = 0; i < distr_len; ++i)
    {
        int bar_height = static_cast<int>((sample_values[i] - min_y) / (max_y - min_y) * y_axis_length);

        int bar_height_prev;
        if (i == 0)
            bar_height_prev = 0;
        else
            bar_height_prev = static_cast<int>((sample_values[i - 1] - min_y) / (max_y - min_y) * y_axis_length);

        CPen pen(PS_SOLID, 2, RGB(0, 100, 0));
        CPen* pOldPen = pDC->SelectObject(&pen);
        
        pDC->MoveTo(rect.left + 50 + left_bottom_prev, rect.top + y_axis_length + 50 - bar_height_prev);
        pDC->LineTo(rect.left + 50 + static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50 - bar_height);
        left_bottom_prev = static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift);

        pDC->SelectObject(pOldPen);
    }


    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkColor(RGB(255, 255, 255));


    // ��������� �������� �� ��� x
    
    CString index_str;
    
    index_str.Format(L"%.0f", 1.0);
    pDC->TextOutW(rect.left + 50 + static_cast<int>(distr_len * bar_width) + static_cast<int>(distr_len * shift) +
            static_cast<int>(shift * 0.25),
            rect.top + y_axis_length + 60, index_str);



    // ������� ����
    pDC->TextOutW(rect.left + x_axis_length / 2, rect.top + y_axis_length + 80, L"Index");
    pDC->TextOutW(rect.left + 10, rect.top + 30 - 20, L"Value");
}

void DrawPHist(CDC* pDC, CPoissonDistributionApplicationDoc* pDoc, CRect rect) {
    double* indexes = new double[pDoc->Get_p_partition()];
    Chi2Histortam& chi = pDoc->getChi();
    PoissonSample* ps = pDoc->GetPs();
    for (size_t i = 1; i <= pDoc->Get_p_partition(); i++)
    {
        indexes[i - 1] = (double)i / pDoc->Get_p_partition();
    }
    DrawLines(pDC, indexes, pDoc->Get_p_array(), pDoc->Get_p_partition(), rect);


    int x_axis_length = rect.Width() - 200; // ����� ��� x
    int y_axis_length = rect.Height() - 200; // ����� ��� y

    std::wstring result =
        L"Sample count = " + std::to_wstring(pDoc->Get_N_p_values()) +
        L"; Sample size = " + std::to_wstring(ps->get_N()) +
        L"; H0: lambda = " + std::to_wstring(pDoc->GetD0().get_lambda()) +
        L"; Method: " + ((int)pDoc->getMod_meth() == 0 ? L"inverse." :
            ((int)pDoc->getMod_meth() == 1 ? L"random variables." : L"inverse table"));
    CString res(result.c_str());
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 120, L"Distribution of p-levels.");
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 160, res);



    // �������
    int rectWidth = 10;
    int rectHeight = 10;
    int rectX = rect.Width() - rectWidth - 200; // 10 - ������ �� ������ �������
    int rectY = 10; // 10 - ������ �� ������� �������
    // ������� ����� � ������� ������ (RGB(255, 192, 203))
    CBrush pinkBrush(RGB(255, 192, 203));
    // ��������� ������� �����
    CBrush* pOldBrush = pDC->SelectObject(&pinkBrush);
    // ������ ������� �������������
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // ��������������� ���������� �����
    pDC->SelectObject(pOldBrush);
    // ������� �����
    CString text = L"Uniform";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    // �������
    rectWidth = 10;
    rectHeight = 10;
    rectX = rect.Width() - rectWidth - 200; // 10 - ������ �� ������ �������
    rectY = 30; // 10 - ������ �� ������� �������
    // ������� ����� � ������� ������ (RGB(255, 192, 203))
    CBrush greenBrush(RGB(0, 100, 0));
    // ��������� ������� �����
    pOldBrush = pDC->SelectObject(&greenBrush);
    // ������ ������� �������������
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // ��������������� ���������� �����
    pDC->SelectObject(pOldBrush);
    // ������� �����
    text = L"p-level";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    delete[] indexes;
}


void DrawBetaHist(CDC* pDC, CPoissonDistributionApplicationDoc* pDoc, CRect rect) {
    double* indexes = new double[pDoc->Get_p_partition()];
    Chi2Histortam& chi = pDoc->getChi();
    PoissonSample*& ps = pDoc->GetPs();
    for (size_t i = 1; i <= pDoc->Get_p_partition(); i++)
    {
        indexes[i - 1] = (double)i / pDoc->Get_p_partition();
    }
    DrawLines(pDC, indexes, pDoc->Get_p_array(), pDoc->Get_p_partition(), rect);


    int x_axis_length = rect.Width() - 200; // ����� ��� x
    int y_axis_length = rect.Height() - 200; // ����� ��� y

    std::wstring result =
        L"Sample count = " + std::to_wstring(pDoc->Get_N_p_values()) +
        L"; Sample size = " + std::to_wstring(ps->get_N()) +
        L"; H0: lambda = " + std::to_wstring(pDoc->GetD0().get_lambda()) +
        L"; H1: lambda = " + std::to_wstring(pDoc->GetD1().get_lambda()) +
        L"; Method: " + ((int)pDoc->getMod_meth() == 0 ? L"inverse." :
            ((int)pDoc->getMod_meth() == 1 ? L"random variables." : L"inverse table"));
    CString res(result.c_str());
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 120, L"Distribution of p-levels.");
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 160, res);



    // �������
    int rectWidth = 10;
    int rectHeight = 10;
    int rectX = rect.Width() - rectWidth - 200; // 10 - ������ �� ������ �������
    int rectY = 10; // 10 - ������ �� ������� �������
    // ������� ����� � ������� ������ (RGB(255, 192, 203))
    CBrush pinkBrush(RGB(255, 192, 203));
    // ��������� ������� �����
    CBrush* pOldBrush = pDC->SelectObject(&pinkBrush);
    // ������ ������� �������������
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // ��������������� ���������� �����
    pDC->SelectObject(pOldBrush);
    // ������� �����
    CString text = L"Uniform";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    // �������
    rectWidth = 10;
    rectHeight = 10;
    rectX = rect.Width() - rectWidth - 200; // 10 - ������ �� ������ �������
    rectY = 30; // 10 - ������ �� ������� �������
    // ������� ����� � ������� ������ (RGB(255, 192, 203))
    CBrush greenBrush(RGB(0, 100, 0));
    // ��������� ������� �����
    pOldBrush = pDC->SelectObject(&greenBrush);
    // ������ ������� �������������
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // ��������������� ���������� �����
    pDC->SelectObject(pOldBrush);
    // ������� �����
    text = L"power";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    delete[] indexes;
}



void CPoissonDistributionApplicationView::OnDraw(CDC* pDC)
{
	CPoissonDistributionApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	if (draw_state == DrawState::sample_distribution) {
		CRect rect;
		GetClientRect(&rect);
		DrawSampleHist(pDC, pDoc, rect);
		//DrawHistogram(pDC, th_freq, sample_freq, indexes, rect);
	}
	else if (draw_state == DrawState::p_distribution) {
		CRect rect;
		GetClientRect(&rect);
		DrawPHist(pDC, pDoc, rect);
	}
	else if (draw_state == DrawState::beta_distribution) {
		CRect rect;
		GetClientRect(&rect);
		DrawBetaHist(pDC, pDoc, rect);
	}
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


void CPoissonDistributionApplicationView::OnGeneratesample()
{
	// TODO: Add your command handler code here
	draw_state = DrawState::sample_distribution;
	CPoissonDistributionApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Distribution& d0 = pDoc->GetD0();
	PoissonSample* ps = pDoc->GetPs();
	Chi2Histortam& chi = pDoc->getChi();
	chi.set_data(d0, *ps);
	chi.calc_chi();
	Invalidate();
}


void CPoissonDistributionApplicationView::OnP()
{
    draw_state = DrawState::p_distribution;
    CPoissonDistributionApplicationDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    Distribution& d = pDoc->GetD0();
    PoissonSample* ps = pDoc->GetPs();
    Chi2Histortam& chi = pDoc->getChi();
    double*& p_array = pDoc->Get_p_array();
    delete[] p_array;
    p_array = new double[pDoc->Get_p_partition()];
    for (int i = 0; i < pDoc->Get_p_partition(); i++) // ������������� ������� ������
    {
        p_array[i] = 0;
    }

    double p = 0;
    for (int i = 0; i < pDoc->Get_N_p_values(); i++) {
        chi.set_data(d, *ps);
        chi.calc_chi();
        p = chi.get_p();
        for (int j = pDoc->Get_p_partition() - 1; j >= int(p * pDoc->Get_p_partition()); --j) {
            ++p_array[j];
        }
    }

    for (int i = 0; i < pDoc->Get_p_partition(); ++i) {
        p_array[i] /= pDoc->Get_N_p_values();
    }
    Invalidate();
}


void CPoissonDistributionApplicationView::OnPowerdistribution()
{
    draw_state = DrawState::beta_distribution;
    CPoissonDistributionApplicationDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    Distribution& d = pDoc->GetD1();
    PoissonSample* ps = pDoc->GetPs();
    Chi2Histortam& chi = pDoc->getChi();
    double*& p_array = pDoc->Get_p_array();
    delete[] p_array;
    p_array = new double[pDoc->Get_p_partition()];
    for (int i = 0; i < pDoc->Get_p_partition(); i++) // ������������� ������� ������
    {
        p_array[i] = 0;
    }

    double p = 0;
    for (int i = 0; i < pDoc->Get_N_p_values(); i++) {
        chi.set_data(pDoc->GetD0(), *ps, d);
        chi.calc_chi();
        p = chi.get_p();
        for (int j = pDoc->Get_p_partition() - 1; j >= int(p * pDoc->Get_p_partition()); --j) {
            ++p_array[j];
        }
    }

    for (int i = 0; i < pDoc->Get_p_partition(); ++i) {
        p_array[i] /= pDoc->Get_N_p_values();
       /* double sadfa = p_array[i];
        double aasffasf = 1;*/
    }
    Invalidate();
}
