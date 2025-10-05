#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <complex>
#include "gnuplot-iostream.h"
using namespace std;

double fm = 5;
double fn = 100;
double fs = 160;
int Tc = 2;
int N = Tc * fs;
vector<double> ka1 = { 0.5, 5, 22 };
vector<double> kp1 = { 0.5, 1.7, 3 * M_PI};
vector<double> kf1 = { 0.5, 1.7, 3 * M_PI};

double kA;
double kP;
double kF;

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

void generate_signal(vector<complex<double>>& signal_zA, vector<complex<double>>& signal_zP, vector<complex<double>>& signal_zF, vector<complex<double>>& signal_mT)
{
    signal_zA.resize(N);
    signal_zP.resize(N);
    signal_zF.resize(N);
    signal_mT.resize(N);
    for (int n = 0; n < N; n++)
    {
        double t = n / fs;
        signal_mT[n] += sin(2 * M_PI * fm * t);
        signal_zA[n] += (kA * signal_mT[n] + 1.0) * cos(2 * M_PI * fn * t);
        signal_zP[n] += cos(2 * M_PI * fn * t + kP * signal_mT[n]);
        signal_zF[n] += cos(2 * M_PI * fn * t + kF / fm * signal_mT[n]);
    }
}

int main()
{
    Gnuplot gp;

    for (int i = 0; i <3; i++)
    {
        kA = ka1[i];
        kP = kp1[i];
        kF = kf1[i];
        vector<complex<double>> signal_zA, signal_zP, signal_zF, signal_mT;
        generate_signal(signal_zA, signal_zP, signal_zF, signal_mT);
        vector<pair<double, complex<double>>> time_zA, time_zP, time_zF, time_mT;
        for (int n = 0; n < N; n++)
        {
            double t = n / fs;
            time_zA.emplace_back(t, signal_zA[n]);
            time_zP.emplace_back(t, signal_zP[n]);
            time_zF.emplace_back(t, signal_zF[n]);
            time_mT.emplace_back(t, signal_mT[n]);
        }
        vector<complex<double>> zA_fft = fft(signal_zA);
        vector<complex<double>> zP_fft = fft(signal_zP);
        vector<complex<double>> zF_fft = fft(signal_zF);
        vector<pair<double, complex<double>>> M_zA, M_zP, M_zF;
        for (int n = 0; n < N / 2; n++)
        {
            double f_k = n * fs / N;
            M_zA.emplace_back(f_k, abs(zA_fft[n]));
            M_zP.emplace_back(f_k, abs(zP_fft[n]));
            M_zF.emplace_back(f_k, abs(zF_fft[n]));
        }
        gp << "set terminal wxt size 2200,800\n";
        gp << "set multiplot layout 1,3 title 'Wykresy czasowe'\n";

        gp << "set title 'Wykres Funkcji x(t)' font ',14'\n";
        gp << "set xlabel 'Czas [s]' font ',12'\n";
        gp << "set ylabel 'Amplituda' font ',12'\n";
        gp << "plot '-' with lines lw 2 linecolor rgb 'orange', '-' with lines lw 2 linecolor rgb 'blue'\n";
        gp.send1d(time_zA);
        gp.send1d(time_mT);

        gp << "set title 'Wykres Funkcji y(t)' font ',14'\n";
        gp << "set xlabel 'Czas [s]' font ',12'\n";
        gp << "set ylabel 'Amplituda' font ',12'\n";
        gp << "plot '-' with lines lw 2 linecolor rgb 'orange', '-' with lines lw 2 linecolor rgb 'blue'\n";
        gp.send1d(time_zP);
        gp.send1d(time_mT);

        gp << "set title 'Wykres Funkcji z(t)' font ',14'\n";
        gp << "set xlabel 'Czas [s]' font ',12'\n";
        gp << "set ylabel 'Amplituda' font ',12'\n";
        gp << "plot '-' with lines lw 2 linecolor rgb 'orange', '-' with lines lw 2 linecolor rgb 'blue'\n";
        gp.send1d(time_zF);
        gp.send1d(time_mT);

        gp << "unset multiplot\n";

        cout << "Wciśnij Enter, aby przejść do wykresów widma Mx, My i Mz...\n";
        cin.ignore();

        gp << "set terminal wxt size 2200,800\n";
        gp << "set multiplot layout 1,3 title 'Widma sygnałów'\n";

        gp << "set title 'Widmo Funkcji x(t)' font ',14'\n";
        gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
        gp << "set ylabel 'Amplituda' font ',12'\n";
        gp << "plot '-' with lines lw 2 linecolor rgb 'red'\n";
        gp.send1d(M_zA);

        gp << "set title 'Widmo Funkcji y(t)' font ',14'\n";
        gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
        gp << "set ylabel 'Amplituda' font ',12'\n";
        gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";
        gp.send1d(M_zP);

        gp << "set title 'Widmo Funkcji z(t)' font ',14'\n";
        gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
        gp << "set ylabel 'Amplituda' font ',12'\n";
        gp << "plot '-' with lines lw 2 linecolor rgb 'orange'\n";
        gp.send1d(M_zF);

        gp << "unset multiplot\n";

        cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
        cin.ignore();
    }
}
