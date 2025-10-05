#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <complex>
#include <algorithm>

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

void generate_signal(vector<complex<double>>& signal_zA, vector<complex<double>>& signal_zP, vector<complex<double>>& signal_zF, vector<complex<double>>& signal_mT) {
    signal_zA.resize(N);
    signal_zP.resize(N);
    signal_zF.resize(N);
    signal_mT.resize(N);
    for (int n = 0; n < N; n++) {
        double t = n / fs;
        signal_mT[n] = sin(2 * M_PI * fm * t);
        signal_zA[n] = (kA * signal_mT[n] + 1.0) * cos(2 * M_PI * fn * t);
        signal_zP[n] = cos(2 * M_PI * fn * t + kP * signal_mT[n]);
        signal_zF[n] = cos(2 * M_PI * fn * t + kF / fm * signal_mT[n]);
    }
}

double find_E(vector<double>& amp) {
    double E = 0;
    for (double a : amp) {
        E += pow(a, 2);
    }
    return E;
}

double find_Ea(vector<double>& freq, vector<double>& amp, double alpha) {
    int n = amp.size();
    double max_val = *max_element(amp.begin(), amp.end());
    int max_index = 0;
    for (int i = 0; i < n; i++) {
        if (amp[i] == max_val)
            max_index = i;
    }
    double center_freq = freq[max_index];
    double Ea = 0;
    for (int i = 0; i < n; i++) {
        if (freq[i] >= center_freq - alpha && freq[i] <= center_freq + alpha) {
            Ea += pow(amp[i], 2);
        }
    }
    return Ea;
}

int main() {
    for (int i = 0; i < 3; i++) {
        kA = ka1[i];
        kP = kp1[i];
        kF = kf1[i];

        vector<complex<double>> signal_zA, signal_zP, signal_zF, signal_mT;
        generate_signal(signal_zA, signal_zP, signal_zF, signal_mT);

        vector<complex<double>> zA_fft = fft(signal_zA);
        vector<complex<double>> zP_fft = fft(signal_zP);
        vector<complex<double>> zF_fft = fft(signal_zF);

        vector<double> z_fs, zA_amp, zP_amp, zF_amp;
        for (int n = 0; n < N / 2; n++) {
            double f_k = n * fs / N;
            z_fs.emplace_back(f_k);
            zA_amp.emplace_back((abs(zA_fft[n])));
            zP_amp.emplace_back((abs(zP_fft[n])));
            zF_amp.emplace_back((abs(zF_fft[n])));
        }

        double E = find_E(zA_amp);
        double alpha_zA = 0;
        double rA = 0;
        while (rA < 80) {
            double Ea = find_Ea(z_fs, zA_amp, alpha_zA);
            rA = Ea / E * 100;
            alpha_zA += 1;
        }


        E = find_E(zP_amp);
        double alpha_zP = 0;
        double ratioP = 0;
        while (ratioP < 80) {
            double Ea = find_Ea(z_fs, zP_amp, alpha_zP);
            ratioP = Ea / E * 100;
     
        E = find_E(zF_amp);
        double alpha_zF = 0;
        double ratioF = 0;
        while (ratioF <= 80 && alpha_zF < fn) {
            double Ea = find_Ea(z_fs, zF_amp, alpha_zF);
            ratioF = Ea / E * 100;
            alpha_zF += 1;
        }

        // Wyświetlenie wyników
        cout << "Dla zestawu #" << (i + 1) << ":\n";
        cout << "kA = " << kA << ", B (AM) = " << 2 * (alpha_zA - 1) << " Hz\n";
        cout << "kP = " << kP << ", B (PM) = " << 2 * (alpha_zP - 1) << " Hz\n";
        cout << "kF = " << kF << ", B (FM) = " << 2 * (alpha_zF - 1) << " Hz\n";
        cout << "-------------------------------------------\n";
    }

    return 0;
}
