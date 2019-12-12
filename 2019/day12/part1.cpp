#include <iostream>
#include <vector>
#include <regex>
#include <string>

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

    int getPotentialEnergy() const {
        return abs(position.x) + abs(position.y) + abs(position.z);
    }

    int getKineticEnergy() const {
        return abs(velocity.x) + abs(velocity.y) + abs(velocity.z);
    }

    int getEnergy() const {
        return getPotentialEnergy() * getKineticEnergy();
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

        moons.push_back(moon);
    }

    for (int i = 0; i < TIME_STEPS; i++) {
        for (int j = 0; j < moons.size(); j++) {
            for (int k = j + 1; k < moons.size(); k++) {
                Moon::alterVelocities(moons[j], moons[k]);
            }
        }

        for (Moon& m : moons) {
            m.step();
        }
    }

    int totalEnergy = 0;
    for (Moon& m : moons) {
        cout << m << endl;

        totalEnergy += m.getEnergy();
    }

    cout << totalEnergy << endl;

    return 0;
}