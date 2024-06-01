#pragma once
#include <iostream>
#include <string>
#include "Vector.h"

using namespace std;


class Distribution
{
private:
    double lambda;
    const string distr_name = "Poisson distribution";
    Vector th_prob;

    void resize_th_prob(int n) {
        if (th_prob.getSize() < n) {
            double p_cum = 1 - th_prob[th_prob.getSize() - 1];
            // обновление последнего элемента, так как он равен 1 - сумма всех начальных
            th_prob[th_prob.getSize() - 1] = th_prob[th_prob.getSize() - 2] * (lambda / (th_prob.getSize() - 1));
            for (int i = th_prob.getSize(); i < n; ++i) { // заполнение оставшихся элементов
                th_prob.push(th_prob[i - 1] * (lambda / i));
                p_cum += th_prob[i];
            }
            th_prob.push(1 - p_cum);
        }
    }

public:
    Distribution(double _lambda = 5) : lambda(_lambda), th_prob(){
        // инициализация начального распределения
        th_prob.push(exp(-lambda));
        double p_cum = th_prob[0];
        for (int i = 1; i < th_prob.getCapacity() - 1; ++i) {
            th_prob.push(th_prob[i-1] * (lambda / i));
            p_cum += th_prob[i];
        }
        th_prob.push(1-p_cum);
    };

    ~Distribution() = default;

    /*vector<double> compute_th_distr(unsigned int n) {
        vector<double> thDistr(n + 1);
        thDistr[0] = exp(-lambda);
        double p_cum = thDistr[0];
        for (unsigned int i = 1; i < n; ++i) {
            thDistr[i] = thDistr[i - 1] * (lambda / i);
            p_cum += thDistr[i];
        }
        thDistr[n] = 1 - p_cum;
        return thDistr;
    }*/



    double* get_th_prob_array(int n) {
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

    double get_th_prob_element(int index) {
        resize_th_prob(index+1);
        return th_prob[index];
    }


    // Setter lambda. Обновляет массив теоретического распределения
    void set_lambda(double _lambda) {
        lambda = _lambda;
        th_prob.clear();
        th_prob.push(exp(-lambda));
        double p_cum = th_prob[0];
        for (int i = 1; i < th_prob.getCapacity() - 1; ++i) {
            th_prob.push(th_prob[i - 1] * (lambda / i));
            p_cum += th_prob[i];
        }
        th_prob.push(1 - p_cum);
    }

    // Getters
    string get_name() {
        return distr_name;
    }

    double get_lambda() const { return lambda; };

    
};
