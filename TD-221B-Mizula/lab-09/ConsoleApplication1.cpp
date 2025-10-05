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

double W = 16;
double B = 10;
double Tc = 0.3;
double Tb = Tc / B;
double fn1 = W / Tb;
double fn2 = 1.2 *W/ Tb;
double fs = 16 * fn2;
double A1 = 1;
double A2 = 1;
const int samples_per_bit = static_cast<int>(fs * Tb);

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
vector<int> demodulate(vector<double>& t, vector<double>& z,
    vector<double>& x1, vector<double>& x2,
    vector<double>& p1, vector<double>& p2,
    vector<double>& p_diff, vector<double>& c) {
    x1.resize(t.size());
    x2.resize(t.size());
    p1.resize(t.size());
    p2.resize(t.size());
    p_diff.resize(t.size());
    c.resize(t.size());  

    for (int i = 0; i < t.size(); i++) {
        x1[i] = z[i] * sin(2 * M_PI * fn1 * t[i]);
        x2[i] = z[i] * sin(2 * M_PI * fn2 * t[i]);
    }

    vector<int> bits;

    for (size_t i = 0; i < t.size(); i += samples_per_bit) {
        double sum1 = 0, sum2 = 0;
        size_t end = min(i + samples_per_bit, t.size());

        for (size_t j = i; j < end; j++) {
            sum1 += x1[j];
            sum2 += x2[j];
            p1[j] = sum1/samples_per_bit;
            p2[j] = sum2/ samples_per_bit;
            p_diff[j] = p2[j] - p1[j];
        }

        double integ1 = sum1 / fs;
        double integ2 = sum2 / fs;
        int bit = (integ2 - integ1) > 0 ? 1 : 0;
        bits.push_back(bit);

        for (size_t j = i; j < end; j++) {
         
            c[j] = bit;  
        }
    }

    return bits;
}


double calculateBER(vector<int>& originalBits, vector<int>& demodulatedBits) {
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

    auto signalFSK = generateSignalFSK(bits);

    std::vector<double> zF_fs, zF_amp;
    for (const auto& p : signalFSK) {
        zF_fs.push_back(p.first);
        zF_amp.push_back(p.second.real());
    }

    vector<double> x1,x2,p1,p2, p_diff, c;
    auto demodulatedFSK = demodulate(zF_fs, zF_amp, x1, x2, p1, p2, p_diff, c);

   
    vector<pair<double, double>> signalx1,signalx2,signalp1,signalp2,signalp_diff,signalc;
 
    for (size_t i = 0; i < zF_fs.size(); ++i) {
        signalx1.emplace_back(zF_fs[i], x1[i]);
        signalx2.emplace_back(zF_fs[i], x2[i]);
        signalp1.emplace_back(zF_fs[i], p1[i]);
        signalp2.emplace_back(zF_fs[i], p2[i]);
        signalp_diff.emplace_back(zF_fs[i], p_diff[i]);
        signalc.emplace_back(zF_fs[i], c[i]);
    }
   
  
    cout << "bity ASK: ";
    for (auto& bit : demodulatedFSK) {
        cout << bit;
    }
    cout << endl;
    cout << "bity Orginalne: ";
    for (auto& bit : bits) {
        cout << bit;
    }

    cout << endl;

    vector<pair<int, int>> bits_vs_demodASK;
    for (size_t i = 0; i < bits.size(); ++i) {
        bits_vs_demodASK.emplace_back(i, bits[i]);
        bits_vs_demodASK.emplace_back(i, demodulatedFSK[i] + 2);

    }

    Gnuplot gp;

    gp << "set title 'Sygnal FSK' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";
    // gp << "set yrange [0:40]\n";
     //gp << "set xrange [0:100]\n";
    // gp << "set xtics 5\n";
    gp.send1d(signalFSK);

    gp << "unset multiplot\n";
    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();

    gp << "set terminal wxt size 1800,600\n";
    gp << "set multiplot layout 1,2 title 'Widma sygnałów'\n";
    gp << "set grid\n";

    gp << "set title 'Sygnal x1' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";
    gp.send1d(signalx1);


    gp << "set title 'Sygnal x2' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";
    gp.send1d(signalx2);

    gp << "unset multiplot\n";
    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();


    gp << "set terminal wxt size 1800,600\n";
    gp << "set multiplot layout 1,2 title 'Widma sygnałów'\n";
    gp << "set grid\n";

    gp << "set title 'Sygnal p1' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";
    gp.send1d(signalp1);


    gp << "set title 'Sygnal p2' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'green'\n";
    gp.send1d(signalp2);

    gp << "unset multiplot\n";
    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();

    gp << "set grid\n";
    gp << "set title 'Sygnal d)' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";

    gp << "plot '-' with lines lw 2 linecolor rgb 'red'\n";
    gp.send1d(signalp_diff);
    gp << "unset multiplot\n";
    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();

    gp << "set grid\n";
    gp << "set title 'Sygnal c' font ',14'\n";
    gp << "set xlabel 'Czas [s]' font ',12'\n";
    gp << "set ylabel 'Amplituda' font ',12'\n";
    gp << "plot '-' with lines lw 2 linecolor rgb 'red'\n";
    gp.send1d(signalc);
    gp << "unset multiplot\n";
    cout << "Wciśnij Enter, aby kontynuować do kolejnego zestawu parametrów...\n";
    cin.ignore();


    gp << "set title 'Sygnal ASK'\n";
    gp << "set xlabel 'Numer bitu'\n";
    gp << "set ylabel 'Wartość bitu'\n";
    gp << "set ytics (\"0\" 0, \"1\" 1, \"0 (dem)\" 2, \"1 (dem)\" 3)\n";
    gp << "plot '-' using 1:2 with points pointtype 7 pointsize 1.5 linecolor rgb 'red'\n";
    gp.send1d(bits_vs_demodASK);

    gp << "unset multiplot\n";
    cout << "Wciśnij Enter, aby zakończyć...\n";
    cin.ignore();
    
}

