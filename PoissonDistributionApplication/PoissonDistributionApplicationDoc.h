
// PoissonDistributionApplicationDoc.h : interface of the CPoissonDistributionApplicationDoc class
//


#pragma once
#include "..\PoissonDistributionBackend\Chi.h"

class CPoissonDistributionApplicationDoc : public CDocument
{
protected: // create from serialization only
	CPoissonDistributionApplicationDoc() noexcept : 
		rnd_gen(100),
		d0(5),
		d1(6),
		mod_meth(ModelingMethod::inverse),
		ps(new PoissonSampleInverse(rnd_gen, d0, 100)),
		chi(),
		p_array(nullptr),
		p_array_size(0)
	{};
	DECLARE_DYNCREATE(CPoissonDistributionApplicationDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CPoissonDistributionApplicationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	
	// ��������� ��������� ��� ���� ���������
	mt19937 rnd_gen;

	// �������� ���� ���������
	Distribution d0;
	Distribution d1;

	PoissonSample* ps;

	Chi2Histortam chi;


	// ���� ��� �������
	// ����� � ���� ��� ������ �������������
	enum class ModelingMethod
	{
		inverse = 0,
		random_variables = 1,
		inverse_table = 2
	};
	ModelingMethod mod_meth;

	// ���� ��� ������������� p-levels � power
	int N_p_values = 10000;
	int p_partition = 20;
	double* p_array = nullptr;
	int p_array_size = 0;

	// ���� ��� ������� ����������� ������ ������� ���� �� ���������� lambda
	double lambda_min = 1;
	double lambda_max = 10;
	int lambda_partition = 20;
	double alpha = 0.05;
	int N_p_value_lambda = 1000;


public:

	// �������
	double Get_lambda_min() { return lambda_min; }

	double Get_lambda_max() { return lambda_max; }

	int Get_lambda_partition() { return lambda_partition; }

	double Get_alpha() { return alpha; }

	int Get_N_cycles_by_lambda() { return N_p_value_lambda; }

	double*& Get_p_array() {
		return p_array;
	}

	int Get_p_partition() {
		return p_partition;
	}

	int Get_N_p_values() {
		return N_p_values;
	}
	Distribution& GetD0() {
		return d0;
	}
	Distribution& GetD1() {
		return d1;
	}

	PoissonSample*& GetPs() {
		return ps;
	}
	Chi2Histortam& getChi() {
		return chi;
	}
	ModelingMethod getMod_meth() {
		return mod_meth;
	}


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnSettings();
};
