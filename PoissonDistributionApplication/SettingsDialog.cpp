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
{

}

SettingsDialog::SettingsDialog(Chi2Histortam& chi, Distribution& d0, Distribution& d1, PoissonSample* ps,
	int mod_meth, int N_p_values, int p_partition, CWnd* pParent /*= nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_sample_size(ps->get_N())
	, m_lambda_h0(d0.get_lambda())
	, m_lambda_h1(d1.get_lambda())
	, m_mod_method(mod_meth)
	, m_p_level_partition(p_partition)
	, m_n_iterations(N_p_values)
{

}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sample_size);
	DDV_MinMaxInt(pDX, m_sample_size, 50, 214748364);
	DDX_Text(pDX, IDC_EDIT4, m_n_iterations);
	DDV_MinMaxInt(pDX, m_n_iterations, 1000, 214748364);
	DDX_Text(pDX, IDC_EDIT5, m_p_level_partition);
	DDV_MinMaxInt(pDX, m_p_level_partition, 10, 30);
	DDX_Text(pDX, IDC_EDIT2, m_lambda_h0);
	DDV_MinMaxDouble(pDX, m_lambda_h0, 0, 100);
	DDX_Text(pDX, IDC_EDIT3, m_lambda_h1);
	DDV_MinMaxDouble(pDX, m_lambda_h1, 0, 100);
	DDX_Radio(pDX, IDC_RADIO1, m_mod_method);
}


BEGIN_MESSAGE_MAP(SettingsDialog, CDialog)
END_MESSAGE_MAP()


// SettingsDialog message handlers
