#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <omp.h>

using namespace std;

constexpr int  width  = 1000;
constexpr int  height = 1000;
constexpr double xmin = -2.0;
constexpr double xmax =  1.0;
constexpr double ymin = -1.5;
constexpr double ymax =  1.5;
constexpr int  max_iter = 1000;

int main() {
    vector<unsigned char> img(size_t(width) * height * 3u);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {

            double cx = xmin + (double)j / (width  - 1) * (xmax - xmin);
            double cy = ymax - (double)i / (height - 1) * (ymax - ymin);
            complex<double> c(cx, cy);

            complex<double> z(0.0, 0.0);
            int iter = 0;
            while (iter < max_iter && std::norm(z) <= 4.0) { // norm(z) = |z|^2
                z = z*z + c;
                ++iter;
            }

            unsigned char val = (iter == max_iter) ? 0 : 255;
            size_t k = (size_t(i) * width + j) * 3u;
            img[k + 0] = val; // R
            img[k + 1] = val; // G
            img[k + 2] = val; // B


        }
    }

    ofstream out("mandelbrot.ppm", ios::binary);
    out << "P6\n" << width << " " << height << "\n255\n";
    out.write(reinterpret_cast<const char*>(img.data()), (std::streamsize)img.size());
    out.close();

    cout << "Zapisano: mandelbrot.ppm\n";
    return 0;
}
