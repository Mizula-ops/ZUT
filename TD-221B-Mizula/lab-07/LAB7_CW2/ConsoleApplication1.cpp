//ARTUR MIZUŁA

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <complex>
#include <algorithm>
#include <bitset>
#include <fstream>
#include "gnuplot-iostream.h"

using namespace std;

double W = 2;
double B = 10;
double Tc = 0.3;
double Tb = Tc / B;
double fn = W / Tb;
double fn1 = (W + 1) / Tb;
double fn2 = (W + 2) / Tb;
double fs = 8 * fn2;
double A1 = 0.5;
double A2 = 1;
const int samples_per_bit = static_cast<int>(fs * Tb);

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
vector<pair<double, complex<double>>> generateSignalASK(const vector<int>& bits) {
    vector<pair<double, complex<double>>> time;

    for (size_t i = 0; i < bits.size(); ++i) {
        double bitStart = i * Tb;
        for (int j = 0; j < Tb * fs; ++j) {
            double t = bitStart + j / fs;
            double amplitude;
            if (bits[i] == 0)
                amplitude = A1;
            else
                amplitude = A2;
            complex<double> value = amplitude * sin(2 * M_PI * fn * t);
            time.emplace_back(t, value);
        }
    }

    return time;
}
vector<pair<double, complex<double>>> generateSignalFSK(const vector<int>& bits) {
    vector<pair<double, complex<double>>> time;

    for (size_t i = 0; i < bits.size(); ++i) {
        double bitStart = i * Tb;
        for (int j = 0; j < Tb * fs; ++j) {
            double t = bitStart + j / fs;
            double f;
            if (bits[i] == 0)
                f = fn1;
            else
                f = fn2;
            complex<double> value = 1.0 * sin(2 * M_PI * f * t);
            time.emplace_back(t, value);
        }
    }

    return time;
}
vector<pair<double, complex<double>>> generateSignalPSK(const vector<int>& bits) {
    vector<pair<double, complex<double>>> time;

    for (size_t i = 0; i < bits.size(); ++i) {
        double bitStart = i * Tb;
        for (int j = 0; j < Tb * fs; ++j) {
            double t = bitStart + j / fs;
            double pi;
            if (bits[i] == 0)
                pi = 0;
            else
                pi = M_PI;
            complex<double> value = 1.0 * sin(2 * M_PI * fn * t + pi);
            time.emplace_back(t, value);
        }
    }

    return time;
}
vector<pair<double, double>> computeSpectrum(const vector<pair<double, complex<double>>>& signal) {
    size_t N = signal.size();


    vector<complex<double>> input(N);
    for (size_t i = 0; i < N; ++i) {
        input[i] = signal[i].second.real();
    }
    vector<complex<double>> spectrum = fft(input);
    vector<pair<double, double>> result;
    for (size_t i = 0; i < N / 2; ++i) {
        double freq = i * fn / N;
        double magnitude = abs(spectrum[i]);
        double magnitude_dB = magnitude;
        result.emplace_back(freq, magnitude_dB);
    }

    return result;
}
vector<int> stringToBitStream(const string& text) {
    vector<int> bitStream;

    for (char c : text) {
        int ascii = static_cast<int>(c);
        if (ascii < 32 || ascii > 127) {
            cerr << "Niedozwolony znak: '" << c << "' (kod ASCII " << ascii << ")" << endl;
            continue;
        }
        for (int i = 6; i >= 0; --i) {
            int bit = (ascii >> i) & 1;
            bitStream.push_back(bit);
        }
    }

    return bitStream;
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
double find_E(vector<double>& amp) {
    double E = 0;
    for (double a : amp) {
        E += pow(a, 2);
    }
    return E;
}
double find_Ea(vector<double>& freq, vector<double>& amp, double& alpha) {
    int n = amp.size();
    int max_index = 0;
    double max_y = *max_element(amp.begin(), amp.end());
    
    for (int i = 1; i < n; ++i) {
        if (amp[i] == max_y) {
            max_index = i;
        }
    }
    double center_freq = freq[max_index];
    double Ea = 0;
    for (int i = 0; i < n; i++) {
        if (freq[i] >= center_freq - alpha && freq[i] <= center_freq + alpha) {

            Ea += pow(amp[i], 2);
        }
    }
    double fa = center_freq - alpha;
    double fb = center_freq + alpha;
    
    return Ea;
    
}
using namespace std;
int main()
{
    string tekst = "Hi";
   vector<int> bits = stringToBitStream(tekst);

    auto signalASK = generateSignalASK(bits);
    auto signalFSK = generateSignalFSK(bits);
    auto signalPSK = generateSignalPSK(bits);

    auto widmoASK = computeSpectrum(signalASK);
    auto widmoFSK = computeSpectrum(signalFSK);
    auto widmoPSK = computeSpectrum(signalPSK);

    std::vector<double> zA_fs, zA_amp;
    for (const auto& p : widmoASK) {
        zA_fs.push_back(p.first);
        zA_amp.push_back(p.second);
    }

    std::vector<double> zF_fs, zF_amp;
    for (const auto& p : widmoFSK) {
        zF_fs.push_back(p.first);
        zF_amp.push_back(p.second);
    }

    std::vector<double> zP_fs, zP_amp;
    for (const auto& p : widmoPSK) {
        zP_fs.push_back(p.first);
        zP_amp.push_back(p.second);
    }

    // AM
    double E = find_E(zA_amp);
    double alpha_zA = 0;
    double rA = 0;
    double bandwitch_rA=0;
    double Ea = 0;
    while (rA < 0.9) {
        Ea= find_Ea(zA_fs, zA_amp, alpha_zA);
        rA = Ea / E ;
        alpha_zA += 1;
    }

    // PSK
    E = find_E(zP_amp);
    double alpha_zP = 0;
    double rP = 0;
    double bandwitch_rP=0;
    double Ep = 0;
    while (rP < 0.8) {
        Ep= find_Ea(zP_fs, zP_amp, alpha_zP);
        rP = Ep / E;
        alpha_zP += 1;
    }

    // FSK
    E = find_E(zF_amp);
    double alpha_zF = 0;
    double rF = 0;
    double bandwitch_rF=0;
    double Ef = 0;
    while (rF < 0.8) {
        Ef= find_Ea(zF_fs, zF_amp, alpha_zF);
        rF = Ef / E;
        alpha_zF += 1;
    }
    
 
    cout << "Energia:\n";
    cout << "Ra dlugosc:"<< Ea<<"B(AM):" << 2 * (alpha_zA -1) << " Hz\n";
    cout << "RP dlugosc:" << Ep << "B(PM):" << 2 * (alpha_zP -1) << " Hz\n";
    cout << "RF dlugosc:" << Ef << "B(FM):" << 2 * (alpha_zF -1) << " Hz\n";
    cout << "Energia: " << E << endl;
    cout << "-------------------------------------------\n";

    Gnuplot gp;

    gp << "set terminal wxt size 2200,800\n";
    gp << "set multiplot layout 1,3 title 'Widma sygnałów'\n";

    //gp << "set logscale x\n"; // <<<--- logarytmiczna skala częstotliwości (oś X)
    gp << "set grid\n";

    gp << "set title 'Widmo Funkcji x(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
   // gp << "set yrange [0:40]\n";
    //gp << "set xrange [0:100]\n";
   // gp << "set xtics 5\n";

    gp << "plot '-' with lines lw 2 linecolor rgb 'red'\n";
   

    gp.send1d(widmoASK);


    gp << "set title 'Widmo Funkcji y(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";

    gp.send1d(widmoPSK);

    gp << "set title 'Widmo Funkcji z(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'orange'\n";

    gp.send1d(widmoFSK);
    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();
}

