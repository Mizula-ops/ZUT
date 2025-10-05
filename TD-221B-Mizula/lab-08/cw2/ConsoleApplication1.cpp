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

vector<int> demodulate(vector<double>& t, vector<double>& z,bool isAsk,double threshold) {
    vector<double> x(t.size());
    vector<double> p(t.size());
    vector<double> c(t.size());
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
            x[i] = z[i] * sin(2 * M_PI * fn * t[i] + M_PI);

        }
    }

    vector <double> integrated;
    //drugi etap sydnal p
    vector <double> pomoc(t.size());
    for (size_t i = 0;i < t.size();i += samples_per_bit) {
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
            if (pomoc[i] > threshold)
                c[i] = 1;
            else
                c[i] = 0;
        }

        for (double val : integrated) {
            bits.push_back(val > threshold ? 1 : 0);
        }
    }

    return bits;

}
double calculateBER(vector<int>& originalBits, vector<int>& demodulatedBits) {
    double errors = 0;
    for (size_t i = 0; i < originalBits.size(); ++i) {
        if (originalBits[i] != demodulatedBits[i]) {
            errors++;
        }
        //cout << errors << endl;
    }
    double ber = errors / originalBits.size();
   
    return ber ;
}
int main()
{
    vector<int> bits = { 0, 1, 0, 0, 1};
    for (int i = 1; i < 4;i++)
    {
        double threshold = (A1 + A2) / (2 * i);
            //SYGNAL ASK
        auto signalASK = generateSignalASK(bits);
        std::vector<double> zA_fs, zA_amp;
        for (const auto& p : signalASK) {
            zA_fs.push_back(p.first);
            zA_amp.push_back(p.second.real());
        }
        auto demodulatedASK = demodulate(zA_fs, zA_amp, true, threshold);
        //SYGNAL PSK
        auto signalPSK = generateSignalPSK(bits);
        std::vector<double> zP_fs, zP_amp;
        for (const auto& p : signalPSK) {
            zP_fs.push_back(p.first);
            zP_amp.push_back(p.second.real());
        }
        auto demodulatedPSK = demodulate(zP_fs, zP_amp, false, threshold);
        cout << "Orginalne bity: ";
        for (auto& bit : bits) {
            cout << bit;
        }
        cout << endl;
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
       

        cout << "Dla ASK rowne h: " << threshold << " BER wynosi: " << calculateBER(bits, demodulatedASK) << endl;
        cout << "Dla PSK rowne h: " << threshold << " BER wynosi: " << calculateBER(bits, demodulatedPSK) << endl;
    }
    
}

