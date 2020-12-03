#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main() {
    regex inputPattern("^(\\d+)-(\\d+)\\s+([a-z]):\\s+([a-z]+)$");

    string line;

    int validPasswords_p1 = 0;
    int validPasswords_p2 = 0;

    while (getline(cin, line)) {
        smatch matches;
        regex_search(line, matches, inputPattern);

        int min = stoi(matches[1].str());
        int max = stoi(matches[2].str());
        char test = matches[3].str()[0];
        string password = matches[4].str();

        int count = 0;
        for (char c : password) {
            if (c == test) {
                count++;
            }
        }

        if (min <= count && count <= max) {
            validPasswords_p1++;
        }

        if (password[min - 1] != password[max - 1] && (password[min - 1] == test || password[max - 1] == test)) {
            validPasswords_p2++;
        }
    }

    cout << "Part 1 Valid Passwords: " << validPasswords_p1 << endl;
    cout << "Part 2 Valid Passwords: " << validPasswords_p2 << endl;

    return 0;
}
