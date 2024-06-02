
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
		chi(d0, *ps),
		p_array(nullptr)
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
	enum class ModelingMethod
	{
		inverse = 0,
		random_variables = 1,
		inverse_table = 2
	};
	ModelingMethod mod_meth;

	mt19937 rnd_gen;

	Distribution d0;
	Distribution d1;

	PoissonSample* ps;

	Chi2Histortam chi;

	int N_p_values = 10000;
	int p_partition = 20;
	double* p_array;


public:

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
