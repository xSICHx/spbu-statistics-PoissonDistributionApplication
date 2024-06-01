#include <iostream>
#include "Distribution.h"
#include "PoissonSample.h"
#include "Vector.h"
#include <vector>

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
	int n = 13333;
	mt19937 mt(100);

	Distribution d;
	int N;
	PoissonSampleInverse ps(mt, d, n);
	ps.generate_sample(d);
	double* sample_vec = ps.get_sample_freq(N);
	double* th_vec = d.get_th_prob_array(N);
	for (size_t i = 0; i < N; i++)
	{
		sample_vec[i] /= n;
	}
	print_vec(sample_vec, N);

	
	print_vec(th_vec, N);

	cout << "------" << endl;

	delete[] th_vec;
	delete[] sample_vec;

}

