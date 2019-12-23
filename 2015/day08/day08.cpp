#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main() {
    int codeSize = 0;
    int memorySize = 0;
    int encodedSize = 0;

    regex escapedHex("\\\\x[a-z0-9][a-z0-9]");

    string line;
    while (getline(cin, line)) {
        codeSize += line.length();

        for (size_t i = 1; i < line.length() - 1; i++) {
            memorySize++;

            if (regex_match(line.begin() + i, line.begin() + i + 4, escapedHex)) {
                i += 3;
            } else if (line[i] == '\\') {
                i++;
            }
        }

        int encSize = 2;
        encodedSize += 2;
        for (char c : line) {
            encodedSize++;
            encSize++;
            if (c == '"' || c =='\\') {
                encSize++;
                encodedSize++;
            }
        }
    }

    cout << "Part 1: " << (codeSize - memorySize) << endl;
    cout << "Part 2: " << (encodedSize - codeSize) << endl;

    return 0;
}