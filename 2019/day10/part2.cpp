#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cmath>
#include <algorithm>

using namespace std;

#define PI 3.14159265358979323846

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

struct Asteroid {
    pair<int, int> position;
    pair<int, int> slope;
    int distance;

    Asteroid(const pair<int, int>& position, const pair<int, int>& slope, int distance) : position(position), slope(slope), distance(distance) {}
};

double getAngle(const pair<int, int>& slope) {
    double angle = atan2(slope.first, slope.second);

    if (angle < 0) {
        angle = -angle + PI / 2;
    } else {
        if (angle > PI / 2) {
            angle = 2 * PI - (angle - PI / 2);
        } else {
            angle = (PI / 2) - angle;
        }
    }

    return angle;
}

bool compareAsteroids(const Asteroid& a, const Asteroid& b) {
    if (a.slope == b.slope) {
        return a.distance < b.distance;
    }

    return getAngle(a.slope) < getAngle(b.slope);
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

    vector<Asteroid> asteroids;
    pair<int, int> best;
    size_t maxVal = 0;
    for (int i = 0; i < points.size(); i++) {
        set<pair<int, int>> unique;
        vector<Asteroid> astTemp;

        for (int j = 0; j < points.size(); j++) {
            if (i == j) {
                continue;
            }

            int rise = points[i].second - points[j].second;
            int run = points[j].first - points[i].first;

            pair<int, int> slope = simplify(pair<int, int>(rise, run));
            unique.insert(slope);

            astTemp.push_back(Asteroid(points[j], slope, abs(rise) + abs(run)));
        }

        if (unique.size() > maxVal) {
            asteroids = astTemp;
            maxVal = unique.size();
            best = points[i];
        }
    }

    sort(asteroids.begin(), asteroids.end(), compareAsteroids);

    vector<bool> used(asteroids.size(), false);
    int count = 0;
    while (count < asteroids.size()) {
        set<pair<int, int>> unique;
        
        for (int i = 0; i < asteroids.size(); i++) {
            if (used[i]) {
                continue;
            }

            if (unique.find(asteroids[i].slope) != unique.end()) {
                continue;
            }

            count++;
            used[i] = true;
            unique.insert(asteroids[i].slope);

            cout << "Position " << count << ": (" << asteroids[i].position.first << ", " << asteroids[i].position.second << ")" << endl;
        }
    }

    return 0;
}