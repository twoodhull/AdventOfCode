#include <iostream>
#include <deque>
#include <string>
#include <vector>

#define PREAMBLE_SIZE 25

using ll = long long;

using namespace std;

int main() {
    ll invalidNumber;
    vector<ll> sequence;
    deque<ll> preamble;

    string input;
    while (getline(cin, input)) {
        ll val = stoll(input);
        sequence.push_back(val);

        if (preamble.size() < PREAMBLE_SIZE) {
            preamble.emplace_back(val);
            continue;
        }

        bool found = false;
        for (auto i = preamble.begin(); !found && i != preamble.end(); ++i) {
            for (auto j = i + 1; !found && j != preamble.end(); ++j) {
                if (*i + *j == val) {
                    found = true;
                }
            }
        }

        if (!found) {
            invalidNumber = val;
            break;
        }

        preamble.emplace_back(val);
        preamble.pop_front();
    }

    ll rollingSum = sequence[0] + sequence[1];
    ll encryptionWeakness;

    for (int i = 0, j = 1; rollingSum != invalidNumber && i < sequence.size() && j <sequence.size();) {
        if (rollingSum > invalidNumber) {
            rollingSum -= sequence[i];
            i++;
        } else if (rollingSum < invalidNumber) {
            j++;
            rollingSum += sequence[j];
        }
        
        if (rollingSum == invalidNumber) {
            ll minVal = sequence[i];
            ll maxVal = sequence[i];

            for (int k = i; k <= j; k++) {
                minVal = min(minVal, sequence[k]);
                maxVal = max(maxVal, sequence[k]);
            }

            encryptionWeakness = minVal + maxVal;
        }
    }

    
    cout << "First invalid value: " << invalidNumber << endl;
    cout << "Encryption Weakness: " << encryptionWeakness << endl;
}