
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
    ON_COMMAND(ID_P32775, &CPoissonDistributionApplicationView::OnP32775)
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

// Функция для отрисовки гистограммы 
void DrawHistogram(CDC* pDC, double* th_values, double* sample_values, double* indexes, int distr_len, CRect rect)
{
    // Определим границы для осей x и y
    double min_y = 0.0;
    double max_y = max(findMax(th_values, distr_len), findMax(sample_values, distr_len));
    //double max_y = 1;


    // Определим размеры области рисования
    int x_axis_length = rect.Width() - 200; // Длина оси x
    int y_axis_length = rect.Height() - 200; // Длина оси y




    // Отрисовка осей
    pDC->MoveTo(rect.left + 50, rect.top + y_axis_length + 50); // Начало оси x
    pDC->LineTo(rect.left + x_axis_length + 50, rect.top + y_axis_length + 50); // Конец оси x
    pDC->MoveTo(rect.left + 50, rect.top + 50); // Начало оси y
    pDC->LineTo(rect.left + 50, rect.top + y_axis_length + 50); // Конец оси y


    // Отрисовка значений на осях
    CString min_y_str, max_y_str;
    min_y_str.Format(L"%.2f", min_y);
    max_y_str.Format(L"%.2f", max_y);
    pDC->TextOutW(rect.left + 10, rect.top + y_axis_length + 40, min_y_str); // Минимальное значение на оси y
    pDC->TextOutW(rect.left + 10, rect.top + 30, max_y_str); // Максимальное значение на оси y




    // Отрисовка столбцов для th_values
    double full_bar_width = (x_axis_length) / distr_len;
    double bar_width = full_bar_width * 0.8;
    double shift = full_bar_width * 0.2;
    for (size_t i = 0; i < distr_len; ++i)
    {
        int bar_height = static_cast<int>((th_values[i] - min_y) / (max_y - min_y) * y_axis_length);


        CRect bar_rect(rect.left + 50 + static_cast<int>(i * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50 - bar_height,
            rect.left + 50 + static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50);
        pDC->FillSolidRect(&bar_rect, RGB(255, 192, 203)); // Розовый цвет для th_values
    }


    bar_width = full_bar_width * 0.6;
    shift = full_bar_width * 0.4;
    // Отрисовка столбцов для sample_values
    for (size_t i = 0; i < distr_len; ++i)
    {
        int bar_height = static_cast<int>((sample_values[i] - min_y) / (max_y - min_y) * y_axis_length);
        CRect bar_rect(rect.left + 50 + static_cast<int>(i * bar_width) + static_cast<int>(i * shift) + static_cast<int>(shift * 0.25),
            rect.top + y_axis_length + 50 - bar_height,
            rect.left + 50 + static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift) + static_cast<int>(shift * 0.25),
            rect.top + y_axis_length + 50);
        pDC->FillSolidRect(&bar_rect, RGB(0, 100, 0)); // Тёмно-зелёный цвет для sample_values
    }


    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkColor(RGB(255, 255, 255));


    // Отрисовка значений на оси x
    for (size_t i = 0; i < distr_len; ++i)
    {
        CString index_str;
        if (indexes[i] == (int)indexes[i])
            index_str.Format(L"%.0f", indexes[i]);
        else
            index_str.Format(L"%.3f", indexes[i]);
        pDC->TextOutW(rect.left + 50 + static_cast<int>(i * bar_width) + static_cast<int>(i * shift) +
            static_cast<int>(shift * 0.25),
            rect.top + y_axis_length + 60, index_str); // Значения на оси x
    }

    // Подписи осей
    pDC->TextOutW(rect.left + x_axis_length / 2, rect.top + y_axis_length + 80, L"Index");
    pDC->TextOutW(rect.left + 10, rect.top + 30 - 20, L"Value");
}

// Функция для отрисовки графика
void DrawLines(CDC* pDC, double* th_values, double* sample_values, int distr_len, CRect rect) {
    // Определим границы для осей x и y
    double min_y = 0.0;
    double max_y;
    if (th_values != nullptr)
        max_y = max(findMax(th_values, distr_len), findMax(sample_values, distr_len));
    else
        max_y = findMax(sample_values, distr_len);
    //double max_y = 1;


    // Определим размеры области рисования
    int x_axis_length = rect.Width() - 200; // Длина оси x
    int y_axis_length = rect.Height() - 200; // Длина оси y




    // Отрисовка осей
    pDC->MoveTo(rect.left + 50, rect.top + y_axis_length + 50); // Начало оси x
    pDC->LineTo(rect.left + x_axis_length + 50, rect.top + y_axis_length + 50); // Конец оси x
    pDC->MoveTo(rect.left + 50, rect.top + 50); // Начало оси y
    pDC->LineTo(rect.left + 50, rect.top + y_axis_length + 50); // Конец оси y


    // Отрисовка значений на осях
    CString min_y_str, max_y_str;
    min_y_str.Format(L"%.2f", min_y);
    max_y_str.Format(L"%.2f", max_y);
    pDC->TextOutW(rect.left + 10, rect.top + y_axis_length + 40, min_y_str); // Минимальное значение на оси y
    pDC->TextOutW(rect.left + 10, rect.top + 30, max_y_str); // Максимальное значение на оси y




    // Отрисовка столбцов для th_values, если есть теоретические значения
    double full_bar_width = (x_axis_length) / distr_len;
    double bar_width = full_bar_width * 0.8;
    double shift = full_bar_width * 0.2;
    int left_bottom_prev = 0;
    if (th_values != nullptr) {
        for (size_t i = 0; i < distr_len; ++i)
        {
            int bar_height_prev;
            if (i == 0)
                bar_height_prev = 0;
            else
                bar_height_prev = static_cast<int>((th_values[i - 1] - min_y) / (max_y - min_y) * y_axis_length);
            int bar_height = static_cast<int>((th_values[i] - min_y) / (max_y - min_y) * y_axis_length);

            CPen pen(PS_SOLID, 2, RGB(255, 192, 203));
            CPen* pOldPen = pDC->SelectObject(&pen);

            pDC->MoveTo(rect.left + 50 + left_bottom_prev, rect.top + y_axis_length + 50 - bar_height_prev);
            pDC->LineTo(rect.left + 50 + static_cast<int>((i + 1) * bar_width) +
                static_cast<int>(i * shift), rect.top + y_axis_length + 50 - bar_height);

            left_bottom_prev = ((int)((i + 1) * bar_width)) + ((int)(i * shift));
            pDC->SelectObject(pOldPen);
            //CRect bar_rect(rect.left + 50 + static_cast<int>(i * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50 - bar_height,
            //    rect.left + 50 + static_cast<int>((i + 1) * bar_width) + static_cast<int>(i * shift), rect.top + y_axis_length + 50);
            //pDC->FillSolidRect(&bar_rect, RGB(255, 192, 203)); // Розовый цвет для th_values
        }
    }


    left_bottom_prev = 0;
    // Отрисовка столбцов для sample_values
    for (size_t i = th_values == nullptr; i < distr_len; ++i)
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
        left_bottom_prev = ((int) ((i + 1) * bar_width)) + ((int) (i * shift));

        pDC->SelectObject(pOldPen);
    }


    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkColor(RGB(255, 255, 255));


    // Отрисовка значений на оси x

    //CString index_str;
    //index_str.Format(L"%.0f", 1.0);
    //pDC->TextOutW(rect.left + 50 + static_cast<int>(distr_len * bar_width) + static_cast<int>(distr_len * shift) +
    //    static_cast<int>(shift * 0.25),
    //    rect.top + y_axis_length + 60, index_str);



    //// Подписи осей
    //pDC->TextOutW(rect.left + x_axis_length / 2, rect.top + y_axis_length + 80, L"Index");
    //pDC->TextOutW(rect.left + 10, rect.top + 30 - 20, L"Value");
}


// Отрисовка распределения выборки
void DrawSampleHist(CDC* pDC, CPoissonDistributionApplicationDoc* pDoc, CRect rect) {
    
    Chi2Histortam& chi = pDoc->getChi();
    PoissonSample* ps = pDoc->GetPs();
    int distr_len = chi.get_distr_len();
    double* th_freq = chi.get_th_freq();
    double* sample_freq = chi.get_sample_freq();
    double* indexes = new double[distr_len];
    for (int i = 0; i < distr_len; i++)
        indexes[i] = i;
    DrawHistogram(pDC, th_freq, sample_freq, indexes, distr_len, rect);

    int x_axis_length = rect.Width() - 200; // Длина оси x
    int y_axis_length = rect.Height() - 200; // Длина оси y

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



    // Подписи
    int rectWidth = 10;
    int rectHeight = 10;
    int rectX = rect.Width() - rectWidth - 200; // 10 - отступ от правой границы
    int rectY = 10; // 10 - отступ от верхней границы
    // Создаем кисть с розовым цветом (RGB(255, 192, 203))
    CBrush pinkBrush(RGB(255, 192, 203));
    // Сохраняем текущую кисть
    CBrush* pOldBrush = pDC->SelectObject(&pinkBrush);
    // Рисуем розовый прямоугольник
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // Восстанавливаем предыдущую кисть
    pDC->SelectObject(pOldBrush);
    // Выводим текст
    CString text = L"Theoretical";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    // Подписи
    rectWidth = 10;
    rectHeight = 10;
    rectX = rect.Width() - rectWidth - 200; // 10 - отступ от правой границы
    rectY = 40; // 10 - отступ от верхней границы
    // Создаем кисть с розовым цветом (RGB(255, 192, 203))
    CBrush greenBrush(RGB(0, 100, 0));
    // Сохраняем текущую кисть
    pOldBrush = pDC->SelectObject(&greenBrush);
    // Рисуем розовый прямоугольник
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // Восстанавливаем предыдущую кисть
    pDC->SelectObject(pOldBrush);
    // Выводим текст
    text = L"Sample";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    delete[] indexes;
}

// Отрисовка графика распределения p-levels
void DrawPLines(CDC* pDC, CPoissonDistributionApplicationDoc* pDoc, CRect rect) {
    double* uniform = new double[pDoc->Get_p_partition()];
    Chi2Histortam& chi = pDoc->getChi();
    PoissonSample* ps = pDoc->GetPs();
    for (size_t i = 1; i <= pDoc->Get_p_partition(); i++)
    {
        uniform[i - 1] = (double)i / pDoc->Get_p_partition();
    }
    DrawLines(pDC, uniform, pDoc->Get_p_array(), pDoc->Get_p_partition(), rect);


    int x_axis_length = rect.Width() - 200; // Длина оси x
    int y_axis_length = rect.Height() - 200; // Длина оси y

    std::wstring result =
        L"Sample count = " + std::to_wstring(pDoc->Get_N_p_values()) +
        L"; Sample size = " + std::to_wstring(ps->get_N()) +
        L"; H0: lambda = " + std::to_wstring(pDoc->GetD0().get_lambda()) +
        L"; Method: " + ((int)pDoc->getMod_meth() == 0 ? L"inverse." :
            ((int)pDoc->getMod_meth() == 1 ? L"random variables." : L"inverse table"));
    CString res(result.c_str());
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 120, L"Distribution of p-levels.");
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 160, res);



    // Подписи
    int rectWidth = 10;
    int rectHeight = 10;
    int rectX = rect.Width() - rectWidth - 200; // 10 - отступ от правой границы
    int rectY = 10; // 10 - отступ от верхней границы
    // Создаем кисть с розовым цветом (RGB(255, 192, 203))
    CBrush pinkBrush(RGB(255, 192, 203));
    // Сохраняем текущую кисть
    CBrush* pOldBrush = pDC->SelectObject(&pinkBrush);
    // Рисуем розовый прямоугольник
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // Восстанавливаем предыдущую кисть
    pDC->SelectObject(pOldBrush);
    // Выводим текст
    CString text = L"Uniform";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    // Подписи
    rectWidth = 10;
    rectHeight = 10;
    rectX = rect.Width() - rectWidth - 200; // 10 - отступ от правой границы
    rectY = 30; // 10 - отступ от верхней границы
    // Создаем кисть с розовым цветом (RGB(255, 192, 203))
    CBrush greenBrush(RGB(0, 100, 0));
    // Сохраняем текущую кисть
    pOldBrush = pDC->SelectObject(&greenBrush);
    // Рисуем розовый прямоугольник
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // Восстанавливаем предыдущую кисть
    pDC->SelectObject(pOldBrush);
    // Выводим текст
    text = L"p-level";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    // Отрисовка значений на оси x
    int bar_width = x_axis_length * 0.8;
    int shift = x_axis_length * 0.2;

    CString index_str;
    index_str.Format(L"%.0f", 1.0);
    pDC->TextOutW(rect.left + 30 + bar_width + static_cast<int>(shift),
        rect.top + y_axis_length + 60, index_str);



    // Подписи осей
    pDC->TextOutW(rect.left + x_axis_length / 2, rect.top + y_axis_length + 80, L"Index");
    pDC->TextOutW(rect.left + 10, rect.top + 30 - 20, L"Value");


    delete[] uniform;
}

// Отрисовка графика мощности
void DrawBetaLines(CDC* pDC, CPoissonDistributionApplicationDoc* pDoc, CRect rect) {
    double* uniform = new double[pDoc->Get_p_partition()];
    Chi2Histortam& chi = pDoc->getChi();
    PoissonSample*& ps = pDoc->GetPs();
    for (size_t i = 1; i <= pDoc->Get_p_partition(); i++)
    {
        uniform[i - 1] = (double)i / pDoc->Get_p_partition();
    }
    DrawLines(pDC, uniform, pDoc->Get_p_array(), pDoc->Get_p_partition(), rect);


    int x_axis_length = rect.Width() - 200; // Длина оси x
    int y_axis_length = rect.Height() - 200; // Длина оси y

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



    // Подписи
    int rectWidth = 10;
    int rectHeight = 10;
    int rectX = rect.Width() - rectWidth - 200; // 10 - отступ от правой границы
    int rectY = 10; // 10 - отступ от верхней границы
    // Создаем кисть с розовым цветом (RGB(255, 192, 203))
    CBrush pinkBrush(RGB(255, 192, 203));
    // Сохраняем текущую кисть
    CBrush* pOldBrush = pDC->SelectObject(&pinkBrush);
    // Рисуем розовый прямоугольник
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // Восстанавливаем предыдущую кисть
    pDC->SelectObject(pOldBrush);
    // Выводим текст
    CString text = L"Uniform";
    pDC->TextOutW(rectX + 20, rectY - 5, text);

    // Подписи
    rectWidth = 10;
    rectHeight = 10;
    rectX = rect.Width() - rectWidth - 200; // 10 - отступ от правой границы
    rectY = 30; // 10 - отступ от верхней границы
    // Создаем кисть с розовым цветом (RGB(255, 192, 203))
    CBrush greenBrush(RGB(0, 100, 0));
    // Сохраняем текущую кисть
    pOldBrush = pDC->SelectObject(&greenBrush);
    // Рисуем розовый прямоугольник
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // Восстанавливаем предыдущую кисть
    pDC->SelectObject(pOldBrush);
    // Выводим текст
    text = L"power";
    pDC->TextOutW(rectX + 20, rectY - 5, text);


    // Отрисовка значений на оси x
    int bar_width = x_axis_length * 0.8;
    int shift = x_axis_length * 0.2;

    CString index_str;
    index_str.Format(L"%.0f", 1.0);
    pDC->TextOutW(rect.left + 30 + bar_width + static_cast<int>(shift),
        rect.top + y_axis_length + 60, index_str);



    // Подписи осей
    pDC->TextOutW(rect.left + x_axis_length / 2, rect.top + y_axis_length + 80, L"Index");
    pDC->TextOutW(rect.left + 10, rect.top + 30 - 20, L"Value");

    delete[] uniform;
}

// Отрисовка графика зависимости ошибки первого рода от параметра lambda
void DrawAlphaPLines(CDC* pDC, CPoissonDistributionApplicationDoc* pDoc, CRect rect) {
    Chi2Histortam& chi = pDoc->getChi();
    PoissonSample*& ps = pDoc->GetPs();
    DrawLines(pDC, nullptr, pDoc->Get_p_array(), pDoc->Get_lambda_partition(), rect);


    int x_axis_length = rect.Width() - 200; // Длина оси x
    int y_axis_length = rect.Height() - 200; // Длина оси y

    std::wstring result =
        L"Lambda min = " + std::to_wstring(pDoc->Get_lambda_min()) +
        L"; Lambda max = " + std::to_wstring(pDoc->Get_lambda_max()) +
        L"; Lambda partition = " + std::to_wstring(pDoc->Get_lambda_partition()) +
        L"; Alpha = " + std::to_wstring(pDoc->Get_alpha()) +
        L"; Number of p-values per lambda = " + std::to_wstring(pDoc->Get_N_cycles_by_lambda()) +
        L"; H0: lambda = " + std::to_wstring(pDoc->GetD0().get_lambda()) +
        L"; Method: " + ((int)pDoc->getMod_meth() == 0 ? L"inverse." :
            ((int)pDoc->getMod_meth() == 1 ? L"random variables." : L"inverse table"));
    CString res(result.c_str());
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 120, L"Number of H0 rejections depending on lambda.");
    pDC->TextOutW(rect.left + 20, rect.top + y_axis_length + 160, res);



    // Подписи
    int rectWidth = 10;
    int rectHeight = 10;
    int rectX = rect.Width() - rectWidth - 200; // 10 - отступ от правой границы
    int rectY = 10; // 10 - отступ от верхней границы
    
    // Подписи
    rectWidth = 10;
    rectHeight = 10;
    rectX = rect.Width() - rectWidth - 200; // 10 - отступ от правой границы
    rectY = 30; // 10 - отступ от верхней границы
    // Создаем кисть с розовым цветом (RGB(255, 192, 203))
    CBrush greenBrush(RGB(0, 100, 0));
    // Сохраняем текущую кисть
    CBrush* pOldBrush = pDC->SelectObject(&greenBrush);
    // Рисуем розовый прямоугольник
    pDC->Rectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);
    // Восстанавливаем предыдущую кисть
    pDC->SelectObject(pOldBrush);
    // Выводим текст
    CString text = L"Rejections";
    pDC->TextOutW(rectX + 20, rectY - 5, text);


    // Отрисовка значений на оси x
    int bar_width = x_axis_length * 0.8;
    int shift = x_axis_length * 0.2;

    // наибольшее значение lambda
    CString index_str;
    index_str.Format(L"%.2f", pDoc->Get_lambda_max());
    pDC->TextOutW(rect.left + 30 + bar_width + static_cast<int>(shift),
        rect.top + y_axis_length + 60, index_str);
    // наименьшее значение lambda
    index_str.Format(L"%.2f", pDoc->Get_lambda_min());
    pDC->TextOutW(rect.left + 30,
        rect.top + y_axis_length + 60, index_str);
    // значение lambda H0
    /*if (pDoc->Get_lambda_min() < pDoc->GetD0().get_lambda() && pDoc->GetD0().get_lambda() < pDoc->Get_lambda_max()) {
        double range = pDoc->Get_lambda_max() - pDoc->Get_lambda_min();
        double lambda_h0 = pDoc->GetD0().get_lambda();
        index_str.Format(L"%.2f", lambda_h0);
        pDC->TextOutW(rect.left + 30 + x_axis_length * ((lambda_h0 - pDoc->Get_lambda_min()) / range),
            rect.top + y_axis_length + 60, index_str);
    }*/


    // Подписи осей
    pDC->TextOutW(rect.left + x_axis_length / 2, rect.top + y_axis_length + 80, L"Lambda");
    pDC->TextOutW(rect.left + 10, rect.top + 30 - 20, L"Rejections");
}


void CPoissonDistributionApplicationView::OnDraw(CDC* pDC)
{
	CPoissonDistributionApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    // Ничего не рисуем
    if (draw_state == DrawState::nothing)
        return;

    // Случаи, когда рисуем
    CRect rect;
    GetClientRect(&rect);
	if (draw_state == DrawState::sample_distribution) // распределние выборки
		DrawSampleHist(pDC, pDoc, rect);
	else if (draw_state == DrawState::p_distribution) // распределение p-levels
		DrawPLines(pDC, pDoc, rect);
	else if (draw_state == DrawState::beta_distribution) // распределение мощности критерия
		DrawBetaLines(pDC, pDoc, rect);
    else if (draw_state == DrawState::alpha_lambda_dependance) // график зависимости ошибки первого рода от lambda
        DrawAlphaPLines(pDC, pDoc, rect);
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
	draw_state = DrawState::sample_distribution;
	CPoissonDistributionApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Distribution& d0 = pDoc->GetD0();
	PoissonSample* ps = pDoc->GetPs();
	Chi2Histortam& chi = pDoc->getChi();
	chi.set_data(d0, *ps);
	chi.calc_chi_parametres();
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
    for (int i = 0; i < pDoc->Get_p_partition(); i++) // инициализация массива частот
    {
        p_array[i] = 0;
    }

    double p = 0;
    for (int i = 0; i < pDoc->Get_N_p_values(); i++) {
        chi.set_data(d, *ps);
        chi.calc_chi_parametres();
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
    for (int i = 0; i < pDoc->Get_p_partition(); i++) // инициализация массива частот
    {
        p_array[i] = 0;
    }

    double p = 0;
    for (int i = 0; i < pDoc->Get_N_p_values(); i++) {
        chi.set_data(pDoc->GetD0(), *ps, d);
        chi.calc_chi_parametres();
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

// Распределение количества отвергнутых гипотез при заданных lambda
void CPoissonDistributionApplicationView::OnP32775()
{
    draw_state = DrawState::alpha_lambda_dependance;
    CPoissonDistributionApplicationDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    Distribution& d = pDoc->GetD0();
    PoissonSample* ps = pDoc->GetPs();
    Chi2Histortam& chi = pDoc->getChi();

    double*& p_array = pDoc->Get_p_array();
    delete[] p_array;
    
    p_array = new double[pDoc->Get_lambda_partition() + 1];
    for (int i = 0; i <= pDoc->Get_lambda_partition(); i++) // инициализация массива частот
    {
        p_array[i] = 0;
    }

    double alpha = pDoc->Get_alpha();
    /*int j = 0;
    double step = (pDoc->Get_lambda_max() - pDoc->Get_lambda_min()) / (double) pDoc->Get_lambda_partition();
    for (double curr_lambda = pDoc->Get_lambda_min(); curr_lambda <= pDoc->Get_lambda_max(); curr_lambda += step) {
        Distribution d_lambda(curr_lambda);
        for (int i = 0; i < pDoc->Get_N_cycles_by_lambda(); ++i) {
            chi.set_data(pDoc->GetD0(), *ps, d_lambda);
            chi.calc_chi_parametres();
            if (chi.get_p() < alpha)
                ++p_array[j];
        }
        ++j;
    }*/
    
    double step = (pDoc->Get_lambda_max() - pDoc->Get_lambda_min()) / (double)pDoc->Get_lambda_partition();
    for (int i = 0; i <= pDoc->Get_lambda_partition(); ++i) {
        double curr_lambda = pDoc->Get_lambda_min() + step * i;
        Distribution d_lambda(curr_lambda);
        for (int j = 0; j < pDoc->Get_N_cycles_by_lambda(); ++j) {
            chi.set_data(pDoc->GetD0(), *ps, d_lambda);
            chi.calc_chi_parametres();
            if (chi.get_p() < alpha)
                ++p_array[i];
        }


        double safdsdf = p_array[i];
        int agggsa = 1;
    }
    Invalidate();
}
