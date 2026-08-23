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
double fs = 16 * fn2;
double A1 = 0.5;
double A2 = 1;
double t0 = Tc * 0.95;
const int samples_per_bit = static_cast<int>(fs * Tb);

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

vector<vector<int>> encodedHamming1511(const vector<int>& inputBits) {

    vector<vector<int>> helpP = {
        {1, 1, 0, 0},
        {1, 0, 1, 0},
        {0, 1, 1, 0},
        {1, 1, 1, 0},
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {1, 1, 0, 1},
        {0, 0, 1, 1},
        {1, 0, 1, 1},
        {0, 1, 1, 1},
        {1, 1, 1, 1}
    };

    vector<vector<int>> P(11, vector<int>(4, 0));
    for (int i = 0; i < 11; i++)
        for (int j = 0; j < 4; j++)
            P[i][j] = helpP[i][j];

    vector<vector<int>> G(11, vector<int>(15, 0));
    for (int i = 0; i < 11; i++) {
        G[i][i] = 1;
        for (int j = 0; j < 4; j++) {
            G[i][11 + j] = P[i][j];
        }
    }

    vector<vector<int>> C;
    for (int b = 0; b + 11 <= inputBits.size(); b += 11) {
        vector<int> block(inputBits.begin() + b, inputBits.begin() + b + 11);
        vector<int> encoded(15, 0);
        for (int j = 0; j < 15; j++) {
            for (int i = 0; i < 11; i++) {
                encoded[j] += block[i] * G[i][j];
            }
            encoded[j] %= 2;
        }
        C.push_back(encoded);
    }



    return C;
}

vector<int> decodeHamming1511(const vector<int>& received) {
    vector<vector<int>> P = {
        {1, 1, 0, 0},
        {1, 0, 1, 0},
        {0, 1, 1, 0},
        {1, 1, 1, 0},
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {1, 1, 0, 1},
        {0, 0, 1, 1},
        {1, 0, 1, 1},
        {0, 1, 1, 1},
        {1, 1, 1, 1}
    };

    vector<vector<int>> H(4, vector<int>(15, 0));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 11; j++) {
            H[i][j] = P[j][i];
        }
    }
    for (int i = 0; i < 4; i++) {
        H[i][11 + i] = 1;
    }

    vector<int> syndrome(4, 0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 15; j++) {
            syndrome[i] ^= received[j] * H[i][j];
        }
        syndrome[i] %= 2;
    }
    int errorPos = 0;
    for (int i = 0; i < 4; i++) {
        errorPos += syndrome[i] << i;
    }

    vector<int> corrected = received;
    if (errorPos != 0) {
        corrected[errorPos - 1] = 1 - corrected[errorPos - 1];
    }


    vector<int> decoded(received.begin(), received.begin() + 11);
    return decoded;
}

vector<int> hamming7(vector<int> bits) {
    vector<int> result;

    for (size_t i = 0; i + 4 <= bits.size(); i += 4) {

        vector<int> data(bits.begin() + i, bits.begin() + i + 4);
        vector<int> code(8);

        code[3] = data[0];
        code[5] = data[1];
        code[6] = data[2];
        code[7] = data[3];

        code[1] = code[3] ^ code[5] ^ code[7];
        code[2] = code[3] ^ code[6] ^ code[7];
        code[4] = code[5] ^ code[6] ^ code[7];

        result.insert(result.end(), code.begin() + 1, code.end());
    }
    return result;
}

vector<int> decodeHamming7(const vector<int>& code) {
    vector<int> result;
    size_t n = code.size();

    for (size_t i = 0; i + 7 <= n; i += 7) {
        vector<int> block(code.begin() + i, code.begin() + i + 7);
        vector<int> c(8);
        for (int j = 1; j <= 7; ++j) {
            c[j] = block[j - 1];
        }

        int p1 = c[1] ^ c[3] ^ c[5] ^ c[7];
        int p2 = c[2] ^ c[3] ^ c[6] ^ c[7];
        int p3 = c[4] ^ c[5] ^ c[6] ^ c[7];

        int errorPos = p1 * 1 + p2 * 2 + p3 * 4;

        if (errorPos != 0 && errorPos >= 1 && errorPos <= 7) {
            c[errorPos] ^= 1;
        }

        result.push_back(c[3]);
        result.push_back(c[5]);
        result.push_back(c[6]);
        result.push_back(c[7]);
    }
    return result;
}
vector<double> generateWhiteNoise(int length, double alpha) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1.0, 1.0);

    vector<double> noise(length);
    for (int i = 0; i < length; ++i) {
        noise[i] =2* alpha * dis(gen);
    }
    return noise;
}
vector<pair<double, complex<double>>> addNoise(
    const vector<pair<double, complex<double>>>& signal, double alpha)
{
    int length = static_cast<int>(signal.size());
    vector<double> noiseReal = generateWhiteNoise(length, alpha);
    vector<double> noiseImag = generateWhiteNoise(length, alpha);

    vector<pair<double, complex<double>>> noisySignal = signal;
    for (int i = 0; i < length; ++i) {
        noisySignal[i].second += complex<double>(noiseReal[i], noiseImag[i]);
    }
    return noisySignal;
}


double calculateBER(const vector<int>& original, const vector<int>& decoded) {
    int errors = 0;
    int length = min(original.size(), decoded.size());

    for (int i = 0; i < length; ++i) {
        if (original[i] != decoded[i]) errors++;
    }
    return (double)errors / length;
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

vector<int> demodulate(vector<double>& t, vector<double>& z, vector<double>& x, vector<double>& p, vector<double>& c, bool isAsk) {
    x.resize(t.size());
    p.resize(t.size());
    c.resize(t.size());
    //pierwsza faza sygnal x
    if (isAsk == true)
    {
        for (int i = 0; i < t.size(); i++) {
            x[i] = z[i] * sin(2 * M_PI * fn * t[i]);

        }
    }
    else
    {
        for (int i = 0; i < t.size(); i++) {
            x[i] = z[i] * sin(2 * M_PI * fn * t[i] + M_PI);

        }
    }

    vector <double> integrated;
    //drugi etap sydnal p
    vector <double> pomoc(t.size());
    for (size_t i = 0; i < t.size(); i += samples_per_bit) {
        size_t end = min(i + samples_per_bit, x.size());
        double sum = 0;

        for (size_t j = i; j < end; j++) {
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
        for (int i = 0; i < t.size(); i++) {
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
        for (int i = 0; i < t.size(); i++) {
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

vector<int> demodulateFSK(vector<double>& t, vector<double>& z,vector<double>& x1, vector<double>& x2,vector<double>& p1, vector<double>& p2,vector<double>& p_diff, vector<double>& c) {
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
            p1[j] = sum1 / samples_per_bit;
            p2[j] = sum2 / samples_per_bit;
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
using namespace std;
void generateFilter(vector<pair<double, complex<double>>>& signal,double beta)
{
    int numSamples = signal.size();
   // double total = Tb * fs;
    double dt = Tb/ numSamples;
    t0 = 0.95 * numSamples;
    for (int i = 0; i < signal.size(); i++) {
        double t = 50*i * dt;
        signal[i].second *= exp(-beta * t) * max(0.0, 1.0 - t / t0);
    }
}

int main1() {
    vector<int> bits = {
        1,0,1,1,0,1,0,0,1,1,0,
        1,0,0,0,0,1,0,0,1,1,1,
        1,0,1,1,1,0,0,0,1,1,0,
        1,0,0,0,0,1,0,0,0,0,0,
        1,0,1,1,0,1,0,0,1,1,0,
        1,0,0,0,0,1,0,0,1,1,1,
        1,0,1,1,1,0,0,0,1,1,0,
        1,0,0,0,0,1,0,0,0,0,0,
        
    };


    auto encodedBlocks15 = encodedHamming1511(bits);

    vector<int> encodedBits15;
    for (const auto& block : encodedBlocks15) {
        encodedBits15.insert(encodedBits15.end(), block.begin(), block.end());
    }
    //BITY HAMMING7
    auto encodedBits7 = hamming7(bits);
    //SYGNAL HAMMING15
    auto signalASK15 = generateSignalASK(encodedBits15);
    auto signalPSK15 = generateSignalPSK(encodedBits15);
    auto signalFSK15 = generateSignalFSK(encodedBits15);
    //SYGNAL HAMMIN7
    auto signalASK7 = generateSignalASK(encodedBits7);
    auto signalPSK7 = generateSignalPSK(encodedBits7);
    auto signalFSK7 = generateSignalFSK(encodedBits7);

   
    vector<double> alphas = { 0.0, 0.3, 0.6,0.9, 1.2,1.5, 2.0,2.3,2.6, 3.0 };
    vector<pair<double, double>> BER_ASK15, BER_PSK15, BER_FSK15, BER_ASK7, BER_PSK7, BER_FSK7;

    for (double alpha : alphas) {
        //SZUMY HAMMING15
        auto noisySignalASK15 = addNoise(signalASK15, alpha);
        auto noisySignalPSK15 = addNoise(signalPSK15, alpha);
        auto noisySignalFSK15 = addNoise(signalFSK15, alpha);
        //SZUMY HAMMING7
        auto noisySignalASK7 = addNoise(signalASK7, alpha);
        auto noisySignalPSK7 = addNoise(signalPSK7, alpha);
        auto noisySignalFSK7 = addNoise(signalFSK7, alpha);
        //ROZDZIELENIE SYGNALU HAMMING15
        vector<double> tASK15, zASK_real15;
        for (auto& p : noisySignalASK15) {
            tASK15.push_back(p.first);
            zASK_real15.push_back(p.second.real());
        }
        vector<double> tPSK15, zPSK_real15;
        for (auto& p : noisySignalPSK15) {
            tPSK15.push_back(p.first);
            zPSK_real15.push_back(p.second.real());
        }
        vector<double> tFSK15, zFSK_real15;
        for (auto& p : noisySignalFSK15) {
            tFSK15.push_back(p.first);
            zFSK_real15.push_back(p.second.real());
        }
        //ROZDZIELENIE SYGNALU HAMMING7
        vector<double> tASK7, zASK_real7;
        for (auto& p : noisySignalASK7) {
            tASK7.push_back(p.first);
            zASK_real7.push_back(p.second.real());
        }
        vector<double> tPSK7, zPSK_real7;
        for (auto& p : noisySignalPSK7) {
            tPSK7.push_back(p.first);
            zPSK_real7.push_back(p.second.real());
        }
        vector<double> tFSK7, zFSK_real7;
        for (auto& p : noisySignalFSK7) {
            tFSK7.push_back(p.first);
            zFSK_real7.push_back(p.second.real());
        }
        //DEMODULACJA HAMMING15
        vector<double> x15a, p15a, c15a, x15p, p15p, c15p;
        auto demodASK_bits15 = demodulate(tASK15, zASK_real15, x15a, p15a, c15a, true);
        auto demodPSK_bits15 = demodulate(tPSK15, zPSK_real15, x15p, p15p, c15p, false);
        vector<double> x115, x215, p115, p215, p_diff15, cFSK15;
        auto demodFSK_bits15 = demodulateFSK(tFSK15, zFSK_real15, x115, x215, p115, p215, p_diff15, cFSK15);
        //DEMODULACJA HAMMING7
        vector<double> x, p, c, xp, pp, cp;
        auto demodASK_bits7 = demodulate(tASK7, zASK_real7, x, p, c, true);
        auto demodPSK_bits7 = demodulate(tPSK7, zPSK_real7, xp, pp, cp, false);

        vector<double> x1, x2, p1, p2, p_diff, cFSK;
        auto demodFSK_bits7 = demodulateFSK(tFSK7, zFSK_real7, x1, x2, p1, p2, p_diff, cFSK);

        //DEKODOWANIE HAMMING15
        vector<int> decodedASK15, decodedPSK15, decodedFSK15;

        for (size_t i = 0; i + 15 <= demodASK_bits15.size(); i += 15) {
            vector<int> block(demodASK_bits15.begin() + i, demodASK_bits15.begin() + i + 15);
            auto decodedBlock = decodeHamming1511(block);
            decodedASK15.insert(decodedASK15.end(), decodedBlock.begin(), decodedBlock.end());
        }

        for (size_t i = 0; i + 15 <= demodPSK_bits15.size(); i += 15) {
            vector<int> block(demodPSK_bits15.begin() + i, demodPSK_bits15.begin() + i + 15);
            auto decodedBlock = decodeHamming1511(block);
            decodedPSK15.insert(decodedPSK15.end(), decodedBlock.begin(), decodedBlock.end());
        }

        for (size_t i = 0; i + 15 <= demodFSK_bits15.size(); i += 15) {
            vector<int> block(demodFSK_bits15.begin() + i, demodFSK_bits15.begin() + i + 15);
            auto decodedBlock = decodeHamming1511(block);
            decodedFSK15.insert(decodedFSK15.end(), decodedBlock.begin(), decodedBlock.end());
        }
        //DEKODOWANIE HAMMING7
        auto decodedASK7 = decodeHamming7(demodASK_bits7);
        auto decodedPSK7 = decodeHamming7(demodPSK_bits7);
        auto decodedFSK7 = decodeHamming7(demodFSK_bits7);

        //BER DLA HAMMING15
        double berASK15 = calculateBER(bits, decodedASK15);
        double berPSK15 = calculateBER(bits, decodedPSK15);
        double berFSK15 = calculateBER(bits, decodedFSK15);

        BER_ASK15.emplace_back(alpha, berASK15);
        BER_PSK15.emplace_back(alpha, berPSK15);
        BER_FSK15.emplace_back(alpha, berFSK15);

        //BER DLA HAMMING7
        double berASK7 = calculateBER(bits, decodedASK7);
        double berPSK7 = calculateBER(bits, decodedPSK7);
        double berFSK7 = calculateBER(bits, decodedFSK7);

        BER_ASK7.emplace_back(alpha, berASK7);
        BER_PSK7.emplace_back(alpha, berPSK7);
        BER_FSK7.emplace_back(alpha, berFSK7);
    }

    cout << "Alpha\tBER_ASK\tBER_PSK\tBER_FSK\n";
    for (size_t i = 0; i < alphas.size(); ++i) {
        cout << alphas[i] << "\t"
            << BER_ASK15[i].second << "\t"
            << BER_PSK15[i].second << "\t"
            << BER_FSK15[i].second << "\n";
    }
    cout << endl;
    cout << "Alpha\tBER_ASK\tBER_PSK\tBER_FSK\n";
    for (size_t i = 0; i < alphas.size(); ++i) {
        cout << alphas[i] << "\t"
            << BER_ASK7[i].second << "\t"
            << BER_PSK7[i].second << "\t"
            << BER_FSK7[i].second << "\n";
    }
    Gnuplot gp;

    gp << "set terminal wxt size 1200,600 title 'BER vs alpha'\n";
    gp << "set multiplot layout 1,2 title 'BER vs α dla różnych kodów i modulacji'\n";

    gp << "set grid\n";
    gp << "set title 'Hamming(15,11)' font ',14'\n";
    gp << "set xlabel 'α' font ',12'\n";
    gp << "set ylabel 'BER' font ',12'\n";
    gp << "set xrange [0:3]\n";
    gp << "plot "
        "'-' with linespoints lt 1 pt 7 lw 2 title 'ASK', "
        "'-' with linespoints lt 2 pt 5 lw 2 title 'PSK', "
        "'-' with linespoints lt 3 pt 9 lw 2 title 'FSK'\n";
    gp.send1d(BER_ASK15);
    gp.send1d(BER_PSK15);
    gp.send1d(BER_FSK15);

    gp << "set grid\n";
    gp << "set title 'Hamming(7,4)' font ',14'\n";
    gp << "set xlabel 'α' font ',12'\n";
    gp << "set ylabel 'BER' font ',12'\n";
    gp << "set xrange [0:3]\n";


    gp << "plot "
        "'-' with linespoints lt 1 pt 7 lw 2 title 'ASK', "
        "'-' with linespoints lt 2 pt 5 lw 2 title 'PSK', "
        "'-' with linespoints lt 3 pt 9 lw 2 title 'FSK'\n";
    gp.send1d(BER_ASK7);
    gp.send1d(BER_PSK7);
    gp.send1d(BER_FSK7);

    gp << "unset multiplot\n";



    return 0;
}


vector<int> bity(int N) {
    vector<int> bity(N);
    for (int i = 0; i < N; i++) {
        bity[i] = rand() % 2;
    }
    return bity;
};
int main() {
    auto bits = bity(4400);


    auto encodedBlocks15 = encodedHamming1511(bits);

    vector<int> encodedBits15;
    for (const auto& block : encodedBlocks15) {
        encodedBits15.insert(encodedBits15.end(), block.begin(), block.end());
    }
    //BITY HAMMING7
    auto encodedBits7 = hamming7(bits);
    //SYGNAL HAMMING15
    auto signalASK15 = generateSignalASK(encodedBits15);
    auto signalPSK15 = generateSignalPSK(encodedBits15);
    auto signalFSK15 = generateSignalFSK(encodedBits15);
    //SYGNAL HAMMIN7
    auto signalASK7 = generateSignalASK(encodedBits7);
    auto signalPSK7 = generateSignalPSK(encodedBits7);
    auto signalFSK7 = generateSignalFSK(encodedBits7);

    
    vector<pair<double, double>> BER_ASK15, BER_PSK15, BER_FSK15, BER_ASK7, BER_PSK7, BER_FSK7;
    vector<double> betas = { 0.0,25,50,75,100,125,150,175,200};


    for (double beta : betas) {
            
            generateFilter(signalASK15, beta);
            generateFilter(signalPSK15, 0.7*beta);
            generateFilter(signalFSK15, 5*beta);

            generateFilter(signalASK7, beta);
            generateFilter(signalPSK7, beta);
            generateFilter(signalFSK7, 2.5*beta);
         
            //ROZDZIELENIE SYGNALU HAMMING15
            vector<double> tASK15, zASK_real15;
            for (auto& p : signalASK15) {
                tASK15.push_back(p.first);
                zASK_real15.push_back(p.second.real());
            }
            vector<double> tPSK15, zPSK_real15;
            for (auto& p : signalPSK15) {
                tPSK15.push_back(p.first);
                zPSK_real15.push_back(p.second.real());
            }
            vector<double> tFSK15, zFSK_real15;
            for (auto& p : signalFSK15) {
                tFSK15.push_back(p.first);
                zFSK_real15.push_back(p.second.real());
            }
            //ROZDZIELENIE SYGNALU HAMMING7
            vector<double> tASK7, zASK_real7;
            for (auto& p : signalASK7) {
                tASK7.push_back(p.first);
                zASK_real7.push_back(p.second.real());
            }
            vector<double> tPSK7, zPSK_real7;
            for (auto& p : signalPSK7) {
                tPSK7.push_back(p.first);
                zPSK_real7.push_back(p.second.real());
            }
            vector<double> tFSK7, zFSK_real7;
            for (auto& p : signalFSK7) {
                tFSK7.push_back(p.first);
                zFSK_real7.push_back(p.second.real());
            }
            //DEMODULACJA HAMMING15
            vector<double> x15a, p15a, c15a, x15p, p15p, c15p;
            auto demodASK_bits15 = demodulate(tASK15, zASK_real15, x15a, p15a, c15a, true);
            auto demodPSK_bits15 = demodulate(tPSK15, zPSK_real15, x15p, p15p, c15p, false);
            vector<double> x115, x215, p115, p215, p_diff15, cFSK15;
            auto demodFSK_bits15 = demodulateFSK(tFSK15, zFSK_real15, x115, x215, p115, p215, p_diff15, cFSK15);
            //DEMODULACJA HAMMING7
            vector<double> x, p, c, xp, pp, cp;
            auto demodASK_bits7 = demodulate(tASK7, zASK_real7, x, p, c, true);
            auto demodPSK_bits7 = demodulate(tPSK7, zPSK_real7, xp, pp, cp, false);

            vector<double> x1, x2, p1, p2, p_diff, cFSK;
            auto demodFSK_bits7 = demodulateFSK(tFSK7, zFSK_real7, x1, x2, p1, p2, p_diff, cFSK);

            //DEKODOWANIE HAMMING15
            vector<int> decodedASK15, decodedPSK15, decodedFSK15;

            for (size_t i = 0; i + 15 <= demodASK_bits15.size(); i += 15) {
                vector<int> block(demodASK_bits15.begin() + i, demodASK_bits15.begin() + i + 15);
                auto decodedBlock = decodeHamming1511(block);
                decodedASK15.insert(decodedASK15.end(), decodedBlock.begin(), decodedBlock.end());
            }

            for (size_t i = 0; i + 15 <= demodPSK_bits15.size(); i += 15) {
                vector<int> block(demodPSK_bits15.begin() + i, demodPSK_bits15.begin() + i + 15);
                auto decodedBlock = decodeHamming1511(block);
                decodedPSK15.insert(decodedPSK15.end(), decodedBlock.begin(), decodedBlock.end());
            }

            for (size_t i = 0; i + 15 <= demodFSK_bits15.size(); i += 15) {
                vector<int> block(demodFSK_bits15.begin() + i, demodFSK_bits15.begin() + i + 15);
                auto decodedBlock = decodeHamming1511(block);
                decodedFSK15.insert(decodedFSK15.end(), decodedBlock.begin(), decodedBlock.end());
            }
            //DEKODOWANIE HAMMING7
            auto decodedASK7 = decodeHamming7(demodASK_bits7);
            auto decodedPSK7 = decodeHamming7(demodPSK_bits7);
            auto decodedFSK7 = decodeHamming7(demodFSK_bits7);

            //BER DLA HAMMING15
            double berASK15 = calculateBER(bits, decodedASK15);
            double berPSK15 = calculateBER(bits, decodedPSK15);
            double berFSK15 = calculateBER(bits, decodedFSK15);

            BER_ASK15.emplace_back(beta, berASK15);
            BER_PSK15.emplace_back(beta, berPSK15);
            BER_FSK15.emplace_back(beta, berFSK15);

            //BER DLA HAMMING7
            double berASK7 = calculateBER(bits, decodedASK7);
            double berPSK7 = calculateBER(bits, decodedPSK7);
            double berFSK7 = calculateBER(bits, decodedFSK7);

            BER_ASK7.emplace_back(beta, berASK7);
            BER_PSK7.emplace_back(beta, berPSK7);
            BER_FSK7.emplace_back(beta, berFSK7);
     }

        cout << "Alpha\tBER_ASK\tBER_PSK\tBER_FSK\n";
        for (size_t i = 0; i < betas.size(); ++i) {
            cout << betas[i] << "\t"
                << BER_ASK15[i].second << "\t"
                << BER_PSK15[i].second << "\t"
                << BER_FSK15[i].second << "\n";
        }
        cout << endl;
        cout << "Alpha\tBER_ASK\tBER_PSK\tBER_FSK\n";
        for (size_t i = 0; i < betas.size(); ++i) {
            cout << betas[i] << "\t"
                << BER_ASK7[i].second << "\t"
                << BER_PSK7[i].second << "\t"
                << BER_FSK7[i].second << "\n";
        }
        Gnuplot gp;

        gp << "set terminal wxt size 1200,600 title 'BER vs alpha'\n";
        gp << "set multiplot layout 1,2 title 'BER vs α dla różnych kodów i modulacji'\n";

        gp << "set grid\n";
        gp << "set title 'Hamming(15,11)' font ',14'\n";
        gp << "set xlabel 'α' font ',12'\n";
        gp << "set ylabel 'BER' font ',12'\n";
       
        gp << "plot "
            "'-' with linespoints lt 1 pt 7 lw 2 title 'ASK', "
            "'-' with linespoints lt 2 pt 5 lw 2 title 'PSK', "
            "'-' with linespoints lt 3 pt 9 lw 2 title 'FSK'\n";
        gp.send1d(BER_ASK15);
        gp.send1d(BER_PSK15);
        gp.send1d(BER_FSK15);

        gp << "set grid\n";
        gp << "set title 'Hamming(7,4)' font ',14'\n";
        gp << "set xlabel 'α' font ',12'\n";
        gp << "set ylabel 'BER' font ',12'\n";
     


        gp << "plot "
            "'-' with linespoints lt 1 pt 7 lw 2 title 'ASK', "
            "'-' with linespoints lt 2 pt 5 lw 2 title 'PSK', "
            "'-' with linespoints lt 3 pt 9 lw 2 title 'FSK'\n";
        gp.send1d(BER_ASK7);
        gp.send1d(BER_PSK7);
        gp.send1d(BER_FSK7);

        gp << "unset multiplot\n";



        return 0;
    
}




