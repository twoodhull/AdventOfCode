#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define PHASE_COUNT 100
#define REPEAT 10000

int main() {
    vector<int> numberSequence;
    string input;

    cin >> input;

    for (int i = 0; i < input.size(); i++) {
        numberSequence.push_back(input[i] - '0');
    }

    int offset = 0;
    for (int i = 0; i < 7; i++) {
        offset = (offset * 10) + numberSequence[i];
    }

    vector<int> realSequence((numberSequence.size() * REPEAT) - offset);
    for (int i = realSequence.size(); i > 0; i--) {
        int negativeIndex = (realSequence.size() - i) % numberSequence.size();

        realSequence[i - 1] = *(numberSequence.rbegin() + negativeIndex);
    }

    for (int i = 0; i < PHASE_COUNT; i++) {
        int sum = 0;

        for (int j = realSequence.size() - 1; j >= 0; j--) {
            sum += realSequence[j];
            realSequence[j] = sum % 10;
        }
    }

    for (int i = 0; i < 8; i++) {
        cout << realSequence[i];
    }
    cout << endl;

    return 0;
}
