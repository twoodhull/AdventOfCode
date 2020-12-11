#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;

#define MAX_JOLT_DIFF 3

vector<ll> memo;

ll getArrangements(int index, const vector<int>& jolts) {
    if (index == jolts.size() - 1) {
        return 1L;
    }

    if (memo[index] != -1L) {
        return memo[index];
    }


    ll ret = 0;
    for (int i = index + 1; i < jolts.size() && jolts[i] - jolts[index] <= MAX_JOLT_DIFF; i++) {
        ret += getArrangements(i, jolts);
    }

    return memo[index] = ret;
}

int main() {
    vector<int> joltDiffs(MAX_JOLT_DIFF + 1, 0);
    vector<int> jolts;
    int maxVal = 0;

    string input;
    while (getline(cin, input)) {
        jolts.push_back(stoi(input));
    }

    jolts.push_back(0);
    sort(jolts.begin(), jolts.end());
    jolts.push_back(jolts.back() + 3);
    
    memo = vector<ll>(jolts.size(), -1L);

    for (int i = 0, last = 0; i < jolts.size(); i++) {
        joltDiffs[jolts[i] - last]++;
        last = jolts[i];
    }


    cout << joltDiffs[1] << " * " << joltDiffs[3] << " = " << (joltDiffs[1] * joltDiffs[3]) << endl;
    cout << "Total Arrangements: " << getArrangements(0, jolts) << endl;

    return 0;
}