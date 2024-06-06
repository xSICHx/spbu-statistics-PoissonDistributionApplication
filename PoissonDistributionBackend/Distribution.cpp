#include "Distribution.h"
#include <cmath>

using namespace std;

/// <summary>
/// Changing the size of theoretical probabilities
/// </summary>
/// <param name="n">To what number do you have to count to</param>
void Distribution::resize_th_prob(const int n) {
    if (th_prob.getSize() < n) {
        double p_cum = 1 - th_prob[th_prob.getSize() - 1];
        // обновление последнего элемента, так как он равен 1 - сумма всех начальных
        th_prob[th_prob.getSize() - 1] = th_prob[th_prob.getSize() - 2] * (lambda / ((double)th_prob.getSize() - 1));
        for (int i = th_prob.getSize(); i < n; ++i) { // заполнение оставшихся элементов
            th_prob.push(th_prob[i - 1] * (lambda / i));
            p_cum += th_prob[i];
        }
        th_prob.push(1 - p_cum);
    }
}

/// <summary>
/// Creates an initial Poisson distribution
/// </summary>
/// <param name="_lambda">Distribution parameter</param>
Distribution::Distribution(const double _lambda) : lambda(_lambda), th_prob(max((int)(lambda + 2 * lambda), 10)) {
    // инициализация начального распределения
    th_prob.push(exp(-lambda));
    double p_cum = th_prob[0];
    for (int i = 1; i < th_prob.getCapacity() - 1; ++i) {
        th_prob.push(th_prob[i - 1] * (lambda / i));
        p_cum += th_prob[i];
    }
    th_prob.push(1 - p_cum);
}

/// <summary>
/// Returns double* an array of the first n theoretical values. 
/// </summary>
/// <param name="n">Number of values</param>
/// <returns>Array of theoretical values</returns>
double* Distribution::get_th_prob_array(int n) {
    resize_th_prob(n);
    double* res = new double[n];
    double p_cum = 0;
    for (int i = 0; i < n - 1; ++i) {
        p_cum += th_prob[i];
        res[i] = th_prob[i];
    }
    res[n - 1] = 1 - p_cum;
    return res;
}

/// <summary>
/// Returns the element of the theoretical value
/// </summary>
/// <param name="index">Item number</param>
/// <returns>Value</returns>
double Distribution::get_th_prob_element(int index) {
    resize_th_prob(index + 1);
    return th_prob[index];
}

/// <summary>
/// Setter lambda. Updates the array of theoretical distribution
/// </summary>
/// <param name="_lambda">Distribution parameter</param>
void Distribution::set_lambda(double _lambda) {
    lambda = _lambda;
    th_prob.clear();
    th_prob.push(exp(-lambda));
    th_prob.reserve((int)(lambda + lambda * 3));
    double p_cum = th_prob[0];
    for (int i = 1; i < th_prob.getCapacity() - 1; ++i) {
        th_prob.push(th_prob[i - 1] * (lambda / i));
        p_cum += th_prob[i];
    }
    th_prob.push(1 - p_cum);
}

// Getters
string Distribution::get_name() {
    return distr_name;
}

double Distribution::get_lambda() const {
    return lambda;
}
