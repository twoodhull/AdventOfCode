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
            ret.push_back(str.substr(pointer, nextSpace - pointer + 1));
        }

        pointer = nextSpace + 1;
    }

    return ret;
}

pair<int, int> findDist(vector<int>& perm, int used, int index, const map<pair<int, int>, int>& distanceMap) {
    if (index == perm.size()) {
        int ret = 0;

        for (int i = 0; i < perm.size() - 1; i++) {
            ret += distanceMap.at(make_pair(perm[i], perm[i+1]));
        }

        return make_pair(ret, ret);
    }

    int minVal = INT_MAX;
    int maxVal = INT_MIN;
    for (int i = 0; i < perm.size(); i++) {
        if ((used & (1 << i)) != 0) {
            continue;
        }

        perm[index] = i;
        pair<int, int> distPair = findDist(perm, used | (1 << i), index + 1, distanceMap);
        minVal = min(minVal, distPair.first);
        maxVal = max(maxVal, distPair.second);
    }

    return make_pair(minVal, maxVal);
}

int main() {
    int locationIndex = 0;
    map<string, int> locationIndexMap;
    map<pair<int, int>, int> distanceMap;

    string line;
    while (getline(cin, line)) {
        vector<string> tokens = splitString(line);

        int loc1, loc2;
        auto it1 = locationIndexMap.find(tokens[0]);
        auto it2 = locationIndexMap.find(tokens[2]);

        if (it1 == locationIndexMap.end()) {
            loc1 = locationIndex++;
            locationIndexMap[tokens[0]] = loc1;
        } else {
            loc1 = it1->second;
        }
        if (it2 == locationIndexMap.end()) {
            loc2 = locationIndex++;
            locationIndexMap[tokens[2]] = loc2;
        } else {
            loc2 = it2->second;
        }

        int distance = stoi(tokens.back());
        distanceMap[make_pair(loc1, loc2)] = distance;
        distanceMap[make_pair(loc2, loc1)] = distance;
    }

    vector<int> permBuffer(locationIndex);
    pair<int, int> distPair = findDist(permBuffer, 0, 0, distanceMap);
    cout << "Part 1: " << distPair.first << endl;
    cout << "Part 2: " << distPair.second << endl;

    return 0;
}