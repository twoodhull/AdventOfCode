#include <iostream>
#include <string>

using namespace std;

int main() {
    string line;
    cin >> line;

    int floor = 0;
    int counter = 0;
    int basement = -1;
    for (char c : line) {
        floor += c == '(' ? 1 : -1;

        counter++;
        if (floor < 0 && basement == -1) {
            basement = counter;
        }
    }

    cout << "Floor: " << floor << endl;
    cout << "Entered basement at index: " << basement << endl;

    return 0;
}
