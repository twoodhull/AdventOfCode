#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    string line;

    int totalArea = 0;
    int totalRibbon = 0;

    while (getline(cin, line)) {
        int i1 = line.find_first_of('x');
        int i2 = line.find_last_of('x');

        int l = stoi(line.substr(0, i1));
        int w = stoi(line.substr(i1 + 1, i2 - (i1 + 1)));
        int h = stoi(line.substr(i2 + 1));

        totalArea += 2 * ((l * w) + (l * h) + (w * h));
        totalArea += min(l * w, min(l * h, w * h));

        vector<int> sizes = {l, w, h};
        sort(sizes.begin(), sizes.end());
        totalRibbon += l * w * h;
        totalRibbon += 2 * (sizes[0] + sizes[1]);
    }

    cout << "Total Area: " << totalArea << endl;
    cout << "Total Ribbon: " << totalRibbon << endl;
}
