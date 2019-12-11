#include <iostream>
#include <vector>
#include <string>
#include <set>

using namespace std;

int gcd(int a, int b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }

    if (a == b) {
        return a;
    }

    return a > b ? gcd(a - b, b) : gcd(a, b - a);
}

pair<int, int> simplify(pair<int, int> fraction) {
    int fracGCD;
    int num = fraction.first;
    int den = fraction.second;

    while ((fracGCD = gcd(abs(num), abs(den))) != 1) {
        num /= fracGCD;
        den /= fracGCD;
    }

    return pair<int, int>(num, den);
}

int main() {
    vector<string> map;
    string input;

    while (getline(cin, input)) {
        map.push_back(input);
    }

    vector<pair<int, int>> points;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].length(); j++) {
            if (map[i][j] == '.') {
                continue;
            }

            points.push_back(pair<int, int>(j, i));
        }
    }

    pair<int, int> best;
    size_t maxVal = 0;
    for (int i = 0; i < points.size(); i++) {
        set<pair<int, int>> unique;

        for (int j = 0; j < points.size(); j++) {
            if (i == j) {
                continue;
            }

            int rise = points[j].second - points[i].second;
            int run = points[j].first - points[i].first;

            pair<int, int> val = simplify(pair<int, int>(rise, run));
            unique.insert(val);
        }

        if (unique.size() > maxVal) {
            maxVal = unique.size();
            best = points[i];
        }
    }

    cout << maxVal << endl;
    cout << best.first << ", " << best.second << endl;
    return 0;
}