#include <iostream>
#include "Distribution.h"
#include "Vector.h"
#include <vector>

using namespace std;

int main()
{
    double lambda = 5;
    int n = 11;

    vector<double> thDistr(n);
    thDistr[0] = exp(-lambda);
    double p_cum = thDistr[0];
    for (unsigned int i = 1; i < n - 1; ++i) {
        thDistr[i] = thDistr[i - 1] * (lambda / i);
        p_cum += thDistr[i];
    }
    thDistr[n - 1] = 1 - p_cum;

    for (size_t i = 0; i < n; i++)
    {
        cout << thDistr[i] << " ";
    }
    cout << endl;


    Distribution d(lambda);

    double* th = d.get_th_prob_array(n);
	for (size_t i = 0; i < n; i++)
	{
		cout << th[i] << " ";
	}
	cout << endl;


    

    delete[] th;

    /*Vector v;
    for (int i = 0; i < 100; ++i) {
        v.push(i);
    }

    v.print_Vector();*/

}

