#include <iostream>
#include <string>
#include <set>

using namespace std;

#define BAD_CHAR ((char)('z'+1))

bool verifyPassword(const string& password) {
    bool hasStraight = false;
    set<char> couples;

    for (int i = 0; i < password.length(); i++) {
        if (i < password.length() - 2) {
            hasStraight = hasStraight || (password[i] == (password[i+1]-1) && (password[i+1] == password[i+2]-1));
        }

        if (i < password.length() - 1) {
            if (password[i] == password[i+1]) {
                couples.insert(password[i]);
            }
        }

        if (password[i] == 'i' || password[i] == 'o' || password[i] == 'l') {
            return false;
        }
    }

    return hasStraight && couples.size() >= 2;
}

void incrementPassword(string& password) {
    int incIndex = password.length() - 1;

    for (bool successfulInc = false; !successfulInc && incIndex >= 0; incIndex--) {
        password[incIndex]++;

        while (password[incIndex] == 'i' || password[incIndex] == 'o' || password[incIndex] == 'l') {
            password[incIndex]++;
        }

        if (password[incIndex] == BAD_CHAR) {
            password[incIndex] = 'a';
        } else {
            successfulInc = true;
        }
    }
}

int main() {
    string password;
    cin >> password;

    while (!verifyPassword(password)) {
        incrementPassword(password);
    }

    cout << "Part 1: " << password << endl;

    incrementPassword(password);
    while (!verifyPassword(password)) {
        incrementPassword(password);
    }

    cout << "Part 2: " << password << endl;
}