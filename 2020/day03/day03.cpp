#include <iostream>
#include <string>
#include <vector>

using namespace std;

long long getTreeCount(const vector<string>& map, int deltaX, int deltaY) {
    long long treeCount = 0;
    for (int y = 0, x = 0; y < map.size(); y += deltaY, x += deltaX) {
        if (map[y][x % map[y].length()] == '#') {
            treeCount++;
        }
    }

    return treeCount;
}

int main() {
    vector<string> map;

    string input;
    while(getline(cin, input)) {
        map.push_back(input);
    }

    long long part2TreeCount = getTreeCount(map, 1, 1)
        * getTreeCount(map, 3, 1)
        * getTreeCount(map, 5, 1)
        * getTreeCount(map, 7, 1)
        * getTreeCount(map, 1, 2);

    cout << "Part 1 Tree Count: " << getTreeCount(map, 3, 1) << endl;
    cout << "Part 2 Tree Count: " << part2TreeCount << endl;

    return 0;
}
