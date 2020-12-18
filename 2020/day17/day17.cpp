#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <functional>

#include <climits>
#include <algorithm>

using namespace std;

vector<int> deltaArr = {-1, 0, 1};

struct Vec4 {
    int x, y, z, w;

    Vec4(int x, int y, int z) : Vec4(x, y, z, 0) {}
    Vec4(const Vec4& other) : Vec4(other.x, other.y, other.z, other.w) {}
    Vec4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

    bool operator==(const Vec4& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool operator!=(const Vec4& other) const {
        return x != other.x || y != other.y || z != other.z || w != other.w;
    }

    Vec4 operator+(const Vec4& other) const {
        return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }
};

ostream& operator<<(ostream& os, const Vec4& vec) {
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
}

struct VecHash {
    static hash<int> intHasher;

    size_t operator() (const Vec4& vec) const {
        return intHasher(((vec.x * 1000 + vec.y) * 1000 + vec.z) * 1000 + vec.w);
    };
};
hash<int> VecHash::intHasher = hash<int>();

// void printCube(const unordered_set<Vec4, VecHash>& activeCubes) {
//     Vec4 minVals(INT_MAX, INT_MAX, INT_MAX);
//     Vec4 maxVals(INT_MIN, INT_MIN, INT_MIN);

//     for (const Vec4& vec : activeCubes) {
//         minVals.x = min(minVals.x, vec.x);
//         minVals.y = min(minVals.y, vec.y);
//         minVals.z = min(minVals.z, vec.z);

//         maxVals.x = max(maxVals.x, vec.x);
//         maxVals.y = max(maxVals.y, vec.y);
//         maxVals.z = max(maxVals.z, vec.z);
//     }

//     for (int z = minVals.z; z <= maxVals.z; z++) {
//         cout << "z=" << z << endl;
//         for (int y = minVals.y; y <= maxVals.y; y++) {
//             for (int x = minVals.x; x <= maxVals.x; x++) {
//                 cout << (activeCubes.find(Vec4(x, y, z)) == activeCubes.end() ? '.' : '#');
//             }
//             cout << endl;
//         }
//         cout << endl;
//     }
//     cout << endl;
// }

void findActiveCubes(unordered_set<Vec4, VecHash> activeCubes, const vector<Vec4>& deltaVecs) {
    for (int cycle = 0; cycle < 6; cycle++) {
        unordered_map<Vec4, int, VecHash> adjMap;

        for (const Vec4& vec : activeCubes) {
            for (const Vec4& deltaVec : deltaVecs) {
                Vec4 newPos = vec + deltaVec;

                if (adjMap.find(newPos) == adjMap.end()) {
                    adjMap[newPos] = 0;
                }

                if (newPos != vec) {
                    adjMap[newPos]++;
                }
            }
        }

        for (auto it : adjMap) {
            if (activeCubes.find(it.first) == activeCubes.end()) {
                if (it.second == 3) {
                    activeCubes.insert(it.first);
                }
            } else {
                if (it.second != 2 && it.second != 3) {
                    activeCubes.erase(it.first);
                }
            }
        }
    }

    cout << "Active cubes: " << activeCubes.size() << endl;
}

void part1(unordered_set<Vec4, VecHash>& activeCubes) {
    vector<Vec4> deltaVecs;
    for (int dx : deltaArr) {
        for (int dy : deltaArr) {
            for (int dz : deltaArr) {
                deltaVecs.push_back(Vec4(dx, dy, dz));
            }
        }
    }

    findActiveCubes(activeCubes, deltaVecs);
}

void part2(unordered_set<Vec4, VecHash>& activeCubes) {
    vector<Vec4> deltaVecs;
    for (int dx : deltaArr) {
        for (int dy : deltaArr) {
            for (int dz : deltaArr) {
                for (int dw : deltaArr) {
                    deltaVecs.push_back(Vec4(dx, dy, dz, dw));
                }
            }
        }
    }

    findActiveCubes(activeCubes, deltaVecs);
}

int main() {
    unordered_set<Vec4, VecHash> activeCubes;

    string input;
    for (int y = 0; getline(cin, input); y++) {
        for (int x = 0; x < input.length(); x++) {
            if (input[x] == '#') {
                activeCubes.insert(Vec4(x, y, 0));
            }
        }
    }

    part1(activeCubes);
    part2(activeCubes);

    return 0;
}
