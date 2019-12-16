#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define PHASE_COUNT 100
#define PATTERN_SIZE 4

int main() {
    vector<int> numberSequence;
    string input;

    cin >> input;

    for (int i = 0; i < input.size(); i++) {
        numberSequence.push_back(input[i] - '0');
    }

    int pattern[] = {1, -1};

    for (int i = 0; i < PHASE_COUNT; i++) {
        for (int j = 0; j < numberSequence.size(); j++) {
            int result = 0;

            int patternIndex = 0;
            for (int k = j; k < numberSequence.size(); k+=2*(j+1)) {
                for (int l = 0; l <= j && k + l < numberSequence.size(); l++) {
                    result += pattern[patternIndex] * numberSequence[k + l];
                }

                patternIndex = 1 - patternIndex;
            }

            numberSequence[j] = abs(result) % 10;
        }
    }

    for (int i = 0; i < 8; i++) {
        cout << numberSequence[i];
    }
    cout << endl;

    return 0;
}