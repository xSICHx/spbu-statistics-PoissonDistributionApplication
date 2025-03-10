#pragma once
#include "PoissonSample.h"


/// <summary>
/// Chi-square class for calculating the chi-square criterion. Can group the sample for applicability of the criterion.
/// Stores sample and theoretical values as well as criterion values, p-value, degrees of freedom.
/// </summary>
class Chi2Histortam
{
private:
    int distr_len = 0;
    /// <summary>
    /// Sample values
    /// </summary>
    double* sample_freq = nullptr;
    /// <summary>
    /// Theoretical values
    /// </summary>
    double* th_freq = nullptr;


    double p = 0;
    double chi = 0;
    int df = 0;


    /// <summary>
    /// Creates grouped data (satisfy chi-square conditions).
    /// </summary>
    /// <param name="th_chied">Empty array for theoretical frequencies, to be filled in later</param>
    /// <param name="sample_chied">Empty array for sampling frequencies to be filled in later</param>
    /// <param name="chied_len">The length of the frequencies, will be subsequently calculated</param>
    void create_chied_values(double*& th_chied, double*& sample_chied, int& chied_len) const {
        // �������� ������ � ������
        double tmp_th = 0, tmp_sample = 0;
        int i = 0;
        while (i < distr_len && (tmp_th < 5 || th_freq[i] < 5)) {
            tmp_th += th_freq[i];
            tmp_sample += sample_freq[i];
            ++i;
        }
        /*th_chied_values.push_back(tmp_th);
        sample_chied_values.push_back(tmp_sample);*/

        // �������� ������ � �����
        double tmp_th2 = 0, tmp_sample2 = 0;
        int j = distr_len - 1;
        while (j >= 0 && (tmp_th2 < 5 || th_freq[j] < 5)) {
            tmp_th2 += th_freq[j];
            tmp_sample2 += sample_freq[j];
            --j;
        }

		if (j == -1) {
			chied_len = 1;
			th_chied = new double[chied_len];
			sample_chied = new double[chied_len];
			th_chied[0] = tmp_th2;
			sample_chied[0] = tmp_sample2;
			return;
		}
        // ��������� ������ ��� �������
        chied_len = (j - i + 1) + (1) + (1); 
        th_chied = new double[chied_len];
        sample_chied = new double[chied_len];

        // �������� ������ � ��������
        for (int k = i; k <= j; ++k) {
            th_chied[k - i + 1] = th_freq[k];
            sample_chied[k - i + 1] = sample_freq[k];
        }


        // ��������� ������ � �����
        th_chied[chied_len - 1] = tmp_th2;
        sample_chied[chied_len - 1] = tmp_sample2;

        // ��������� ������ � ������
        th_chied[0] = tmp_th;
        sample_chied[0] = tmp_sample;

		for (int i = 0; i < chied_len; ++i) {
			double sfdaad = th_chied[i];
			int afsdaf = 0;
		}
    }

    /// <summary>
    /// Counts chi square over theoretical and sample frequencies.
    /// </summary>
    /// <returns>Meaning of Chi square</returns>
    double calc_chi2_value(const double* th_chied, const double* sample_chied, const int chied_len ) const {
        double res = 0, diff;
        for (int i = 0; i < chied_len; ++i) {
            diff = sample_chied[i] - th_chied[i];
            res += diff * diff / (th_chied[i]);
        }
        return res;
    }

public:
    Chi2Histortam() = default;
	/// <summary>
	/// Creates and stores theoretical and sample frequencies from a single distribution  
	/// </summary>
	/// <param name="d"> Distribution </param>
	/// <param name="sample"> Sample </param>
	Chi2Histortam(Distribution& d, PoissonSample& sample) {
		sample_freq = sample.generate_sample(d, distr_len);
		th_freq = d.get_th_prob_array(distr_len); // ���� ��� �� �������, � �����������
		for (int i = 0; i < distr_len; ++i)
			th_freq[i] *= sample.get_N();

	};
	/// <summary>
	/// Creates and stores theoretical and sample frequencies from different distributions.
	/// </summary>
	/// <param name="d"> Theoretical distribution </param>
	/// <param name="sample"> Sample </param>
	/// <param name="d_sample"> Distribution for the sample </param>
    Chi2Histortam(Distribution& d, PoissonSample& sample, Distribution& sample_d) {
        sample_freq = sample.generate_sample(sample_d, distr_len, (int) (d.get_lambda() + d.get_lambda()*2) );
        th_freq = d.get_th_prob_array(distr_len); // ���� ��� �� �������, � �����������
        for (int i = 0; i < distr_len; ++i)
            th_freq[i] *= sample.get_N();

    };
    ~Chi2Histortam() {
        delete[] sample_freq;
        delete[] th_freq;
    };

	/// <summary>
	/// Creates and stores theoretical and sample frequencies from a single distribution.  
	/// </summary>
	/// <param name="d"> Distribution </param>
	/// <param name="sample"> Sample </param>
	void set_data(Distribution& d, PoissonSample& sample) {
		delete[] sample_freq;
		delete[] th_freq;

		sample_freq = sample.generate_sample(d, distr_len);

		th_freq = d.get_th_prob_array(distr_len); // ���� ��� �� �������, � �����������
		for (int i = 0; i < distr_len; ++i)
			th_freq[i] *= sample.get_N();
	}

	/// <summary>
	/// Creates and stores theoretical and sample frequencies from different distributions.
	/// </summary>
	/// <param name="d"> Theoretical distribution </param>
	/// <param name="sample"> Sample </param>
	/// <param name="d_sample"> Distribution for the sample </param>
    void set_data(Distribution& d, PoissonSample& sample, Distribution& d_sample) {
        delete[] sample_freq;
        delete[] th_freq;


        sample_freq = sample.generate_sample(d_sample, distr_len, (int)(d.get_lambda() + d.get_lambda() * 2));

        th_freq = d.get_th_prob_array(distr_len); // ���� ��� �� �������, � �����������

        for (int i = 0; i < distr_len; ++i)
            th_freq[i] *= sample.get_N();
    }

	/// <summary>
	/// Calculates the criterion, p-value, and degrees of freedom for the sample
	/// </summary>
    void apply_chi_criterion() {
        int chied_len;
        double* th_chied = nullptr; double* sample_chied = nullptr;

        create_chied_values(th_chied, sample_chied, chied_len);
        chi = calc_chi2_value(th_chied, sample_chied, chied_len);
        df = chied_len - 1;
        p = 1 - pChiSq(chi, df);




        delete[] th_chied;
        delete[] sample_chied;
    }


    double get_p() { return p; }
    double get_chi() { return chi; }
    int get_df() { return df; }

    int get_distr_len() { return distr_len; }
    double*& get_sample_freq() { return sample_freq; }
    double*& get_th_freq() { return th_freq; }


// ������� �� PROBDIST.H. �������� ��������� ���.
private:
	const double Eps = 1e-15;

	int fequal(double a, double b)
	{
		return  (fabs(a - b) < Eps) ? 1 : 0;
	}

	int fcompare(double a, double b)
	{
		double d = a - b;
		int res;

		if (fabs(d) < Eps)
			res = 0;
		else if (d < 0.0)
			res = -1;
		else
			res = 1;
		return res;
	}
	//
	// Normal distribution
	//

	void NORMAL(int IFLAG, double& X, double& PROB)
	{
		/*    ' Normal distribution subroutine

			' Input

			'  IFLAG = type of computation
			'      1 = given x, compute probability
			'      2 = given probability, compute x

			' Output (or input)

			'  X     = x value
			'  PROB  = probability; 0 < PROB < 1
		*/
		double x1;

		switch (IFLAG) {
		case 1:
			//  N(x)
			x1 = fabs(X);

			if (x1 > 7)
				PROB = 0;
			else {
				PROB = 1 + x1 * (0.049867347 + x1 * (0.0211410061 + x1 *
					(0.0032776263 + x1 * (0.0000380036 + x1 *
						(0.0000488906 + x1 * 0.000005383)))));
				PROB = 0.5 * pow(PROB, -16);
			}

			if (X < 0.0)  PROB = 1.0 - PROB;
			break;
		case 2:
			// N(p)
			if (fequal(PROB, 0.5)) {
				X = 0.0;
				return;
			}

			x1 = (PROB > 0.5) ? 1.0 - PROB : PROB;

			if (fequal(x1, 0.05))
				X = 1.64485;
			else if (fequal(x1, 0.025))
				X = 1.95996;
			else if (fequal(x1, 0.01))
				X = 2.32635;
			else if (fequal(x1, 0.005))
				X = 2.57583;
			else {
				x1 = -log(4.0 * x1 * (1.0 - x1));
				X = (-3.231081277E-09 * x1 + 8.360937017E-08) * x1 - 0.00000104527497;
				X = (X * x1 + 0.000005824238515) * x1 + 0.000006841218299;
				X = ((X * x1 - 0.0002250947176) * x1 - 0.000836435359) * x1 + 0.03706987906;
				X = X * x1 + 1.570796288;
				X = sqrt(x1 * X);
			}

			if (PROB > 0.5) X = -X;
		}

	}

	double pNormal(double x)
	{
		double prob;
		NORMAL(1, x, prob);
		return 1.0 - prob;
	}

	double xNormal(double prob)
	{
		double x, p = 1.0 - prob;
		NORMAL(2, x, p);
		return x;
	}

	//
	//  Chi-2 distribution
	//

	void CHI(int IFLAG, double N, double& X, double& PROB)
	{
		/*	' Chi-squared distribution subroutine

			' Input

			'  IFLAG = type of computation
			'      1 = given x, compute probability
			'      2 = given probability, compute x
			'  N     = degrees of freedom; N >= 1

			' Output (or input)

			'  X     = x value; X >=0
			'  PROB  = probability; 0 < PROB <= 1

			' NOTE: requires subroutine pNORMAL.BAS
		*/
		int i;
		switch (IFLAG) {
		case 1:
			//	Chi(x)

			double X1, X3;
			double QF, QP, QX0, QX1, QX2, QX3;
			int QPIndex, iN;

			if (fequal(X, 0.0)) {
				PROB = 1.0;
				return;
			}

			if (N > 40) {
				X3 = 2.0 / (9.0 * N);
				X1 = (pow(X / N, 0.3333333333) - 1.0 + X3) / sqrt(X3);
				NORMAL(1, X1, PROB);
				return;
			}

			iN = N;
			QPIndex = 2 - iN + 2 * (iN / 2);

			X3 = sqrt(X);

			if (QPIndex != 1) {
				PROB = exp(-X / 2);
				QF = PROB / 2;
			}
			else {
				NORMAL(1, X3, PROB);
				PROB = 2.0 * PROB;
				QF = 0.3989422804 * exp(-X / 2.0) / X3;
			}

			for (i = QPIndex; i < iN; i += 2) {
				QF = QF * X / i;
				PROB = PROB + 2.0 * QF;
			}
			return;
		case 2:
			//	Chi(p)

			if (N == 1) {
				X1 = PROB;
				X1 = X1 / 2;
				NORMAL(2, X, X1);
				X = X * X;
				return;
			}
			else if (N == 2) {
				X = -2 * log(PROB);
				return;
			}

			QX1 = 0;
			QX2 = 1;
			QX3 = 0.5;
			QP = PROB;

			do {
				X = 1.0 / QX3 - 1.0;

				CHI(1, N, X, PROB);

				if (PROB <= QP)
					QX1 = QX3;
				else
					QX2 = QX3;

				QX0 = QX3;
				QX3 = (QX1 + QX2) / 2;
			} while (fabs(QX3 - QX0) > (0.00001 * QX3));

			X = 1 / QX3 - 1;
			PROB = QP;
			return;
		}
	}

	double pChiSq(double x, int n)
	{
		double prob;
		CHI(1, (double)n, x, prob);
		return 1.0 - prob;
	}

	double xChi(double prob, int n)
	{
		double x, p = 1.0 - prob;
		CHI(2, (double)n, x, p);
		return x;
	}
};

