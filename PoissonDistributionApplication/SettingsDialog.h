#pragma once
#include "..\PoissonDistributionBackend\Chi.h"

// SettingsDialog dialog

class SettingsDialog : public CDialog
{
	DECLARE_DYNAMIC(SettingsDialog)

public:
	SettingsDialog(CWnd* pParent = nullptr);   // standard constructor
	SettingsDialog(Chi2Histortam& chi, Distribution& d0, Distribution& d1, PoissonSample* ps,
		int mod_meth, int N_p_values, int p_partition, CWnd* pParent = nullptr);
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
};
