#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <climits>
#include <algorithm>

using namespace std;

vector<string> splitString(const string& str) {
    vector<string> ret;

    size_t pointer = 0;
    while (pointer < str.length()) {
        size_t nextSpace = str.find(' ', pointer);

        if (nextSpace == string::npos) {
            ret.push_back(str.substr(pointer));
            break;
        } else {
            ret.push_back(str.substr(pointer, nextSpace - pointer));
        }

        pointer = nextSpace + 1;
    }

    return ret;
}

int findOptimalHappiness(vector<int>& perm, int used, int index, const map<pair<int, int>, int>& happinessMap) {
    if (index == perm.size()) {
        int ret = 0;

        for (int i = 0; i < perm.size(); i++) {
            int left = perm[(perm.size() + i - 1) % perm.size()];
            int right = perm[(i + 1) % perm.size()];

            ret += happinessMap.at(make_pair(perm[i], left)) + happinessMap.at(make_pair(perm[i], right));
        }

        return ret;
    }

    int maxVal = INT_MIN;
    for (int i = 0; i < perm.size(); i++) {
        if ((used & (1 << i)) != 0) {
            continue;
        }

        perm[index] = i;
        maxVal = max(maxVal, findOptimalHappiness(perm, used | (1 << i), index + 1, happinessMap));
    }

    return maxVal;
}

int main() {
    int nameIndex = 0;
    map<string, int> nameIndexMap;
    map<pair<int, int>, int> happinessMap;

    string line;
    while (getline(cin, line)) {
        vector<string> tokens = splitString(line);
        string name1 = tokens[0];
        string name2 = tokens.back().substr(0, tokens.back().length() - 1);

        int p1, p2;
        auto nameIt1 = nameIndexMap.find(name1);
        auto nameIt2 = nameIndexMap.find(name2);

        if (nameIt1 == nameIndexMap.end()) {
            p1 = nameIndex++;
            nameIndexMap[name1] = p1;
        } else {
            p1 = nameIt1->second;
        }
        if (nameIt2 == nameIndexMap.end()) {
            p2 = nameIndex++;
            nameIndexMap[name2] = p2;
        } else {
            p2 = nameIt2->second;
        }

        int happiness = stoi(tokens[3]);
        if (tokens[2] == "lose") {
            happiness = -happiness;
        }
        happinessMap[make_pair(p1, p2)] = happiness;
    }

    vector<int> permBuffer(nameIndex);
    cout << "Part 1: " << findOptimalHappiness(permBuffer, 0, 0, happinessMap) << endl;

    for (int i = 0; i < nameIndex; i++) {
        happinessMap[make_pair(nameIndex, i)] = 0;
        happinessMap[make_pair(i, nameIndex)] = 0;
    }
    permBuffer.push_back(0);
    cout << "Part 2: " << findOptimalHappiness(permBuffer, 0, 0, happinessMap) << endl;

    return 0;
}