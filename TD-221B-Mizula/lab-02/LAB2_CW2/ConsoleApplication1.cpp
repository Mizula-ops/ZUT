#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <complex>
#include <numbers>
#include <cmath>
#include <utility>
#include "gnuplot-iostream.h"
using namespace std;
double f1 = 50;
double fs = 2000;;
double Tc = 1;
int N = Tc * fs;

vector<complex<double>> dft(vector<complex<double>>& x) {
    vector<complex<double>> result(N);
    for (int k = 0;k < N;k++) {

        for (int n = 0;n < N;n++) {
            double angle = -2.0 * M_PI * k * n / N;
            result[k] += (x[n] * complex<double>(cos(angle), sin(angle)));
        }
    }
    return result;
}
vector<complex<double>> generate_signal_3_tones() {
    vector<complex<double>> signalx(N);
    double f2 = fs / 2 - f1;
    double f3 = (f1 / 2);
    for (int n = 0; n < N; n++) {
        double t = n / fs;


        signalx[n] = sin(2 * M_PI * f1 * t) + sin(2 * M_PI * f2 * t) + sin(2 * M_PI * f3 * t);

    }
    return signalx;
}
int main()
{
    Gnuplot gp;
    vector<complex<double>> x_t = generate_signal_3_tones();
    vector<complex<double>> x_dft = dft(x_t);
    vector<pair<double, double>> signal;
    vector<pair<double, double>> signal_db;
    vector<pair<double, complex<double>>> time;
    for (int k = 0;k < N / 2;k++)
    {
        double f_k = k * fs / N ;
        //Widmo w skali liniowej
        double x = abs(x_dft[k]);
        //Widmo w skali decybelowej
        double f_kdb = 10 * log10(f_k);
        signal.emplace_back(f_kdb, x);     
        double x_l = log10(x);
        signal_db.emplace_back(f_kdb, x_l);
    }
        //Skala liniowa
        gp << "set title 'Wykres Funckji u(t)'\n";
        gp << "set xlabel 'Czestotliwosc [dB]'\n";
        gp << "set ylabel 'Amplituda'\n";
        gp << "plot '-' with lines title '-'\n";
        gp.send1d(signal);


        cout << "Wciśnij Enter, aby przejść do wykresu widma...\n";
        cin.ignore();
        //Skala decybelowa
        gp << "set title 'Wykres Funckji u(t)'\n";
        gp << "set xlabel 'Czestotliwosc [dB]'\n";
        gp << "set ylabel 'Amplituda [dB]'\n";
        gp << "plot '-' with lines title '-'\n";
        gp.send1d(signal_db);
    
}
