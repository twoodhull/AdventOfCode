#include <iostream>
#include <string>
#include <climits>
#include <algorithm>
#include <unordered_set>

using namespace std;

int main() {
    string input;

    int maxID = INT_MIN;
    unordered_set<int> foundIDs;

    while (getline(cin, input)) {
        int rowMin = 0, rowMax = 127;
        int colMin = 0, colMax = 7;

        for (char c : input) {
            int rowMid = (rowMin + rowMax) / 2;
            int colMid = (colMin + colMax) / 2;

            if (c == 'F') {
                rowMax = rowMid;
            } else if (c == 'B') {
                rowMin = rowMid;
            } else if (c == 'L') {
                colMax = colMid;
            } else if (c == 'R') {
                colMin = colMid;
            }
        }

        int seatID = (rowMax * 8) + colMax;
        maxID = max(maxID, seatID);

        foundIDs.insert(seatID);
    }

    cout << "Max ID: " << maxID << endl;

    for (int i = 0; ; i++) {
        if (foundIDs.find(i) == foundIDs.end() && foundIDs.find(i - 1) != foundIDs.end() && foundIDs.find(i + 1) != foundIDs.end()) {
            cout << "Seat ID: " << i << endl;
            break;
        }
    }

    return 0;
}