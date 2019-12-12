#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <map>
#include <set>
#include <climits>

using namespace std;

#define TIME_STEPS 1000

struct Vec3 {
    int x, y, z;

    Vec3() : Vec3(0, 0, 0) {};
    Vec3(int x, int y, int z) : x(x), y(y), z(y) {};

    Vec3 operator+(const Vec3& other) {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
};

struct Moon {
    Vec3 position;
    Vec3 velocity;

    map<pair<int, int>, long long> xHistory;
    map<pair<int, int>, long long> yHistory;
    map<pair<int, int>, long long> zHistory;

    int getPotentialEnergy() const {
        return abs(position.x) + abs(position.y) + abs(position.z);
    }

    int getKineticEnergy() const {
        return abs(velocity.x) + abs(velocity.y) + abs(velocity.z);
    }

    int getEnergy() const {
        return getPotentialEnergy() * getKineticEnergy();
    }

    void logHistory(long long step) {
        xHistory[pair<int, int>(position.x, velocity.x)] = step;
        yHistory[pair<int, int>(position.y, velocity.y)] = step;
        zHistory[pair<int, int>(position.z, velocity.z)] = step;
    }

    vector<long long> getHistory() {
        auto xIt = xHistory.find(pair<int, int>(position.x, velocity.x));
        auto yIt = yHistory.find(pair<int, int>(position.y, velocity.y));
        auto zIt = zHistory.find(pair<int, int>(position.z, velocity.z));

        vector<long long> ret = {
            xIt != xHistory.end() ? xIt->second : -1,
            yIt != yHistory.end() ? yIt->second : -1,
            zIt != zHistory.end() ? zIt->second : -1,
        };
        return ret;
    }

    void step() {
        position += velocity;
    }

    static void alterVelocities(Moon& a, Moon& b) {
        if (a.position.x < b.position.x) {
            a.velocity.x++;
            b.velocity.x--;
        } else if (a.position.x > b.position.x) {
            a.velocity.x--;
            b.velocity.x++;
        }

        if (a.position.y < b.position.y) {
            a.velocity.y++;
            b.velocity.y--;
        } else if (a.position.y > b.position.y) {
            a.velocity.y--;
            b.velocity.y++;
        }

        if (a.position.z < b.position.z) {
            a.velocity.z++;
            b.velocity.z--;
        } else if (a.position.z > b.position.z) {
            a.velocity.z--;
            b.velocity.z++;
        }
    }
};

ostream& operator<<(ostream& os, const Vec3& vec) {
    return os << "<x= " << vec.x << ", y= " << vec.y << ", z= " << vec.z << ">";
}

ostream& operator<<(ostream& os, const Moon& moon) {
    return os << "pos=" << moon.position << ", vel=" << moon.velocity << ", energy=" << moon.getEnergy();
}

long long gcd(long long a, long long b) {
    if (a == 0) {
        return b;
    }

    return gcd(b % a, a);
}

long long lcm(long long a, long long b) {
    return (a / gcd(a, b)) * b;
}

int main() {
    vector<Moon> moons;
    string input;

    regex number("-?\\d+");

    while (getline(cin, input)) {
        Moon moon;

        sregex_iterator it(input.begin(), input.end(), number);
        moon.position.x = stoi((it++)->str(0));
        moon.position.y = stoi((it++)->str(0));
        moon.position.z = stoi((it++)->str(0));

        moon.logHistory(0);

        moons.push_back(moon);
    }

    long long steps = 0;
    long long xSteps0, xSteps1 = -1;
    long long ySteps0, ySteps1 = -1;
    long long zSteps0, zSteps1 = -1;

    while (true) {
        for (int j = 0; j < moons.size(); j++) {
            for (int k = j + 1; k < moons.size(); k++) {
                Moon::alterVelocities(moons[j], moons[k]);
            }
        }
        steps++;

        set<long long> xHistory;
        set<long long> yHistory;
        set<long long> zHistory;

        for (Moon& m : moons) {
            m.step();

            vector<long long> repeats = m.getHistory();
            xHistory.insert(repeats[0]);
            yHistory.insert(repeats[1]);
            zHistory.insert(repeats[2]);

            m.logHistory(steps);
        }

        if (xSteps1 < 0 && xHistory.size() == 1 && xHistory.find(-1) == xHistory.end()) {
            cout << "X: " << steps << endl;
            xSteps0 = *(xHistory.begin());
            xSteps1 = steps;
        }
        if (ySteps1 < 0 && yHistory.size() == 1 && yHistory.find(-1) == yHistory.end()) {
            cout << "Y: " << steps << endl;
            ySteps0 = *(yHistory.begin());
            ySteps1 = steps;
        }
        if (zSteps1 < 0 && zHistory.size() == 1 && zHistory.find(-1) == zHistory.end()) {
            cout << "Z: " << steps << endl;
            zSteps0 = *(zHistory.begin());
            zSteps1 = steps;
        }

        if (xSteps1 > 0 && ySteps1 > 0 && zSteps1 > 0) {
            long long minVal = lcm(xSteps1 - xSteps0, lcm(ySteps1 - ySteps0, zSteps1 - zSteps0));

            cout << minVal << endl << endl;
            break;
        }
    }

    return 0;
}