//Tabela 1 funckja nr1
//tabela 2 funckja nr2
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include "gnuplot-iostream.h"
using namespace std;

double f = 2000;
double fs = 8000;
double Tc = 1;
int N = Tc * fs;
double o = M_PI / 2;


void generate_signal(vector<pair<double, double>>& x_signal,
    vector<pair<double, double>>& y_signal,
    vector<pair<double, double>>& z_signal,
    vector<pair<double, double>>& v_signal) {

    for (int n = 0; n < N; n++) {
        double t = n / fs;


        double x = cos(2 * M_PI * f * t + o) * cos(2.5 * pow(t, 0.2) * M_PI);

        double y = (x * t) / 3 + cos(20 * M_PI * t);
        double z = pow(t, 2) * abs(x * y - 2 / (10 + y));
        double v = pow(z, 3) + 3 * sin(z * y) * abs(y - x);

        x_signal.emplace_back(t, x);
        y_signal.emplace_back(t, y);
        z_signal.emplace_back(t, z);
        v_signal.emplace_back(t, v);
    }
}


int main() {
    Gnuplot gp;
    vector<pair<double, double>> x_signal, y_signal, z_signal, v_signal, u_signal;


    generate_signal(x_signal, y_signal, z_signal, v_signal);

    gp << "set multiplot layout 2,2 title 'Wykresy sygnalow'\n";


    gp << "set title 'x(t)'\n";
    gp << "set ytics 0.4\n";
    gp << "set xtics 0.4\n";
    gp << "plot '-' using 1:2 with lines title ''\n";
    gp.send1d(x_signal);

    gp << "set title 'y(t)'\n";
    gp << "set ytics 0.4\n";
    gp << "set xtics 0.4\n";
    gp << "plot '-' using 1:2 with lines title ''\n";
    gp.send1d(y_signal);

    gp << "set title 'z(t)'\n";
    gp << "set ytics 0.2\n";
    gp << "set xtics 0.2\n";
    gp << "plot '-' using 1:2 with lines title ''\n";
    gp.send1d(z_signal);


    gp << "set title 'v(t)'\n";
    gp << "set ytics 0.4\n";
    gp << "set ytics 0.4\n";
    gp << "plot '-' using 1:2 with lines title ''\n";
    gp.send1d(v_signal);

    gp << "unset multiplot\n";
  

}



