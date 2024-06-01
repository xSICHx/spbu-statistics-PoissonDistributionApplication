#pragma once
#include <iostream>
#include <string>
#include <random>
#include "Vector.h"

#include "Distribution.h"



class PoissonSample
{
protected:
    mt19937& gen;
    uniform_real_distribution<double> uniform_rand = uniform_real_distribution<double>(0.0, 1.0);
    int N;

    Vector sample;

public:
    PoissonSample() = default;
    PoissonSample(mt19937& _gen, Distribution& d, int _N_samples = 100) : gen(_gen), N(_N_samples){}
    virtual ~PoissonSample() = default;

    
    virtual int simulate(Distribution& d) const = 0;
    
    virtual void generate_sample(Distribution& d) {
        if (N < 50) {
            cerr << "Error: Sample size should be more 50" << endl;
        }

        double lambda = d.get_lambda();
        for (int i = 0, curr_elem = 0; i < N; ++i) {
            curr_elem = simulate(d);
            if (curr_elem + 1 > sample.getSize()) { // увеличиваем объём массива частот, если не влезает
                auto prev_end = sample.getSize();
                sample.resize(curr_elem + 1);
            }
            ++sample[curr_elem];
        }
    }

    
   

    double* get_sample_freq(int& n) { 
        n = sample.getSize(); 
        return sample.getData();
    }
    int get_N() { return N; };
    void set_N(int _N) { N = _N; }

};




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




class PoissonSampleRandomVariables : public PoissonSample
{
public:
    PoissonSampleRandomVariables(mt19937& _gen, Distribution& d, int _N_samples = 100) : PoissonSample(_gen, d, _N_samples) {}
    ~PoissonSampleRandomVariables() override = default;

    int simulate(Distribution& d) const override {
        double lambda = d.get_lambda();
        int k = 0;
        double p = exp(-lambda), alpha = uniform_rand(gen), q = alpha;
        while (q >= p) {
            alpha = uniform_rand(gen);
            q *= alpha;
            ++k;
        }
        return k;
    }
};