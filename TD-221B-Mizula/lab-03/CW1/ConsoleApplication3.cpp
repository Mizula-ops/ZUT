#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
#include <utility>

#include "gnuplot-iostream.h"
using namespace std;

double f = 1;        
double fs = 16;    
double Tc = 2;       
int N = Tc * fs;     
int H = 10;    

//funckja fft
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

//generowanie signal x
vector<complex<double>> generate_signal_x() {
    vector<complex<double>> signalx(N, 0);
    for (int k = 1; k <= H; k++) {
        for (int n = 0; n < N; n++) {
            double t = n / fs;
            signalx[n] += (2 / M_PI) * pow(-1, k + 1) * (sin(2 * M_PI * k * f * t) / k);
        }
    }
    return signalx;
}

//generowanie signal y
vector<complex<double>> generate_signal_y() {
    vector<complex<double>> signaly(N, 0);
    for (int k = 1; k <= H; k++) {
        for (int n = 0; n < N; n++) {
            double t = n / fs;
            signaly[n] += (8 / pow(M_PI, 2)) * pow(-1, k - 1) * (sin(2 * M_PI * (2 * k - 1) * f * t) / pow(2 * k - 1, 2));
        }
    }
    return signaly;
}

//generowanie signal z
vector<complex<double>> generate_signal_z() {
    vector<complex<double>> signalz(N, 0);
    for (int k = 1; k <= H; k++) {
        for (int n = 0; n < N; n++) {
            double t = n / fs;
            signalz[n] += (4 / M_PI) * (sin(2 * M_PI * (2 * k - 1) * f * t) / (2 * k - 1));
        }
    }
    return signalz;
}



//liczba prazkow
int count_peaks(const vector<pair<double, double>>& spectrum) {
    int peaks = 0;

    for (int i = 1; i < spectrum.size() - 1; i++) {
        double prev = spectrum[i - 1].second;
        double current = spectrum[i].second;
        double next = spectrum[i + 1].second;

      
        if (current > prev && current > next) {
            peaks++;
        }
    }

    return peaks;
}
int main() {
    Gnuplot gp;
 
    vector<complex<double>> x_t = generate_signal_x();
    vector<complex<double>> y_t = generate_signal_y();
    vector<complex<double>> z_t = generate_signal_z();
    vector<complex<double>> x_fft = fft(x_t);
    vector<complex<double>> y_fft = fft(y_t);
    vector<complex<double>> z_fft = fft(z_t);

    vector<pair<double, double>> signal;
    vector<pair<double, double>> signalx1;
    
    vector<pair<double, double>> signalx; 
    vector<pair<double, double>> signaly;
    vector<pair<double, double>> signalz;
    vector<pair<double, complex<double>>> timex; 
    vector<pair<double, complex<double>>> timey;
    vector<pair<double, complex<double>>> timez;

    // Czas 
    for (int k = 0; k < N; k++) {
        double t = k / fs;
        timex.emplace_back(t, x_t[k]);
        timey.emplace_back(t, y_t[k]);
        timez.emplace_back(t, z_t[k]);
    }

    // Widmo 
    for (int k = 0; k < N / 2; k++) {
        double f_k = k * fs / N;
        double x = abs(x_fft[k]);
        double x_db = 10 * log10(x);
        double y = abs(y_fft[k]);
        double z = abs(z_fft[k]);
        signalx.emplace_back(f_k, x_db);
        signaly.emplace_back(f_k, y);
        signalz.emplace_back(f_k, z);
    }

    int prazki_x = count_peaks(signalx);
    int prazki_y = count_peaks(signaly);
    int prazki_z = count_peaks(signalz);

    // Wypisanie liczby prążków
    cout << "Liczba prążków w widmie sygnału x: " << prazki_x << endl;
    cout << "Liczba prążków w widmie sygnału y: " << prazki_y << endl;
    cout << "Liczba prążków w widmie sygnału z: " << prazki_z << endl;

    // x(t) i jego widmo liczba prążków 7
    gp << "set multiplot layout 1,2 title 'Wykresy czasowe i widma - x(t)'\n";
    gp << "set title 'Wykres Funkcji x(t)'\n";
    gp << "set xlabel 'Czas [s]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'x(t)'\n";
    gp.send1d(timex);

    gp << "set title 'Widmo x(t)'\n";
    gp << "set xlabel 'Częstotliwość [Hz]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'Widmo x(t)'\n";
    gp.send1d(signalx);
    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby kontynuować...\n";
    cin.ignore();

    //y(t) i jego widmo liczba prążków 4
    gp << "set multiplot layout 1,2 title 'Wykresy czasowe i widma - y(t)'\n";
    gp << "set title 'Wykres Funkcji y(t)'\n";
    gp << "set xlabel 'Czas [s]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'y(t)'\n";
    gp.send1d(timey);

    gp << "set title 'Widmo y(t)'\n";
    gp << "set xlabel 'Częstotliwość [Hz]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'Widmo y(t)'\n";
    gp.send1d(signaly);
    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby kontynuować...\n";
    cin.ignore();

    //z(t) i jego widmo liczba prążków 4
    gp << "set multiplot layout 1,2 title 'Wykresy czasowe i widma - z(t)'\n";
    gp << "set title 'Wykres Funkcji z(t)'\n";
    gp << "set xlabel 'Czas [s]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'z(t)'\n";
    gp.send1d(timez);

    gp << "set title 'Widmo z(t)'\n";
    gp << "set xlabel 'Częstotliwość [Hz]'\n";
    gp << "set ylabel 'Amplituda'\n";
    gp << "plot '-' with lines title 'Widmo z(t)'\n";
    gp.send1d(signalz);
    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby zakończyć...\n";
    cin.ignore();
    return 0;
   

}
