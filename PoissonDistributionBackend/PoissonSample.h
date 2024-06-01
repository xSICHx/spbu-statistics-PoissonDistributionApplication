#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "Distribution.h"



class PoissonSample
{
public:
    PoissonSample() = default;
    PoissonSample(mt19937& _gen, Distribution& d, int _N_samples = 100) : gen(_gen), N(_N_samples) {
    }
    virtual ~PoissonSample() = default;

    
    virtual int simulate(const Distribution& d) const = 0;
    
    virtual void generate_sample(Distribution& d) {
        if (N < 50) {
            cerr << "Error: Sample size should be more 50" << endl;
        }

        double lambda = d.get_lambda();
        sample = vector<double>((int)lambda, 0);
        simulate(d);
        for (int i = 0, curr_elem = 0; i < N; ++i) {
            curr_elem = simulate(d);
            if (curr_elem + 1 > sample.size()) { // увеличиваем объём массива частот, если не влезает
                auto prev_end = sample.size();
                sample.resize(curr_elem + 1);
                for (; prev_end < sample.size(); ++prev_end)
                    sample[prev_end] = 0;
            }
            ++sample[curr_elem];
        }
    }

    
   

    vector<double> get_sample() { return sample; }
    int get_N() { return N; };
    void set_N(int _N) { N = _N; }
public:
    mt19937& gen;
    uniform_real_distribution<double> uniform_rand = uniform_real_distribution<double>(0.0, 1.0);
    int N;
    vector<double> sample;
};




class PoissonSampleInverse : public PoissonSample
{
public:
    PoissonSampleInverse(mt19937& _gen, Distribution& d, int _N_samples = 100) : PoissonSample(_gen, d, _N_samples) {}
    ~PoissonSampleInverse() override = default;

    int simulate(const Distribution& d) const override {
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

    vector<double> th_cum_sum;

    void generate_sample(Distribution& d) {
        if (N < 50) {
            cerr << "Error: Sample size should be more 50" << endl;
        }

        double lambda = d.get_lambda();
        sample = vector<double>((int)lambda, 0);

        // подготовительные операции
        vector<double> th_distr = d.compute_th_distr((int)lambda*lambda*2 + 1);
        th_cum_sum = vector<double>(th_distr.size());
        th_cum_sum[0] = th_distr[0];
        for (int i = 1; i < th_cum_sum.size(); ++i) {
            th_cum_sum[i] = th_cum_sum[i - 1] + th_distr[i];
        }

        simulate(d);
        for (int i = 0, curr_elem = 0; i < N; ++i) {
            curr_elem = simulate(d);
            if (curr_elem + 1 > sample.size()) { // увеличиваем объём массива частот, если не влезает
                auto prev_end = sample.size();
                sample.resize(curr_elem + 1);
                for (; prev_end < sample.size(); ++prev_end)
                    sample[prev_end] = 0;
            }
            ++sample[curr_elem];
        }
    }
    

    int simulate(const Distribution& d) const override {
        double alpha = uniform_rand(gen);
        int k = 0;
        while (alpha > th_cum_sum[k]) {
            ++k;
        }
        return k;
    }
};




class PoissonSampleRandomVariables : public PoissonSample
{
public:
    PoissonSampleRandomVariables(mt19937& _gen, Distribution& d, int _N_samples = 100) : PoissonSample(_gen, d, _N_samples) {}
    ~PoissonSampleRandomVariables() override = default;

    int simulate(const Distribution& d) const override {
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