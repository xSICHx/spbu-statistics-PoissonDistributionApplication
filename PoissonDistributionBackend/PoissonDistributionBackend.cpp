#pragma once
#include <iostream>
#include "Distribution.h"
#include "PoissonSample.h"
#include "Chi.h"

using namespace std;

// Function for output double*
void print_vec(const double* arr, int n) {
	for (size_t i = 0; i < n; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}


int main()
{
	cout << "How to use Distribution class: " << endl;
	Distribution d1(6); // Poisson distribution with lambda = 5
	const double* th_prob = d1.get_th_prob_array(20); // Obtaining the vector of theoretical probabilities
	cout << "Theoretical probabilities with lambda = 6:" << endl;
	print_vec(th_prob, 20);
	d1.set_lambda(11); // Substitution of lambda in the theoretical distribution
	double th_15 = d1.get_th_prob_element(15); // Get 15-th element of theoretical distribution
	cout << "15-th element of theoretical distribution with lambda = 11" << endl;


	cout << "\n\nHow to use PoissonSample classes:" << endl;
	mt19937 mt3(100); // Random number generator
	Distribution d3(5); // Poisson distribution with lambda = 5
	int n_samples3 = 100;
	PoissonSample* ps3 = new PoissonSampleInverse(mt3, d3, n_samples3); // Pointer on sample generator by inverse function method
	 // Generate one element from Poisson distribution
	cout << "One element from Poisson distribution simulated by inverse function method: " << ps3->simulate(d3) << endl;
	delete ps3;

	ps3 = new PoissonSampleRandomVariables(mt3, d3, n_samples3); // Pointer on sample generator by table inverse function method
	int distr_len3; // The length of the distribution, will be calculated when the sample is generated
	// Distribution generation and calculation of the length of grouped frequencies
	ps3->generate_sample(d3);
	distr_len3 = ps3->get_sample_freq_len();
	const double* sample_vec3 = ps3->get_sample_freq();
	cout << "Sample frequencies with lambda = 5 generated by table inverse function method:" << endl;
	print_vec(sample_vec3, distr_len3);
	delete ps3;

	cout << "\n\nHow to use Chi2Histogram class:" << endl;
	mt19937 mt(100); // Random number generator

	Distribution d(5); // Poisson distribution with lambda = 5
	int n_samples = 10000; // Number of samples
	PoissonSampleInverse ps(mt, d, n_samples); // Inverse function sampling generator

	int distr_len; // The length of the distribution, will be calculated when the sample is generated
	// Distribution generation and calculation of the length of grouped frequencies
	ps.generate_sample(d); // Generates and stores sample grouped frequencies
	const double* sample_vec = ps.get_sample_freq(); // Get sample grouped frequencies
	distr_len = ps.get_sample_freq_len(); // Get length of sample grouped frequencies
	const double* th_vec = d.get_th_prob_array(distr_len); // Obtaining the vector of theoretical probabilities
	cout << "Sample frequencies with lambda = 5:" << endl;
	print_vec(sample_vec, distr_len); // Displaying sample frequencies on the screen
	cout << "Theoretical probabilities with lambda = 5:" << endl;
	print_vec(th_vec, distr_len); // Displaying theoretical probabilities on the screen 

	d.set_lambda(10); // Substitution of lambda in the theoretical distribution
	Chi2Histortam chi(d, ps); // Initialization of the class for Chi^2 criterion calculation
	// Criterion Application. Groups the sample under the conditions of criterion applicability,
	// counts degrees of freedom, criterion value, p-level
	chi.apply_chi_criterion();

	distr_len = chi.get_distr_len(); // Obtaining the length of the distribution for which the criterion was applied
	const double* sample_freq = chi.get_sample_freq(); // Obtaining a sample for which the criterion was applied
	const double* th_freq = chi.get_th_freq(); // Obtaining the frequencies for which the criterion was applied
	cout << "Sample frequencies with lambda = 10:" << endl;
	print_vec(sample_freq, distr_len); // Sample output
	cout << "Theoretical frequencies with lambda = 10:" << endl;
	print_vec(th_freq, distr_len); // Derivation of theoretical values
	cout << "p-level = " << chi.get_p() << endl;
	cout << "Chi-value = " << chi.get_chi() << endl;
	cout << "d.f = " << chi.get_df() << endl;
}

/*! \mainpage Classes for generating the Poisson distribution and calculating the chi square criterion for hypothesis testing
* Example of using PoissonDistribution project
* @code
#include <iostream>
#include "Distribution.h"
#include "PoissonSample.h"
#include "Chi.h"

using namespace std;

// Function for output double*
void print_vec(double* arr, int n) {
	for (size_t i = 0; i < n; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}


int main()
{
	cout << "How to use Distribution class: " << endl;
	Distribution d1(6); // Poisson distribution with lambda = 5
	const double* th_prob = d1.get_th_prob_array(20); // Obtaining the vector of theoretical probabilities
	cout << "Theoretical probabilities with lambda = 6:" << endl;
	print_vec(th_prob, 20);
	d1.set_lambda(11); // Substitution of lambda in the theoretical distribution
	double th_15 = d1.get_th_prob_element(15); // Get 15-th element of theoretical distribution
	cout << "15-th element of theoretical distribution with lambda = 11" << endl;


	cout << "\n\nHow to use PoissonSample classes:" << endl;
	mt19937 mt3(100); // Random number generator
	Distribution d3(5); // Poisson distribution with lambda = 5
	int n_samples3 = 100;
	PoissonSample* ps3 = new PoissonSampleInverse(mt3, d3, n_samples3); // Pointer on sample generator by inverse function method
	 // Generate one element from Poisson distribution
	cout << "One element from Poisson distribution simulated by inverse function method: " << ps3->simulate(d3) << endl;
	delete ps3;

	ps3 = new PoissonSampleRandomVariables(mt3, d3, n_samples3); // Pointer on sample generator by table inverse function method
	int distr_len3; // The length of the distribution, will be calculated when the sample is generated
	// Distribution generation and calculation of the length of grouped frequencies
	ps3->generate_sample(d3);
	distr_len3 = ps3->get_sample_freq_len();
	const double* sample_vec3 = ps3->get_sample_freq();
	cout << "Sample frequencies with lambda = 5 generated by table inverse function method:" << endl;
	print_vec(sample_vec3, distr_len3);
	delete ps3;

	cout << "\n\nHow to use Chi2Histogram class:" << endl;
	mt19937 mt(100); // Random number generator

	Distribution d(5); // Poisson distribution with lambda = 5
	int n_samples = 10000; // Number of samples
	PoissonSampleInverse ps(mt, d, n_samples); // Inverse function sampling generator

	int distr_len; // The length of the distribution, will be calculated when the sample is generated
	// Distribution generation and calculation of the length of grouped frequencies
	ps.generate_sample(d); // Generates and stores sample grouped frequencies
	const double* sample_vec = ps.get_sample_freq(); // Get sample grouped frequencies
	distr_len = ps.get_sample_freq_len(); // Get length of sample grouped frequencies
	const double* th_vec = d.get_th_prob_array(distr_len); // Obtaining the vector of theoretical probabilities
	cout << "Sample frequencies with lambda = 5:" << endl;
	print_vec(sample_vec, distr_len); // Displaying sample frequencies on the screen
	cout << "Theoretical probabilities with lambda = 5:" << endl;
	print_vec(th_vec, distr_len); // Displaying theoretical probabilities on the screen 

	d.set_lambda(10); // Substitution of lambda in the theoretical distribution
	Chi2Histortam chi(d, ps); // Initialization of the class for Chi^2 criterion calculation
	// Criterion Application. Groups the sample under the conditions of criterion applicability,
	// counts degrees of freedom, criterion value, p-level
	chi.apply_chi_criterion();

	distr_len = chi.get_distr_len(); // Obtaining the length of the distribution for which the criterion was applied
	const double* sample_freq = chi.get_sample_freq(); // Obtaining a sample for which the criterion was applied
	const double* th_freq = chi.get_th_freq(); // Obtaining the frequencies for which the criterion was applied
	cout << "Sample frequencies with lambda = 10:" << endl;
	print_vec(sample_freq, distr_len); // Sample output
	cout << "Theoretical frequencies with lambda = 10:" << endl;
	print_vec(th_freq, distr_len); // Derivation of theoretical values
	cout << "p-level = " << chi.get_p() << endl;
	cout << "Chi-value = " << chi.get_chi() << endl;
	cout << "d.f = " << chi.get_df() << endl;
}
* @endcode
*/



