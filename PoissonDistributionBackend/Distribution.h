#pragma once
#include <iostream>
#include <string>
#include "Vector.h"

using namespace std;

/// <summary>
/// Poisson distribution class. Calculates the theoretical distribution and stores it in itself
/// </summary>
class Distribution
{
private:
    /// <summary>
    /// Parameter lambda of the Poisson distribution
    /// </summary>
    double lambda;
    /// <summary>
    /// Name of distribution
    /// </summary>
    const string distr_name = "Poisson distribution";
    /// <summary>
    /// An array of theoretical probabilities. Updated as needed
    /// </summary>
    //Vector th_prob;
    double* th_probability = nullptr;
    int distr_len = 0;

    /// <summary>
    /// Changing the size of theoretical probabilities
    /// </summary>
    /// <param name="n">To what number do you have to count to</param>
    void resize_th_prob(const int n) {
        Vector th_prob;
        th_prob.resize(n);
        th_prob[0] = (exp(-lambda));
        double p_cum = th_prob[0];
        for (int i = 1; i < n-1; ++i) {
            th_prob[i] = (th_prob[i - 1] * (lambda / i));
            p_cum += th_prob[i];
        }
        if (n != 1)
            th_prob[n - 1] = 1 - p_cum;

        if (th_probability != nullptr)
            delete[] th_probability;
        th_probability = th_prob.getDataCopy();

        distr_len = n;
    }

public:
    /// <summary>
    /// Creates an initial Poisson distribution
    /// </summary>
    /// <param name="_lambda">Distribution parameter</param>
    Distribution(const double _lambda = 5) : lambda(_lambda){
        // инициализация начального распределения
        resize_th_prob(max((int)(lambda + 2 * lambda), 10));
    };

    ~Distribution() {
        if (th_probability != nullptr)
            delete[] th_probability;
    };

    


    /// <summary>
    /// Returns double* an array of the first n theoretical values.  Must be deleted.
    /// </summary>
    /// <param name="n">Number of values</param>
    /// <returns>Array of theoretical values</returns>
    const double* get_th_prob_array(int n) {
        resize_th_prob(n);
        return th_probability;
    }

    /// <summary>
    /// Returns the element of the theoretical value
    /// </summary>
    /// <param name="index">Item number</param>
    /// <returns>Value</returns>
    double get_th_prob_element(int index) {
        if (index >= distr_len - 1)
            resize_th_prob((index+10)*2);
        return th_probability[index];
    }

    
    /// <summary>
    /// Setter lambda. Updates the array of theoretical distribution
    /// </summary>
    /// <param name="_lambda">Distribution parameter</param>
    void set_lambda(double _lambda) {
        lambda = _lambda;
        resize_th_prob(max((int)(lambda + 2 * lambda), 10));
    }

    // Getters
    string get_name() const {
        return distr_name;
    }

    int get_distr_len() const { return distr_len; }
    double get_lambda() const { return lambda; };

    
};
