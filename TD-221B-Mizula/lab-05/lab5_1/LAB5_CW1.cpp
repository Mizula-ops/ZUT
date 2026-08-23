#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <complex>
#include <algorithm>
#include "gnuplot-iostream.h"
using namespace std;

double fm = 5;
double fn = 1000;
double fs = 160;
int Tc = 2;
int N = Tc * fs;
vector<double> ka1 = { 0.5, 5, 22 };
vector<double> kp1 = { 0.5, 1.7, 3 * M_PI };
vector<double> kf1 = { 0.5, 1.7, 3 * M_PI };

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
double find_bandwidth(vector<double>& freq, vector<double>& amp, double level_dB) {
    double max_y = *max_element(amp.begin(), amp.end());
    double threshold = max_y - level_dB;

    int n = amp.size();
    int left_index = -1;
    int right_index = -1;

    for (int i = 0; i < n; i++) {
        if (amp[i] >= threshold) {
            if (left_index == -1) {
                left_index = i;
            }
            right_index = i;
        }
    }

    if (left_index != -1 && right_index != -1) {
        return freq[right_index] - freq[left_index];
    }

    return 0.0; 
}


int main()
{
    Gnuplot gp;

    for (int i = 0; i < 3; i++)
    {
        kA = ka1[i];
        kP = kp1[i];
        kF = kf1[i];
        vector<complex<double>> signal_zA, signal_zP, signal_zF, signal_mT;
        generate_signal(signal_zA, signal_zP, signal_zF, signal_mT);
     
      
        vector<complex<double>> zA_fft = fft(signal_zA);
        vector<complex<double>> zP_fft = fft(signal_zP);
        vector<complex<double>> zF_fft = fft(signal_zF);

        vector<double> z_fs,zA_amp,zP_amp,zF_amp;
     ;
        vector<pair<double,double>> M_zA, M_zP, M_zF;
        for (int n = 0; n < N / 2; n++)
        {
            double f_k = n * fs / N;
            z_fs.emplace_back(f_k);
            zA_amp.emplace_back(10*log10(abs(zA_fft[n])));
            zP_amp.emplace_back(10*log10(abs(zP_fft[n])));
            zF_amp.emplace_back(10*log10(abs(zF_fft[n])));

            M_zA.emplace_back(f_k, 10*log10(abs(zA_fft[n])));
            M_zP.emplace_back(f_k, 10*log10(abs(zP_fft[n])));
            M_zF.emplace_back(f_k, 
                10*log10(abs(zF_fft[n])));
        }
        double B3dB_zA = find_bandwidth(z_fs, zA_amp, 3);
        double B3dB_zP = find_bandwidth(z_fs, zP_amp, 3);
        double B3dB_zF = find_bandwidth(z_fs, zF_amp, 3);
        double B6dB_zA = find_bandwidth(z_fs, zA_amp, 6);
        double B6dB_zP = find_bandwidth(z_fs, zP_amp, 6);
        double B6dB_zF = find_bandwidth(z_fs, zF_amp, 6);
        double B10dB_zA = find_bandwidth(z_fs, zA_amp, 10);
        double B10dB_zP = find_bandwidth(z_fs, zP_amp, 10);
        double B10dB_zF = find_bandwidth(z_fs, zF_amp, 10);
        cout << "Szerokosc pasma B3dB dla zA: " << B3dB_zA << endl;
        cout << "Szerokosc pasma B3dB dla zP: " << B3dB_zP<<endl;
        cout << "Szerokosc pasma B3dB dla zF: " << B3dB_zF<<endl;
        cout << "Szerokosc pasma B6dB dla zA: " << B6dB_zA << endl;
        cout << "Szerokosc pasma B6dB dla zP: " << B6dB_zP << endl;
        cout << "Szerokosc pasma B6dB dla zF: " << B6dB_zF << endl;
        cout << "Szerokosc pasma B10dB dla zA: " << B10dB_zA << endl;
        cout << "Szerokosc pasma B10dB dla zP: " << B10dB_zP << endl;
        cout << "Szerokosc pasma B10dB dla zF: " << B10dB_zF << endl;

        gp << "set terminal wxt size 2200,800\n";
        gp << "set multiplot layout 1,3 title 'Wykresy czasowe'\n";



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
