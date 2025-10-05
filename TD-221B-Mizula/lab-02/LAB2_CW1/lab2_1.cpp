#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <complex>
#include <numbers>
#include <cmath>
#include <utility>
#include "gnuplot-iostream.h"
using namespace std;
double f = 500;
double fs = 16000;;
double Tc = 2;
int N = Tc * fs;
double o = M_PI / 2;
vector<complex<double>> dft(vector<complex<double>> &x) {
    vector<complex<double>> result(N);
    for (int k = 0;k < N;k++) {
        
        for (int n = 0;n < N;n++) {
            double angle = -2.0 * M_PI * k * n / N;
            result[k] += x[n] * complex<double>(cos(angle), sin(angle));
        }
    }
    return result;
}
vector<complex<double>> generate_signal() {
    vector<complex<double>> signalx(N);
    for (int n = 0; n < N; n++) {
        double t = n / fs;


        signalx[n] = sin(2 * M_PI * f * t);
    }
    return signalx;
}

int main()
{
    Gnuplot gp;
    vector<complex<double>> x_t = generate_signal();
    vector<complex<double>> x_dft = dft(x_t);
    vector<pair<double, double>> signal;
    vector<pair<double, double>> signal_db;
    vector<pair<double,complex<double>>> time;
    //Czas
    for (int k = 0;k < N;k++)
    {
        double t = k / fs;
        time.emplace_back(t, x_t[k]);
    }
    //Widma
    for (int k = 0;k < N/2;k++)
    {
        double f_k = k*  fs/N;
        //Widmo w skali liniowej
        double x = abs(x_dft[k]);
        signal.emplace_back(f_k, x);
        //Widmo w skali decybelowej
        double x_db = 10 * log10(x);
        signal_db.emplace_back(f_k, x_db);
    }
   
    //Skala liniowa
    gp << "set title 'Wykres Funckji u(t)'\n";
    gp << "set xlabel 'Częstotliwość [kHz]'\n";
    gp << "set ylabel 'Amplituda'\n";


    gp << "plot '-' with lines title '-'\n";
    gp.send1d(signal);
   
    
    cout << "Wciśnij Enter, aby przejść do wykresu widma...\n";
    cin.ignore();
    //Skala decybelowa
    gp << "set title 'Wykres Funckji u(t)'\n";
    gp << "set xlabel 'Częstotliwość [DB]'\n";
    gp << "set ylabel 'Amplituda'\n";

    gp << "plot '-' with lines title '-'\n";
    gp.send1d(signal_db);


    cout << "Wciśnij Enter, aby przejść do wykresu widma...\n";
    cin.ignore();
    //Wykres czasu
    gp << "set title 'Sygnał w dziedzinie czasu u(t)'\n";
    gp << "set xlabel 'Czas [s]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "set yrange [-1:1]\n";
    gp << "set xrange [0:0.012]\n";
    gp << "plot '-' with lines title 'Sygnał u(t)'\n";
    gp.send1d(time);
    
}