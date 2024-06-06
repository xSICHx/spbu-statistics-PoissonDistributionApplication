#pragma once
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
	mt19937 mt(100); // Random number generator
	
	Distribution d(5); // Poisson distribution with lambda = 5
	int n_samples = 10000; // Number of samples
	PoissonSampleInverse ps(mt, d, n_samples); // Inverse function sampling generator
	
	int distr_len; // The length of the distribution, will be calculated when the sample is generated
	// Distribution generation and calculation of the length of grouped frequencies
	double* sample_vec = ps.generate_sample(d, distr_len); 
	double* th_vec = d.get_th_prob_array(distr_len); // Obtaining the vector of theoretical probabilities
	for (size_t i = 0; i < distr_len; i++) // Taking sample to probabilities
	{
		sample_vec[i] /= n_samples;
	}
	print_vec(sample_vec, distr_len); // Displaying sample values on the screen
	
	print_vec(th_vec, distr_len); // Displaying theoretical values on the screen 

	delete[] th_vec;
	delete[] sample_vec;

	cout << "------" << endl;

	d.set_lambda(10); // Substitution of lambda in the theoretical distribution
	Chi2Histortam chi(d, ps); // Initialization of the class for Chi^2 criterion calculation
	// Criterion Application. Groups the sample under the conditions of criterion applicability, 
	// counts degrees of freedom, criterion value, p-level
	chi.apply_chi_criterion(); 

	cout << "p-level = " << chi.get_p() << endl;
	distr_len = chi.get_distr_len(); // Obtaining the length of the distribution for which the criterion was applied
	double* sample_freq = chi.get_sample_freq(); // Obtaining a sample for which the criterion was applied
	double* th_freq = chi.get_th_freq(); // Obtaining the frequencies for which the criterion was applied
	print_vec(sample_freq, distr_len); // Sample output
	print_vec(th_freq, distr_len); // Derivation of theoretical values 
}

/*! \mainpage Classes for generating the Poisson distribution and calculating the chi square criterion for hypothesis testing
* Example of using PoissonDistribution 
* @code
#pragma once
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
	mt19937 mt(100); // Random number generator

	Distribution d(5); // Poisson distribution with lambda = 5
	int n_samples = 10000; // Number of samples
	PoissonSampleInverse ps(mt, d, n_samples); // Inverse function sampling generator

	int distr_len; // The length of the distribution, will be calculated when the sample is generated
	// Distribution generation and calculation of the length of grouped frequencies
	double* sample_vec = ps.generate_sample(d, distr_len);
	double* th_vec = d.get_th_prob_array(distr_len); // Obtaining the vector of theoretical probabilities
	for (size_t i = 0; i < distr_len; i++) // Taking sample to probabilities
	{
		sample_vec[i] /= n_samples;
	}
	print_vec(sample_vec, distr_len); // Displaying sample values on the screen

	print_vec(th_vec, distr_len); // Displaying theoretical values on the screen

	delete[] th_vec;
	delete[] sample_vec;

	cout << "------" << endl;

	d.set_lambda(10); // Substitution of lambda in the theoretical distribution
	Chi2Histortam chi(d, ps); // Initialization of the class for Chi^2 criterion calculation
	// Criterion Application. Groups the sample under the conditions of criterion applicability,
	// counts degrees of freedom, criterion value, p-level
	chi.apply_chi_criterion();

	cout << "p-level = " << chi.get_p() << endl;
	distr_len = chi.get_distr_len(); // Obtaining the length of the distribution for which the criterion was applied
	double* sample_freq = chi.get_sample_freq(); // Obtaining a sample for which the criterion was applied
	double* th_freq = chi.get_th_freq(); // Obtaining the frequencies for which the criterion was applied
	print_vec(sample_freq, distr_len); // Sample output
	print_vec(th_freq, distr_len); // Derivation of theoretical values
}
* @endcode
* 
* 
* Example of using Distribution class
* @code
Distribution d(5); // Poisson distribution with lambda = 5
double* th_prob = d.get_th_prob_array(20); // Obtaining the vector of theoretical probabilities
d.set_lambda(10); // Substitution of lambda in the theoretical distribution
double th_20 = d.get_th_prob_element(20); // Get 20-th element of theoretical distribution

delete[] th_prob;
* @endcode
* 
* 
* Example of using PoissonSample classes
* @code
mt19937 mt(100); // Random number generator
Distribution d(5); // Poisson distribution with lambda = 5
int n_samples = 100;
PoissonSample* ps = new PoissonSampleInverse(mt, d, n_samples); // Pointer on sample generator by inverse function method
ps->simulate(d); // Generate one element from Poisson distribution
int distr_len; // The length of the distribution, will be calculated when the sample is generated
// Distribution generation and calculation of the length of grouped frequencies
double* sample_vec = ps->generate_sample(d, distr_len);
delete[] sample_vec;
delete ps;

ps = new PoissonSampleInverseTable(mt, d, n_samples); // Pointer on sample generator by table inverse function method
delete ps;
* @endcode
* 
* 
* Example of using Chi2Histortam class
* @code
mt19937 mt(100); // Random number generator
Distribution d(5); // Poisson distribution with lambda = 5
PoissonSampleRandomVariables ps(mt, d, 100);
Chi2Histortam chi(d, ps); // Initialization of the class for Chi^2 criterion calculation
// Criterion Application. Groups the sample under the conditions of criterion applicability,
// counts degrees of freedom, criterion value, p-level
chi.apply_chi_criterion();

cout << "p-level = " << chi.get_p() << endl;
int distr_len = chi.get_distr_len(); // Obtaining the length of the distribution for which the criterion was applied
double* sample_freq = chi.get_sample_freq(); // Obtaining a sample for which the criterion was applied
double* th_freq = chi.get_th_freq(); // Obtaining the frequencies for which the criterion was applied
print_vec(sample_freq, distr_len); // Sample output
print_vec(th_freq, distr_len); // Derivation of theoretical values
* @endcode
*/



