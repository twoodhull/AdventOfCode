#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include <string>
#include <climits>

using namespace std;

using ll = long long;

ll modInverse(ll a, ll m) {
    ll m0 = m;
    ll x0 = 0, x1 = 1;
    ll t, q;

    if (m == 1) {
        return 0;
    }

    while (a > 1) {
        q = a / m;
        t = m;

        m = a % m;
        a = t;

        t = x0;

        x0 = x1 - q * x0;

        x1 = t;
    }

    if (x1 < 0) {
        x1 += m0;
    }

    return x1;
}

ll crt(const vector<ll>& remainders, const vector<ll>& numbers) {
    ll prod = 1;
    for (ll i : numbers) {
        prod *= i;
    }

    ll res = 0;
    for (int i = 0; i < numbers.size(); i++) {
        ll pp = prod / numbers[i];
        res += remainders[i] * modInverse(pp, numbers[i]) * pp;
    }

    return res % prod;
}

int main() {
    string input;
    getline(cin, input);

    ll timestamp = stoll(input);
    ll earliestTime = LLONG_MAX;
    ll earliestBusId = -1;

    regex inputPattern("\\d+|x");
    smatch match;
    vector<ll> busIds;
    vector<ll> remainders;

    ll constraint = 0;
    getline(cin, input);
    while (regex_search(input, match, inputPattern)) {
        if (match[0] != "x") {
            busIds.push_back(stoll(match[0]));

            int i = busIds.back();
            while (i < constraint) i += busIds.back();
            remainders.push_back((i - constraint) % busIds.back());

            ll catchTime = timestamp + ((busIds.back() - (timestamp % busIds.back())) % busIds.back());
            if (catchTime < earliestTime) {
                earliestTime = catchTime;
                earliestBusId = busIds.back();
            }
        }

        input = match.suffix().str();
        constraint++;
    }

    cout << earliestBusId << " * " << (earliestTime - timestamp) << " = " << (earliestBusId * (earliestTime - timestamp)) << endl;
    cout << "Part 2: " << crt(remainders, busIds) << endl;

    return 0;
}
