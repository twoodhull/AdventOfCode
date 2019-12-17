#include <iostream>
#include <set>
#include <string>

using namespace std;

ostream& operator<<(ostream& os, const pair<int, int>& p) {
    os << "(" << p.first << ", " << p.second << ")";
}

int main() {
    set<pair<int, int>> santaVisited;
    set<pair<int, int>> roboVisited;

    string directions;
    cin >> directions;

    pair<int, int> santaPosition(0, 0);
    pair<int, int> roboPosition(0, 0);
    santaVisited.insert(santaPosition);
    roboVisited.insert(roboPosition);

    for (int i = 0; i < directions.length(); i++) {
        pair<int, int>* positionPointer = &santaPosition;

        if (i % 2 == 1) {
            positionPointer = &roboPosition;
        }

        switch (directions[i]) {
            case 'v':
                positionPointer->second--;
                break;
            case '>':
                positionPointer->first++;
                break;
            case '^':
                positionPointer->second++;
                break;
            case '<':
                positionPointer->first--;
                break;
            default:
                break;
        }

        santaVisited.insert(santaPosition);
        roboVisited.insert(roboPosition);
    }

    cout << "Santa Visited: " << santaVisited.size() << endl;

    set<pair<int, int>> unionSet;
    unionSet.insert(santaVisited.begin(), santaVisited.end());
    unionSet.insert(roboVisited.begin(), roboVisited.end());

    cout << "Total Visited: " << unionSet.size() << endl;

    return 0;
}
