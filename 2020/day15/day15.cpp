#include <iostream>
#include <cstdio>
#include <regex>
#include <unordered_map>

using namespace std;

int main() {
    unordered_map<int, int> turnMap;
    int turn = 1;

    string input;
    getline(cin, input);

    regex numberPattern("\\d+");
    smatch match;

    while (regex_search(input, match, numberPattern)) {
        int val = stoi(match[0].str());
        turnMap[val] = turn++;
        input = match.suffix().str();
    }

    int cache2020;

    int cur = 0;
    while (turn < 30000000) {
        if (turn == 2020) {
            cache2020 = cur;
        }

        if (turnMap.find(cur) == turnMap.end()) {
            turnMap[cur] = turn++;
            cur = 0;
        } else {
            int next = turn - turnMap[cur];
            turnMap[cur] = turn++;
            cur = next;
        }
    }

    cout << "2020th number: " << cache2020 << endl;
    cout << "30000000th number: " << cur << endl;
}
