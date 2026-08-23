#ARTUR MIZUŁA

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
double fn = W/Tb;
double fn1 = (W + 1) / Tb;
double fn2 = (W + 2) / Tb;
double fs = 8*fn2;
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
            complex<double> value = 1.0* sin(2 * M_PI * fn * t +pi);
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
    for (size_t i = 0; i < N/2 ; ++i) { 
        double freq = i*fs/N;
        double magnitude = abs(spectrum[i]);
        double magnitude_dB =10*log10(magnitude+1e-12); 
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

int main() {
    
    string text = "2"; 
    /*
    vector<int> bits(30);
    for (int i = 0;i < 30;i++)
    {
        if (i < 10 || i>19) bits[i] = 0;
        else bits[i] = 1;
    }
    */
    string tekst = "Hello";

    vector<int> bits = stringToBitStream(tekst);
   
    auto signalASK=generateSignalASK(bits);
    auto signalFSK = generateSignalFSK(bits);
    auto signalPSK = generateSignalPSK(bits);

    auto widmoASK = computeSpectrum(signalASK);
    auto widmoFSK = computeSpectrum(signalFSK);
    auto widmoPSK = computeSpectrum(signalPSK);
    
    Gnuplot gp;
    gp << "set terminal wxt size 2200,800\n";
    gp << "set multiplot layout 1,3 title 'Widma sygnałów'\n";

    gp << "set title 'Widmo Funkcji x(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'red'\n";
 
    gp.send1d(signalASK);
    
    
    gp << "set title 'Widmo Funkcji y(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";

    gp.send1d(signalFSK);

    gp << "set title 'Widmo Funkcji z(t)' font ',14'\n";
     gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'orange'\n";

    gp.send1d(signalPSK);
    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();

    
    gp << "set terminal wxt size 2200,800\n";
    gp << "set multiplot layout 1,3 title 'Widma sygnałów'\n";

    gp << "set logscale x\n"; // <<<--- logarytmiczna skala częstotliwości (oś X)
    gp << "set grid\n";

    gp << "set title 'Widmo Funkcji x(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'red'\n";

    gp.send1d(widmoASK);


    gp << "set title 'Widmo Funkcji y(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";

    gp.send1d(widmoFSK);

    gp << "set title 'Widmo Funkcji z(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'orange'\n";

    gp.send1d(widmoPSK);
    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();
    return 0;
}