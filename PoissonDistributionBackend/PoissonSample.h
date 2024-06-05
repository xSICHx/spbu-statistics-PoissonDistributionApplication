#pragma once
#include <iostream>
#include <string>
#include <random>
#include "Vector.h"

#include "Distribution.h"


/// <summary>
/// Poisson distribution sample generation class
/// </summary>
class PoissonSample
{
protected:
    /// <summary>
    /// Random number generator 
    /// </summary>
    mt19937& gen;
    uniform_real_distribution<double> uniform_rand = uniform_real_distribution<double>(0.0, 1.0);
    /// <summary>
    /// Number of elements in the sample
    /// </summary>
    int N_samples;


public:
    PoissonSample() = default;
    /// <summary>
    /// Initialization of the Poisson distribution
    /// </summary>
    /// <param name="_gen">Random Number Generator</param>
    /// <param name="d">Theoretical distribution</param>
    /// <param name="_N_samples">Number of elements in the sample</param>
    PoissonSample(mt19937& _gen, Distribution& d, int _N_samples = 100) : gen(_gen), N_samples(_N_samples){}
    virtual ~PoissonSample() = default;

    /// <summary>
    /// Generate one sample item
    /// </summary>
    /// <param name="d">Theoretical distribution</param>
    /// <returns>Sample element</returns>
    virtual int simulate(Distribution& d) const = 0;
    
    /// <summary>
    /// Generates a whole sample
    /// </summary>
    /// <param name="d">Theoretical distribution</param>
    /// <param name="len_sample">Sample length</param>
    /// <param name="init_len_sample">Additional parameter.
    /// If you want a sample whose array consists of at least init_len_sample values.</param>.
    /// <returns>Sampling</returns>
    virtual double* generate_sample(Distribution& d, int& len_sample, ///< Сколько элементов будет в частотах массива >
        int init_len_sample = -1 // если нужно заранее задать размер, все элементы будут нулевые
    ) {
        if (N_samples < 50) {
            cerr << "Error: Sample size should be more 50" << endl;
        }

        Vector sample;
        if (init_len_sample > 0) {
            sample.resize(init_len_sample);
        }
            
            

        double lambda = d.get_lambda();
        for (int i = 0, curr_elem = 0; i < N_samples; ++i) {
            curr_elem = simulate(d);
            if (curr_elem + 1 > sample.getSize()) { // увеличиваем объём массива частот, если не влезает
                auto prev_end = sample.getSize();
                sample.resize(curr_elem + 1);
            }
            ++sample[curr_elem];
        }
        len_sample = sample.getSize();
        return sample.getDataCopy();
    }

    
   

    int get_N() { return N_samples; };
    void set_N(int _N) { N_samples = _N; }

};



/// <summary>
/// Class of sample generation from Poisson distribution by inverse function method
/// </summary>
class PoissonSampleInverse : public PoissonSample
{
public:
    PoissonSampleInverse(mt19937& _gen, Distribution& d, int _N_samples = 100) : PoissonSample(_gen, d, _N_samples) {}
    ~PoissonSampleInverse() override = default;

    int simulate(Distribution& d) const override {
        double lambda = d.get_lambda();
        double alpha = uniform_rand(gen), p_cum, p;
        int k = 0;
        p_cum = p = exp(-lambda);
        while (alpha > p_cum) {
            ++k;
            p *= lambda / k;
            p_cum += p;
        }
        return k;
    }
};


/// <summary>
/// Class of sample generation from Poisson distribution by tabular inverse function method
/// </summary>
class PoissonSampleInverseTable : public PoissonSample
{
public:
    PoissonSampleInverseTable(mt19937& _gen, Distribution& d, int _N_samples = 100) : PoissonSample(_gen, d, _N_samples) {}
    ~PoissonSampleInverseTable() override = default;
    
    

    int simulate(Distribution& d) const override {
        double alpha = uniform_rand(gen);
        int k = 0;
        double p_cum = d.get_th_prob_element(0);
        while (alpha > p_cum) {
            ++k;
            p_cum += d.get_th_prob_element(k);
        }
        return k;
    }
};



/// <summary>
/// Class of sample generation of Poisson distribution modeling through random variables
/// </summary>
class PoissonSampleRandomVariables : public PoissonSample
{
public:
    PoissonSampleRandomVariables(mt19937& _gen, Distribution& d, int _N_samples = 100) : PoissonSample(_gen, d, _N_samples) {}
    ~PoissonSampleRandomVariables() override = default;

    int simulate(Distribution& d) const override {
        double lambda = d.get_lambda();
        int k = 0;
        double p = exp(-lambda), alpha = uniform_rand(gen), q = alpha;
        while (q > p) {
            alpha = uniform_rand(gen);
            q *= alpha;
            ++k;
        }
        return k;
    }
};