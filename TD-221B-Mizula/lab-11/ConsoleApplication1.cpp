//ARTUR MIZUŁA
#include <iostream>
#include <vector>
#include <bitset>
#include <cstdlib>
#include <ctime>

using namespace std;

vector<vector<int>> encodedHamming(const vector<int>& inputBits) {

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

vector<int> decodeHamming(const vector<int>& received) {
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
vector<int> introduceError(const vector<int>& encoded) {
    vector<int> corrupted = encoded;
    srand(time(nullptr));
    int blockIndex = rand() % (corrupted.size() / 15);
    int bitIndex = blockIndex * 15 + (rand() % 15);
    corrupted[bitIndex] = 1 - corrupted[bitIndex];
    return corrupted;
}
int main() {
    vector<int> input = {
        1,0,1,1,0,1,0,0,1,1,0,
        1,0,0,0,0,1,0,0,1,1,1,
        1,0,1,1,1,0,0,0,1,1,0,
        1,0,0,0,0,1,0,0,0,0,0,
        1,0,1,0,0,0,0,0,0,0,0,
        1,0,0,0,0,1,0,0,0,0,0,
        1,0,0,0,0,1,0,0,0,0,1,
     
    };

    auto encodedBlocks = encodedHamming(input);

    vector<int> encoded;
    for (auto& block : encodedBlocks) {
        encoded.insert(encoded.end(), block.begin(), block.end());
    }
    vector<int> decoded_no_error;
    for (size_t i = 0; i + 15 <= encoded.size(); i += 15) {
        vector<int> block(encoded.begin() + i, encoded.begin() + i + 15);
        auto decodedBlock = decodeHamming(block);
        decoded_no_error.insert(decoded_no_error.end(), decodedBlock.begin(), decodedBlock.end());
    }

    auto corrupted = introduceError(encoded);
    vector<int> decoded_with_correction;
    for (size_t i = 0; i + 15 <= corrupted.size(); i += 15) {
        vector<int> block(corrupted.begin() + i, corrupted.begin() + i + 15);
        auto decodedBlock = decodeHamming(block);
        decoded_with_correction.insert(decoded_with_correction.end(), decodedBlock.begin(), decodedBlock.end());
    }

    cout << "Oryginalne dane:\n";
    for (size_t i = 0; i < input.size(); i++) {
        cout << input[i];
        if ((i + 1) % 11 == 0) cout << '\n';
    }

    cout << "\nZakodowane dane:\n";
    for (size_t i = 0; i < encoded.size(); i++) {
        cout << encoded[i];
        if ((i + 1) % 15 == 0) cout << '\n';
    }

    cout << "\nZdekodowane bez bledu:\n";
    for (size_t i = 0; i < decoded_no_error.size(); i++) {
        cout << decoded_no_error[i];
        if ((i + 1) % 11 == 0) cout << '\n';
    }

    cout << "\nZdekodowane z korekcją bledu:\n";
    for (size_t i = 0; i < decoded_with_correction.size(); i++) {
        cout << decoded_with_correction[i];
        if ((i + 1) % 11 == 0) cout << '\n';
    }

    return 0;
}
