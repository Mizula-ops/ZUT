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

double W = 16;
double B = 10;
double Tc = 0.3;
double Tb = Tc / B;
double fn = W / Tb;
double fn2 = (W + 2) / Tb;
double fs = 16 * fn2;
double A1 = 0.5;
double A2 = 1;
const int samples_per_bit = static_cast<int>(fs * Tb);

vector<pair<double, complex<double>>> generateSignalASK(const vector<int>& bits) {
    vector<pair<double, complex<double>>> time;

    for (size_t i = 0; i < bits.size(); ++i) {
        double bitStart = i * Tb;
        for (int j = 0; j < samples_per_bit; ++j) {
            double t = bitStart + j / fs;
            double amplitude = bits[i] == 0 ? A1 : A2;
            complex<double> value = amplitude * sin(2 * M_PI * fn * t);
            time.emplace_back(t, value);
        }
    }

    return time;
}

vector<pair<double, complex<double>>> generateSignalPSK(const vector<int>& bits) {
    vector<pair<double, complex<double>>> time;

    for (size_t i = 0; i < bits.size(); ++i) {
        double bitStart = i * Tb;
        for (int j = 0; j < samples_per_bit; ++j) {
            double t = bitStart + j / fs;
            double phase = bits[i] == 0 ? 0 : M_PI;
            complex<double> value = 1.0 * sin(2 * M_PI * fn * t + phase);
            time.emplace_back(t, value);
        }
    }

    return time;
}

vector<int> demodulate(vector<double>& t, vector<double>& z, vector<double>& x, vector<double>& p, vector<double>& c,bool isAsk) {
    x.resize(t.size());
    p.resize(t.size());
    c.resize(t.size());
    //pierwsza faza sygnal x
    if (isAsk == true)
    {
        for (int i = 0;i < t.size();i++) {
            x[i] = z[i] * sin(2 * M_PI * fn * t[i]);

        }
    }
    else
    {
        for (int i = 0;i < t.size();i++) {
            x[i] = z[i] * sin(2 * M_PI * fn * t[i] +M_PI);

        }
    }
    
    vector <double> integrated;
    //drugi etap sydnal p
    vector <double> pomoc(t.size());
    for (size_t i = 0;i < t.size();i+=samples_per_bit ) {
        size_t end = min(i + samples_per_bit, x.size());
        double sum = 0;

        for (size_t j = i;j < end;j++) {
            sum += x[j];
            p[j] = sum / samples_per_bit;

        }
        double avg = sum / samples_per_bit;
        for (size_t j = i; j < end; j++) {
            pomoc[j] = avg;
        }
        integrated.push_back(p[end - 1]);
    }
    //trzeci etap sygnal c
    vector<int> bits;
    if (isAsk == true)
    {
        double threshold = (A1 + A2) / 4;
        for (int i = 0;i < t.size();i++) {
            if (pomoc[i] > threshold)
                c[i] = 1;
            else
                c[i] = 0;
        }
    
        for (double val : integrated) {
            bits.push_back(val > threshold ? 1 : 0);
        }
    }
    else
    {
        for (int i = 0;i < t.size();i++) {
            if (pomoc[i] > 0)
                c[i] = 1;
            else
                c[i] = 0;
        }
      
        for (double val : integrated) {
            bits.push_back(val > 0 ? 1 : 0);
        }
    }
    
    return bits;

}
    
double calculateBER(const vector<int>& originalBits, const vector<int>& demodulatedBits) {
    int errors = 0;
    for (size_t i = 0; i < originalBits.size(); ++i) {
        if (originalBits[i] != demodulatedBits[i]) {
            errors++;
        }
    }
    double ber = static_cast<double>(errors) / originalBits.size();
    return ber * 100;  
}


int main() {
    vector<int> bits = { 0, 1, 0, 0, 1 };
    //SYGNAL ASK
    auto signalASK = generateSignalASK(bits);
    
    std::vector<double> zA_fs, zA_amp;
    for (const auto& p : signalASK) {
        zA_fs.push_back(p.first);
        zA_amp.push_back(p.second.real());
    }
    
    vector<double> x_A, p_A, c_A;
    auto demodulatedASK = demodulate(zA_fs,zA_amp,x_A,p_A,c_A,true);
    //auto demodulatedPSK = demodulatePSK(signalPSK);
    vector<pair<double, double>> signal_xA;
    for (size_t i = 0; i < x_A.size(); ++i) {
        signal_xA.emplace_back(zA_fs[i], x_A[i]);
    }
    vector<pair<double, double>> signal_pA;
    for (size_t i = 0; i < x_A.size(); ++i) {
        signal_pA.emplace_back(zA_fs[i], p_A[i]);
    }
    vector<pair<double, double>> signal_cA;
    for (size_t i = 0; i < x_A.size(); ++i) {
        signal_cA.emplace_back(zA_fs[i], c_A[i]);
    }
    //SYGNAL PSK
    auto signalPSK = generateSignalPSK(bits);
    std::vector<double> zP_fs, zP_amp;
    for (const auto& p : signalPSK) {
        zP_fs.push_back(p.first);
        zP_amp.push_back(p.second.real());
    }
    vector<double> x_P, p_P, c_P;
    auto demodulatedPSK = demodulate(zP_fs, zP_amp, x_P, p_P, c_P, false);
    //auto demodulatedPSK = demodulatePSK(signalPSK);
    vector<pair<double, double>> signal_xP;
    for (size_t i = 0; i < x_P.size(); ++i) {
        signal_xP.emplace_back(zA_fs[i], x_P[i]);
    }
    vector<pair<double, double>> signal_pP;
    for (size_t i = 0; i < x_P.size(); ++i) {
        signal_pP.emplace_back(zA_fs[i], p_P[i]);
    }
    vector<pair<double, double>> signal_cP;
    for (size_t i = 0; i < x_P.size(); ++i) {
        signal_cP.emplace_back(zA_fs[i], c_P[i]);
    }
    cout << "bity ASK: ";
    for (auto& bit : demodulatedASK) {
        cout << bit;
    }
    cout << endl;
    cout << "bity PSK: ";
    for (auto& bit : demodulatedPSK) {
        cout << bit;
    }
    cout << endl;
    //OSTATNI KROK
    vector<pair<int, int>> bits_vs_demodASK;
    vector<pair<int, int>> bits_vs_demodPSK;
    for (size_t i = 0; i < bits.size(); ++i) {
        bits_vs_demodASK.emplace_back(i, bits[i]);
        bits_vs_demodASK.emplace_back(i, demodulatedASK[i] + 2); // przesunięcie dla wizualizacji

        bits_vs_demodPSK.emplace_back(i, bits[i]);
        bits_vs_demodPSK.emplace_back(i, demodulatedPSK[i] + 2); // przesunięcie dla wizualizacji
    }

    Gnuplot gp;
    
 
    gp << "set terminal wxt size 2200,800\n";
    gp << "set multiplot layout 1,3 title 'Widma sygnałów'\n";

    gp << "set grid\n";
    gp << "set title 'Widmo Funkcji x(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    // gp << "set yrange [0:40]\n";
     //gp << "set xrange [0:100]\n";
    // gp << "set xtics 5\n";

    gp << "plot '-' with lines lw 2 linecolor rgb 'red'\n";
    gp.send1d(signal_xA);


    gp << "set title 'Widmo Funkcji y(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";

    gp.send1d(signal_pA);

    gp << "set title 'Widmo Funkcji z(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'orange'\n";

    gp.send1d(signal_cA);
    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();

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


    gp.send1d(signal_xP);


    gp << "set title 'Widmo Funkcji y(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";

    gp.send1d(signal_pP);

    gp << "set title 'Widmo Funkcji z(t)' font ',14'\n";
    gp << "set xlabel 'Częstotliwość [Hz]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'orange'\n";

    gp.send1d(signal_cP);
    gp << "unset multiplot\n";

    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();

    gp << "set terminal wxt size 1400,400\n";
    gp << "set multiplot layout 1,2 title 'Porównanie sygnału z demodulowanym'\n";
    gp << "set grid\n";

    gp << "set title 'Sygnal ASK'\n";
    gp << "set xlabel 'Numer bitu'\n";
    gp << "set ylabel 'Wartość bitu'\n";
    gp << "set ytics (\"0\" 0, \"1\" 1, \"0 (dem)\" 2, \"1 (dem)\" 3)\n";
    gp << "plot '-' using 1:2 with points pointtype 7 pointsize 1.5 linecolor rgb 'red'\n";
    gp.send1d(bits_vs_demodASK);

    gp << "set title 'Sygnal PSK'\n";
    gp << "set xlabel 'Numer bitu'\n";
    gp << "set ylabel 'Wartość bitu'\n";
    gp << "set ytics (\"0\" 0, \"1\" 1, \"0 (dem)\" 2, \"1 (dem)\" 3)\n";
    gp << "plot '-' using 1:2 with points pointtype 7 pointsize 1.5 linecolor rgb 'green'\n";
    gp.send1d(bits_vs_demodPSK);

    gp << "unset multiplot\n";
    cout << "Wciśnij Enter, aby zakończyć...\n";
    cin.ignore();

}

