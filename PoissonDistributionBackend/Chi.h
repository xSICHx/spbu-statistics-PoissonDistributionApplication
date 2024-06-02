#pragma once
#include "PoissonSample.h"
#include "PROBDIST.H"


class Chi2Histortam
{

private:
    int distr_len;
    double* sample_freq;
    double* th_freq;


    double p;
    double chi;
    double df;


    void data_to_chi2(double*& th_chied, double*& sample_chied, int& chied_len) {
        
        // собираем строки в начале
        double tmp_th = 0, tmp_sample = 0;
        int i = 0;
        while (i < distr_len && (tmp_th < 5 || th_freq[i] < 5)) {
            tmp_th += th_freq[i];
            tmp_sample += sample_freq[i];
            ++i;
        }
        /*th_chied_values.push_back(tmp_th);
        sample_chied_values.push_back(tmp_sample);*/

        // собираем строки с конца
        double tmp_th2 = 0, tmp_sample2 = 0;
        int j = distr_len - 1;
        while (j >= 0 && (tmp_th2 < 5 || th_freq[j] < 5)) {
            tmp_th2 += th_freq[j];
            tmp_sample2 += sample_freq[j];
            --j;
        }

        // выделение памяти под массивы
        chied_len = (j - i + 1) + (1) + (1);
        th_chied = new double[chied_len];
        sample_chied = new double[chied_len];

        // собираем строки с середины
        for (int k = i; k <= j; ++k) {
            th_chied[k - i + 1] = th_freq[k];
            sample_chied[k - i + 1] = sample_freq[k];
        }


        // добавляем строку в конец
        th_chied[chied_len - 1] = tmp_th2;
        sample_chied[chied_len - 1] = tmp_sample2;

        // добавляем строку в начало
        th_chied[0] = tmp_th;
        sample_chied[0] = tmp_sample;
    }

    double calculate_chi2(double* th_chied, double* sample_chied, int chied_len ) {
        double res = 0, diff;
        for (int i = 0; i < chied_len; ++i) {
            diff = sample_chied[i] - th_chied[i];
            res += diff * diff / (th_chied[i]);
        }
        return res;
    }

public:
    Chi2Histortam() = default;
    Chi2Histortam(Distribution& d, PoissonSample& sample) {
        sample_freq = sample.generate_sample(d, distr_len);
        th_freq = d.get_th_prob_array(distr_len); // пока что не частоты, а вероятности
        for (int i = 0; i < distr_len; ++i)
            th_freq[i] *= sample.get_N();

    };
    ~Chi2Histortam() {
        delete[] sample_freq;
        delete[] th_freq;
    };


    void set_data(Distribution& d, PoissonSample& sample) {
        delete[] sample_freq;
        delete[] th_freq;


        sample_freq = sample.generate_sample(d, distr_len);
        th_freq = d.get_th_prob_array(distr_len); // пока что не частоты, а вероятности
        for (int i = 0; i < distr_len; ++i)
            th_freq[i] *= sample.get_N();
    }


    void calc_chi() {
        int chied_len;
        double* th_chied = nullptr; double* sample_chied = nullptr;

        data_to_chi2(th_chied, sample_chied, chied_len);
        chi = calculate_chi2(th_chied, sample_chied, chied_len);
        df = chied_len - 1;
        p = 1 - pChi(chi, df);

        delete[] th_chied;
        delete[] sample_chied;
    }


    double get_p() { return p; }
    double get_chi() { return chi; }
    double get_df() { return df; }

    int get_distr_len() { return distr_len; }
    double* get_sample_freq() { return sample_freq; }
    double* get_th_freq() { return th_freq; }
};

