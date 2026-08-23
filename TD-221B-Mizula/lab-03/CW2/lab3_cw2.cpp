#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
#include <utility>

#include "gnuplot-iostream.h"
using namespace std;

double f1 = 5;      
double f2= 15;
double fs = 160;     
double Tc = 2;       
int N = Tc * fs;     
int H = 10;

vector<complex<double>> fft(vector<complex<double>>& f) {
    int N = f.size();
    if (N == 1) {
        return f;
    }
    int halfN = N / 2;
    vector<complex<double>> f_even(halfN);
    vector<complex<double>> f_odd(halfN);

    for (int i = 0; i < halfN; i++) {
        f_even[i] = f[2 * i];
        f_odd[i] = f[2 * i + 1];
    }
    vector<complex<double>> fft_even = fft(f_even);
    vector<complex<double>> fft_odd = fft(f_odd);

    vector<complex<double>> result(N);
    for (int k = 0; k < halfN; k++) {
        double angle = -2.0 * M_PI * k / N;
        complex<double> t = complex<double>(cos(angle), sin(angle)) * fft_odd[k];
        result[k] = fft_even[k] + t;
        result[k + halfN] = fft_even[k] - t;
    }

    return result;
}
vector<complex<double>> generate_signal_x() {
    vector<complex<double>> signalx(N, 0);

        for (int n = 0; n < N; n++) {
            double t = n / fs;
            signalx[n] += 0.5*sin(2*M_PI *f1*t);
        }
    
    return signalx;
}
vector<complex<double>> generate_signal_y() {
    vector<complex<double>> signaly(N, 0);

    for (int n = 0; n < N; n++) {
        double t = n / fs;
        signaly[n] += sin(2*M_PI*f2*t)+0.7*sin(2*M_PI *f1 *t);
    }

    return signaly;
}
int main()
{
    Gnuplot gp;
    vector<complex<double>> x = generate_signal_x();
    vector<complex<double>> y = generate_signal_y();
    vector<complex<double>> z (N);
    vector<pair<double, complex<double>>> timex;
    vector<pair<double, complex<double>>> timey;
    vector<pair<double, complex<double>>> timez;
    double alpha = 2.0, beta = 3.0;

    for (int i = 0;i < N;i++)
    {
        double t = i / fs;
        z[i] = alpha * x[i] + beta * y[i];
        timex.emplace_back(t, x[i]);
        timey.emplace_back(t, y[i]);
        timez.emplace_back(t, z[i]);
    }
    
    vector<complex<double>> x_fft = fft(x);
    vector<complex<double>> y_fft = fft(y);
    vector<complex<double>> z_fft = fft(z);
    vector<pair<double, double>> Mx;
    vector<pair<double, double>> My;
    vector<pair<double, double>> Mz_fft;
    vector<pair<double, double>> Mz;

    for (int k = 0; k < N / 2; k++) {
        double f_k = k * fs / N;
      
        double x = abs(x_fft[k]);
        double y = abs(y_fft[k]);
        double z_f = abs(z_fft[k]);
        double z = alpha * x + beta * y;
        Mx.emplace_back(f_k, x);
        My.emplace_back(f_k, y);
        Mz_fft.emplace_back(f_k, z_f);
        Mz.emplace_back(f_k, z);
    }
   

  
    // Wykresy czasowe
    gp << "set terminal wxt size 2200,800\n";

    gp << "set multiplot layout 1,3 title 'Wykresy czasowe'\n";
    gp << "set title 'Wykres Funkcji x(t)' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 title 'x(t)'\n";
    gp.send1d(timex);

    gp << "set title 'Wykres Funkcji y(t)' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 title 'y(t)'\n";
    gp.send1d(timey);

    gp << "set title 'Wykres Funkcji z(t)' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 title 'z(t)'\n";
    gp.send1d(timez);

    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby przejść do wykresów widma Mx i My...\n";
    cin.ignore();

    // Widma Mx i My
    gp << "set multiplot layout 1,2 title 'Widma Mx i My'\n";
    gp << "set title 'Widmo Mx'\n";
    gp << "set xlabel 'Częstotliwość [Hz]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'Mx'\n";
    gp.send1d(Mx);

    gp << "set title 'Widmo My'\n";
    gp << "set xlabel 'Częstotliwość [Hz]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'My'\n";
    gp.send1d(My);

    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby przejść do wykresów widma Mz_fft i Mz...\n";
    cin.ignore();

    // Widma Mz_fft i Mz
    gp << "set multiplot layout 1,2 title 'Widma Mz_fft i Mz'\n";
    gp << "set title 'Widmo Mz z FFT'\n";
    gp << "set xlabel 'Częstotliwość [Hz]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'Mz_fft'\n";
    gp.send1d(Mz_fft);

    gp << "set title 'Widmo Mz'\n";
    gp << "set xlabel 'Częstotliwość [Hz]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'Mz'\n";
    gp.send1d(Mz);

    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby zakończyć...\n";
    cin.ignore();
}

