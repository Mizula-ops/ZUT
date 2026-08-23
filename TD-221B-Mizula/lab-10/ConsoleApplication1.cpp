//ARTUR MIZUŁA
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "gnuplot-iostream.h"

using namespace std;

vector<int> generateRandomBits(int length) {
    vector<int> bits;
    srand(time(0)); 
    for (int i = 0; i < length; i++) {
        bits.push_back(rand() % 2);  
    }

    return bits;
}

vector<int> hamming(vector<int> bits) {
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
vector<int> decodeStream(const vector<int>& code) {
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
vector<int> introduceErrors(const vector<int>& encoded) {
    vector<int> corrupted = encoded;
    srand(time(0));
    for (size_t i = 0; i + 7 <= corrupted.size(); i += 7) {
        int bitToFlip = rand() % 7; 
        corrupted[i + bitToFlip] ^= 1; 
    }
    return corrupted;
}


int main()
{
    int bitCount = 40;
    vector<int> bits = generateRandomBits(bitCount);
    cout << "CZESC 1" << endl;
    cout << "Prawdziwe bity: " << endl;
    //for (int b : bits) cout << b<<",";
    for (size_t i = 0; i + 4 <= bits.size(); i += 4) {
        vector<int> data(bits.begin() + i, bits.begin() + i + 4);
        cout << data[0] << "," << data[1] << "," << data[2] << "," << data[3] << endl;

    }
    cout << endl;
    vector<int> encoded = hamming(bits);
    cout << "Zakodowane bity: " << endl;
    for (size_t i = 0; i + 7 <= encoded.size(); i += 7) {
        vector<int> data(encoded.begin() + i, encoded.begin() + i + 7);
        cout << data[0] << "," << data[1] << "," << data[2] << "," << data[3] <<","<< data[4] << "," << data[5] << "," << data[6] << "," << endl;

    }
    cout << endl;
    cout << "CZESC 2" << endl;
    cout << endl;
    vector<int> decodedClean = decodeStream(encoded);
    cout << "Dekodowanie bez bledow" << endl;
    for (size_t i = 0; i + 4 <= decodedClean.size(); i += 4) {
        vector<int> data(decodedClean.begin() + i, decodedClean.begin() + i + 4);
        cout << data[0] << "," << data[1] << "," << data[2] << "," << data[3] << endl;

    }
    cout << endl;
    vector<int> corrupted = introduceErrors(encoded);
    cout << "Bity z błedami" << endl;
    for (size_t i = 0; i + 7 <= corrupted.size(); i += 7) {
        vector<int> data(corrupted.begin() + i, corrupted.begin() + i + 7);
        cout << data[0] << "," << data[1] << "," << data[2] << "," << data[3] << "," << data[4] << "," << data[5] << "," << data[6] << "," << endl;

    }
    cout << endl;
    vector<int> decodedCorrected = decodeStream(corrupted);
    cout << "Dekodowanie bity z błedami" << endl;
    for (size_t i = 0; i + 4 <= decodedCorrected.size(); i += 4) {
        vector<int> data(decodedCorrected.begin() + i, decodedCorrected.begin() + i + 4);
        cout << data[0] << "," << data[1] << "," << data[2] << "," << data[3] << endl;

    }
}

