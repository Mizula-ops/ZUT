//Z tabeli 3 wybrana funckja nr 1
//Z tabeli 4 wybrana funckja nr 1
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include "gnuplot-iostream.h"
using namespace std;


double fs = 22050;
double Tc = 1;
int N = Tc * fs;
double o = M_PI / 2;

void generate_signal1(vector<pair<double, double>>& u_signal)
{

    for (int n = 0; n < N; n++) {
        double t = n / fs;
        double u = 0;
        if (0.1 > t && t >= 0)
            u = sin(6 * M_PI * t) * cos(5 * M_PI * t);
        else if (0.4 > t && t >= 0.1)
            u = -1.1 * t * cos(41 * M_PI * pow(t, 2));
        else if (0.72 > t && t >= 0.4)
            u = -t * sin(20 * pow(t, 4));
        else if (1 > t && t >= 0.72)
            u = 3.3 * (t - 0.72) * cos(27 * t + 1.3);

        u_signal.emplace_back(t, u);

    }
}
double b_k(double t, int k) {
    double sum = 0;
    for (int h = 1;h <= k;h++) {
        sum += pow(-1, h) / h * sin(h * M_PI * 2 * t);
    }
    return (2 / M_PI) * sum;
}
void generate_signal2(vector<pair<double, double>>& b5_signal,
    vector<pair<double, double>>& b20_signal,
    vector<pair<double, double>>& b50_signal)
{

    for (int n = 0; n < N; n++) {
        double t = n / fs;
        b5_signal.emplace_back(t, b_k(t, 5));
        b20_signal.emplace_back(t, b_k(t, 20));
        b50_signal.emplace_back(t, b_k(t, 50));

    }
}


int main() {
    Gnuplot gp;
    vector<pair<double, double>> u_signal, b5_signal, b20_signal, b50_signal;


    generate_signal1(u_signal);
    generate_signal2(b5_signal, b20_signal, b50_signal);

    gp << "set multiplot layout 2,2 title 'Wykresy sygnalow'\n";


    gp << "set title 'u(t)'\n";
    gp << "set ytics 0.4\n";
    gp << "set xtics 0.4\n";
    gp << "plot '-' using 1:2 with lines title ''\n";
    gp.send1d(u_signal);

    gp << "set title 'b5(t)'\n";
    gp << "set ytics 0.4\n";
    gp << "set xtics 0.4\n";
    gp << "plot '-' using 1:2 with lines title ''\n";
    gp.send1d(b5_signal);

    gp << "set title 'b20(t)'\n";
    gp << "set ytics 0.4\n";
    gp << "set xtics 0.4\n";
    gp << "plot '-' using 1:2 with lines title ''\n";
    gp.send1d(b20_signal);


    gp << "set title 'b50(t)'\n";
    gp << "set ytics 0.4\n";
    gp << "set ytics 0.4\n";
    gp << "plot '-' using 1:2 with lines title ''\n";
    gp.send1d(b50_signal);

    gp << "unset multiplot\n";

}



