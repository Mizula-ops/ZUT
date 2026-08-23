// ALGO2 IS1 211B LAB10
// ARTUR MIZUŁA
// ma55831@zut.edu.pl

#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <complex>
#include <numbers>
#include <cmath>

using namespace std;
string print_table(double* array, int n)
{
	std::string krokiet = "Elementy: |";

	if (n > 10)
	{
		for (int i = 0;i < 5;i++) krokiet += to_string(array[i]) + "|";
		krokiet += "...";
		for (int i = n - 5;i < n;i++) krokiet += std::to_string(array[i]) + "|";
	}
	else
	{
		for (int i = 0;i < n;i++) krokiet += std::to_string(array[i]) + "|";
	}
	return krokiet;
}
#include <complex>
#include <string>
#include <iostream>

std::string print_table_ff(std::complex<double>* array, bool see, int n)
{
    if (see)
    {
        
        std::string krokiet = "";

        if (n > 8)
        {
            for (int i = 0; i < 4; i++)
                krokiet += "(" +to_string(array[i].real()) +"," + to_string(abs(array[i].imag())) + "i), ";
            krokiet += "...";
            for (int i = n - 4; i < n; i++)
                krokiet += "(" + std::to_string(array[i].real()) + "," + std::to_string(abs(array[i].imag())) + "i), ";
        }
        else
        {
            for (int i = 0; i < n; i++)
                krokiet += "(" + std::to_string(array[i].real()) + " " + std::to_string(abs(array[i].imag())) + "i), ";
        }

        return krokiet;
    }

    return ""; 
}

complex<double>* dft(double* f, int N) {
    complex<double>* result = new complex<double>[N];
    for (int k = 0;k < N;k++) {
        result[k] = 0;
        for (int n = 0;n < N;n++) {
            double angle = -2.0 * M_PI * k * n / N;
            result[k] += f[n] * complex<double>(cos(angle), sin(angle));
        }
    }
    return result;
}
complex<double>* fft(double* f, int N) {
    if (N == 1) {
        return new complex<double>(f[0], 0);
    }
    int halfN = N / 2;
    double* f_even = new double[halfN];
    double* f_odd = new double[halfN];

    for (int i = 0;i < halfN;i++) {
        f_even[i] = f[2 * i];
        f_odd[i] = f[2 * i + 1];
    }
    complex<double>* fft_even = fft(f_even, halfN);
    complex<double>* fft_odd = fft(f_odd, halfN);

    complex<double>* result = new complex<double>[N];
    for (int k = 0;k < halfN;k++) {
        double angle = -2.0 * M_PI * k / N;
        complex<double> t = complex<double>(cos(angle), sin(angle)) * fft_odd[k];
        result[k] = fft_even[k] + t;
        result[k + halfN] = fft_even[k] - t;
    }
    delete[] f_even;
    delete[] f_odd;
    delete[] fft_even;
    delete[] fft_odd;

    return result;
}
double err(complex<double>* dft, complex<double>* fft,int n) {
    double error = 0.0;
    for (int k = 0; k < n; ++k) {
        error += abs(dft[k] - fft[k]);
    }
    return error / n;
}
//cout<<print_table(f, N)<<endl;
int main()
{
    const int MAX_ORDER = 4;
    const bool PRINT_COEFS = true;

    for (int o = 1;o < MAX_ORDER; o++)
    {
        const int N = 1 << o;
        printf("N: %i \n", N);

        double* f = new double[N];
        for (int n = 0;n < N;n++)
            f[n] = n / (double)N;


        clock_t t1 = clock();
		complex<double>* cDFT = dft(f, N);
        clock_t t2 = clock();
        double dft_time = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC) * 1000.0;
       
        printf("DFT time [ms]: %f \n", dft_time);

        t1 = clock();
        complex<double>* cFFT = fft(f, N);
        t2 = clock();
        double fft_time = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC) * 1000.0;
        printf("FFT time [ms]: %f \n", fft_time);
       
        
        printf("mean error : % f \n", err(cDFT, cFFT, N));
        if (PRINT_COEFS) {
            cout << "DFT:" << print_table_ff(cDFT, PRINT_COEFS, N) << endl;
            cout << "FFT:" << print_table_ff(cFFT, PRINT_COEFS, N) << endl;
        }
        
        
        delete[] f;
        delete[] cDFT;
        delete[] cFFT;
    }
        

    
}

