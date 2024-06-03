#include <iostream>
#include "Distribution.h"
#include "PoissonSample.h"
#include "Vector.h"
#include "Chi.h"

using namespace std;

void print_vec(double* arr, int n) {
	for (size_t i = 0; i < n; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

int main()
{
	int n_samples = 13333;
	mt19937 mt(100);

	Distribution d;
	int N;
	PoissonSampleInverse ps(mt, d, n_samples);
	
	double* sample_vec = ps.generate_sample(d, N);
	double* th_vec = d.get_th_prob_array(N);
	for (size_t i = 0; i < N; i++)
	{
		sample_vec[i] /= n_samples;
	}
	print_vec(sample_vec, N);

	
	print_vec(th_vec, N);

	delete[] th_vec;
	delete[] sample_vec;

	cout << "------" << endl;

	d.set_lambda(10);
	Chi2Histortam chi(d, ps);
	chi.calc_chi_parametres();
	cout << "p-level = " << chi.get_p() << endl;
	int distr_len = chi.get_distr_len();
	double* sample_freq = chi.get_sample_freq();
	double* th_freq = chi.get_th_freq();
	print_vec(sample_freq, distr_len);
	print_vec(th_freq, distr_len);



}

