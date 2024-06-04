#pragma once
#include "..\PoissonDistributionBackend\Chi.h"

// SettingsDialog dialog

class SettingsDialog : public CDialog
{
	DECLARE_DYNAMIC(SettingsDialog)

public:
	SettingsDialog(CWnd* pParent = nullptr);   // standard constructor
	SettingsDialog(Chi2Histortam& chi, Distribution& d0, Distribution& d1, PoissonSample* ps,
		int mod_meth, int N_p_values, int p_partition,
		double lambda_min, double lambda_max, int lambda_partition, double alpha, int N_p_value_lambda,
		CWnd* pParent = nullptr);
	virtual ~SettingsDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_sample_size;
	double m_lambda_h0;
	double m_lambda_h1;
	int m_mod_method;
	int m_n_iterations;
	int m_p_level_partition;
	double m_lambda_min;
	double m_lambda_max;
	int m_lambda_partition;
	double m_alpha;
	int m_N_p_value_lambda;
};
