// SettingsDialog.cpp : implementation file
//

#include "pch.h"
#include "PoissonDistributionApplication.h"
#include "SettingsDialog.h"
#include "afxdialogex.h"


// SettingsDialog dialog

IMPLEMENT_DYNAMIC(SettingsDialog, CDialog)

SettingsDialog::SettingsDialog(CWnd* pParent /*= nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_sample_size(100)
	, m_lambda_h0(5)
	, m_lambda_h1(6)
	, m_mod_method(0)
	, m_p_level_partition(20)
	, m_n_iterations(10000)
	, m_lambda_min(1)
	, m_lambda_max(10)
	, m_lambda_partition(20)
	, m_alpha(0.05)
	, m_N_p_value_lambda(10000)

{

}

SettingsDialog::SettingsDialog(Chi2Histortam& chi, Distribution& d0, Distribution& d1, PoissonSample* ps,
	int mod_meth, int N_p_values, int p_partition,
	double lambda_min, double lambda_max, int lambda_partition, double alpha, int N_p_value_lambda,
	CWnd* pParent /*= nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_sample_size(ps->get_N())
	, m_lambda_h0(d0.get_lambda())
	, m_lambda_h1(d1.get_lambda())
	, m_mod_method(mod_meth)
	, m_p_level_partition(p_partition)
	, m_n_iterations(N_p_values)
	, m_lambda_min(lambda_min)
	, m_lambda_max(lambda_max)
	, m_lambda_partition(lambda_partition)
	, m_alpha(alpha)
	, m_N_p_value_lambda(N_p_value_lambda)
{

}

SettingsDialog::~SettingsDialog()
{
}

/*void SettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_sample_size);
	DDV_MinMaxInt(pDX, m_sample_size, 50, 214748364);
	

	DDX_Text(pDX, IDC_EDIT4, m_n_iterations);
	DDV_MinMaxInt(pDX, m_n_iterations, 1000, 214748364);

	DDX_Text(pDX, IDC_EDIT5, m_p_level_partition);
	DDV_MinMaxInt(pDX, m_p_level_partition, 10, 30);

	DDX_Text(pDX, IDC_EDIT2, m_lambda_h0);
	DDV_MinMaxDouble(pDX, m_lambda_h0, 0.01, 100);

	DDX_Text(pDX, IDC_EDIT3, m_lambda_h1);
	DDV_MinMaxDouble(pDX, m_lambda_h1, 0.01, 100);

	DDX_Radio(pDX, IDC_RADIO1, m_mod_method);

	DDX_Text(pDX, IDC_EDIT6, m_lambda_min);
	DDV_MinMaxDouble(pDX, m_lambda_min, 0.01, m_lambda_max-0.001);

	DDX_Text(pDX, IDC_EDIT7, m_lambda_max);
	DDV_MinMaxDouble(pDX, m_lambda_max, m_lambda_min+0.001, 40);

	DDX_Text(pDX, IDC_EDIT8, m_lambda_partition);
	DDV_MinMaxInt(pDX, m_lambda_partition, 10, 100);

	DDX_Text(pDX, IDC_EDIT9, m_alpha);
	DDV_MinMaxDouble(pDX, m_alpha, 0, 1);

	DDX_Text(pDX, IDC_EDIT10, m_N_p_value_lambda);
	DDV_MinMaxInt(pDX, m_N_p_value_lambda, 1000, 10000);
}*/


void SettingsDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    // Sample size
    int prev_m_sample_size = m_sample_size;
    DDX_Text(pDX, IDC_EDIT1, m_sample_size);
    if (m_sample_size < 50 || m_sample_size > 10000) {
        AfxMessageBox(L"The Sample size must be between 50 and 10000. This parameter was returned to its previous value.");
        m_sample_size = prev_m_sample_size;
    }

    // Number of iterations
    int prev_m_n_iterations = m_n_iterations;
    DDX_Text(pDX, IDC_EDIT4, m_n_iterations);
    if (m_n_iterations < 1000 || m_n_iterations > 214748364) {
        AfxMessageBox(L"The number of iterations must be between 1000 and 214748364. This parameter was returned to its previous value.");
        m_n_iterations = prev_m_n_iterations;
    }

    // P-level partition
    int prev_m_p_level_partition = m_p_level_partition;
    DDX_Text(pDX, IDC_EDIT5, m_p_level_partition);
    if (m_p_level_partition < 10 || m_p_level_partition > 30) {
        AfxMessageBox(L"The P-level partition must be between 10 and 30. This parameter was returned to its previous value.");
        m_p_level_partition = prev_m_p_level_partition;
    }

    // Lambda H0
    double prev_m_lambda_h0 = m_lambda_h0;
    DDX_Text(pDX, IDC_EDIT2, m_lambda_h0);
    if (m_lambda_h0 < 0.01 || m_lambda_h0 > 100) {
        AfxMessageBox(L"Lambda H0 must be between 0.01 and 100. This parameter was returned to its previous value.");
        m_lambda_h0 = prev_m_lambda_h0;
    }

    // Lambda H1
    double prev_m_lambda_h1 = m_lambda_h1;
    DDX_Text(pDX, IDC_EDIT3, m_lambda_h1);
    if (m_lambda_h1 < 0.01 || m_lambda_h1 > 100) {
        AfxMessageBox(L"Lambda H1 must be between 0.01 and 100. This parameter was returned to its previous value.");
        m_lambda_h1 = prev_m_lambda_h1;
    }

    // Mod method
    DDX_Radio(pDX, IDC_RADIO1, m_mod_method);

    // Lambda min
    double prev_m_lambda_min = m_lambda_min;
    DDX_Text(pDX, IDC_EDIT6, m_lambda_min);
    if (m_lambda_min < 0.01 || m_lambda_min >= m_lambda_max) {
        AfxMessageBox(L"Lambda min must be between 0.01 and Lambda max - 0.001. This parameter was returned to its previous value.");
        m_lambda_min = prev_m_lambda_min;
    }

    // Lambda max
    double prev_m_lambda_max = m_lambda_max;
    DDX_Text(pDX, IDC_EDIT7, m_lambda_max);
    if (m_lambda_max <= m_lambda_min || m_lambda_max > 40) {
        AfxMessageBox(L"Lambda max must be between Lambda min + 0.001 and 40. This parameter was returned to its previous value.");
        m_lambda_max = prev_m_lambda_max;
    }

    // Lambda partition
    int prev_m_lambda_partition = m_lambda_partition;
    DDX_Text(pDX, IDC_EDIT8, m_lambda_partition);
    if (m_lambda_partition < 10 || m_lambda_partition > 100) {
        AfxMessageBox(L"Lambda partition must be between 10 and 100. This parameter was returned to its previous value.");
        m_lambda_partition = prev_m_lambda_partition;
    }

    // Alpha
    double prev_m_alpha = m_alpha;
    DDX_Text(pDX, IDC_EDIT9, m_alpha);
    if (m_alpha < 0 || m_alpha > 1) {
        AfxMessageBox(L"Alpha must be between 0 and 1. This parameter was returned to its previous value.");
        m_alpha = prev_m_alpha;
    }

    // N p-value lambda
    int prev_m_N_p_value_lambda = m_N_p_value_lambda;
    DDX_Text(pDX, IDC_EDIT10, m_N_p_value_lambda);
    if (m_N_p_value_lambda < 1000 || m_N_p_value_lambda > 10000) {
        AfxMessageBox(L"N p-value lambda must be between 1000 and 10000. This parameter was returned to its previous value.");
        m_N_p_value_lambda = prev_m_N_p_value_lambda;
    }
}



BEGIN_MESSAGE_MAP(SettingsDialog, CDialog)
END_MESSAGE_MAP()


// SettingsDialog message handlers
