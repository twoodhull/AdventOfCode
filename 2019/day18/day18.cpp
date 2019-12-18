#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct State {
    int keyMask;
    int steps;
    vector<char> at;

    State() = delete;
    State(const vector<char>& at) :
        keyMask(0),
        steps(0),
        at(at)
    {}
    State(const State& other) :
        keyMask(other.keyMask),
        steps(other.steps),
        at(other.at)
    {}

    bool operator<(const State& other) const {
        return steps > other.steps;
    }

    bool operator==(const State& other) const {
        bool isEqual = true;
        for (int i = 0; i < at.size() && isEqual; i++) {
            isEqual = at[i] == other.at[i];
        }

        return isEqual && keyMask == other.keyMask;
    }

    void addKey(char c) {
        c = tolower(c);
        keyMask |= (1 << (c - 'a'));
    }

    bool hasKey(char c) const {
        c = tolower(c);
        return ((1 << (c - 'a')) & keyMask) != 0;
    }
};

struct Hasher {
    size_t operator()(const State& state) const {
        size_t charHash = 0;

        for (char c : state.at) {
            charHash = charHash * 31 + hash<char>()(c);
        }

        return hash<int>()(state.keyMask) * 31 + charHash;
    }

    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        return hash<T1>()(p.first) * 31 + hash<T2>()(p.second);
    }
};

struct StepKeyPair {
    int steps;
    int requiredKeys;
};

bool inBounds(const pair<int, int>& position, const vector<string>& caveMap) {
    return position.first >= 0 && position.second >= 0 && position.first < caveMap.size() && position.second < caveMap[position.first].length();
}

int main() {
    int vaultIndex = 0;
    vector<string> caveMap;
    vector<char> keys;
    unordered_map<char, pair<int, int>> positions;
    string input;

    int allKeyMask = 0;

    while (getline(cin, input)) {
        caveMap.push_back(input);

        for (int j = 0; j < input.length(); j++) {
            if (input[j] == '@') {
                caveMap.back()[j] = '0' + vaultIndex;
                positions['0' + vaultIndex] = make_pair(caveMap.size() - 1, j);
                vaultIndex++;
            }

            if (input[j] >= 'a' && input[j] <= 'z') {
                allKeyMask |= 1 << (input[j] - 'a');
                positions[input[j]] = make_pair(caveMap.size() - 1, j);
                keys.push_back(input[j]);
            }
        }
    }

    unordered_map<pair<char, char>, StepKeyPair, Hasher> stepMap;

    int dirCount = 4;
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};

    for (auto it : positions) {
        queue<int> q;
        q.push(it.second.first);
        q.push(it.second.second);
        q.push(0);
        q.push(0);

        vector<vector<bool>> visited(caveMap.size(), vector<bool>(caveMap[0].length(), false));
        visited[it.second.first][it.second.second] = true;

        while (!q.empty()) {
            int y = q.front();
            q.pop();
            int x = q.front();
            q.pop();
            int steps = q.front();
            q.pop();
            int requiredKeys = q.front();
            q.pop();

            if ('A' <= caveMap[y][x] && caveMap[y][x] <= 'Z') {
                requiredKeys |= (1 << (caveMap[y][x] - 'A'));
            }

            if (caveMap[y][x] != it.first && 'a' <= caveMap[y][x] && caveMap[y][x] <= 'z') {
                StepKeyPair& r = stepMap[make_pair(it.first, caveMap[y][x])];
                r.steps = steps;
                r.requiredKeys = requiredKeys;
                continue;
            }

            for (int dir = 0; dir < dirCount; dir++) {
                int yy = y + dy[dir];
                int xx = x + dx[dir];

                if (!inBounds(make_pair(yy, xx), caveMap) || caveMap[yy][xx] == '#') {
                    continue;
                }

                if (visited[yy][xx]) {
                    continue;
                }
                visited[yy][xx] = true;

                q.push(yy);
                q.push(xx);
                q.push(steps + 1);
                q.push(requiredKeys);
            }
        }
    }

    priority_queue<State> q;
    unordered_map<State, int, Hasher> visitedMap;
    vector<char> at = {'0', '1', '2', '3'};
    q.push(State(at));

    int minSteps = INT_MAX;

    while (!q.empty()) {
        State cur = q.top();
        q.pop();

        auto it = visitedMap.find(cur);
        if (it != visitedMap.end() && it->second <= cur.steps) {
            continue;
        }
        visitedMap[cur] = cur.steps; 

        if (cur.keyMask == allKeyMask) {
            minSteps = cur.steps;
            break;
        }

        for (int i = 0; i < cur.at.size(); i++) {
            char at = cur.at[i];

            for (char c : keys) {
                if (c == at) {
                    continue;
                }

                auto it = stepMap.find(make_pair(at, c));
                if (it == stepMap.end()) {
                    continue;
                }

                StepKeyPair stepKeyPair = it->second;

                if ((cur.keyMask & stepKeyPair.requiredKeys) != stepKeyPair.requiredKeys) { 
                    continue;
                }

                State next(cur);
                next.at[i] = c;
                next.steps += stepKeyPair.steps;
                next.addKey(c);

                auto nextIt = visitedMap.find(next);
                if (nextIt != visitedMap.end() && nextIt->second <= next.steps) {
                    continue;
                }

                q.push(next);
            }
        }
    }

    cout << "Minimum steps: " << minSteps << endl;

    return 0;
}