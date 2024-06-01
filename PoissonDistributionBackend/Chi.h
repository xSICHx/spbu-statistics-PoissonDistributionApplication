#pragma once
#include "PoissonSample.h"
#include "PROBDIST.H"


class Chi2Histortam
{
public:
    void print_vec(vector<double> vec) {
        for (size_t i = 0; i < vec.size(); i++)
        {
            cout << vec[i] << " ";
        }
        cout << endl;
    }
    Chi2Histortam() = default;
    Chi2Histortam(Distribution* _d, PoissonSample* _sample) : d(_d), sample(_sample){}
    ~Chi2Histortam() {
        sample = nullptr;
        d = nullptr;
    };


    void calc_chi() {
        th_chied_values.clear();
        sample_chied_values.clear();

        vector<double> th_counter = convert_th_freq_to_counters(d->compute_th_distr(sample->get_N()));
        data_to_chi2(th_counter, sample->get_sample_freq());
        chi = calculate_chi2(sample_chied_values, th_chied_values);
        df = th_chied_values.size() - 1;
        p = 1 - pChi(chi, df);
    }

private:
// ToDo: перепписать так, чтобы этого не было в полях
    Distribution* d; // убрать
    PoissonSample* sample; // убрать

    vector<double> th_chied_values; // без этого
    vector<double> sample_chied_values; // без этого

    unsigned int min_index_chi; // вообще бесполезно
    unsigned int max_index_chi; // вообще бесполезно

    double p;
    double chi;
    double df;
    
public:
    double get_p() { return p; }
    double get_chi() { return chi; }
    double get_df() { return df; }
    unsigned int get_min_index() { return min_index_chi; }
    unsigned int get_max_index() { return max_index_chi; }
    vector<double> get_th_chied_values() { return th_chied_values; }
    vector<double> get_sample_chied_values() { return sample_chied_values; }
    PoissonSample* get_sample() { return sample; }

    void set_data(Distribution* _d, PoissonSample* _sample){
        d = _d;
        sample = _sample;
        
    }
private:
    void data_to_chi2(const vector<double>& th_vals, const vector<double>& sample_values) {
        th_chied_values.clear();
        sample_chied_values.clear();

        // собираем строки в начале
        double tmp_th = 0, tmp_sample = 0;
        int i = 0;
        // 4 столбец отвечате за теоретическое распределение
        while (i < th_vals.size() && (tmp_th < 5 || th_vals[i] < 5)) {
            min_index_chi = i;
            tmp_th += th_vals[i];
            tmp_sample += sample_values[i];
            ++i;
        }
        th_chied_values.push_back(tmp_th);
        sample_chied_values.push_back(tmp_sample);

        // собираем строки с конца
        double tmp_th2 = 0, tmp_sample2 = 0;
        int j = th_vals.size() - 1;
        while (j >= 0 && (tmp_th2 < 5 || th_vals[j] < 5)) {
            max_index_chi = j;
            tmp_th2 += th_vals[j];
            tmp_sample2 += sample_values[j];
            --j;
        }

        // собираем строки с середины
        for (int k = i; k <= j; ++k) {
            th_chied_values.push_back(th_vals[k]);
            sample_chied_values.push_back(sample_values[k]);
        }


        // добавляем строку в конец
        th_chied_values.push_back(tmp_th2);
        sample_chied_values.push_back(tmp_sample2);
    }
    double calculate_chi2(const vector<double>& sample_freq, const vector<double>& th_freq) {
        double res = 0, diff;
        for (int i = 0; i < sample_freq.size(); ++i) {
            diff = sample_freq[i] - th_freq[i];
            res += diff * diff / (th_freq[i]);
        }
        return res;
    }
    vector<double> convert_th_freq_to_counters(const vector<double> th_freq) {
        vector<double> res = vector<double>(sample->get_sample_freq().size());
        for (size_t i = 0; i < sample->get_sample_freq().size(); i++)
        {
            res[i] = th_freq[i] * sample->get_N();
        }
        return res;
    }

    

};

