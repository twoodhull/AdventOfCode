#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <cmath>

using namespace std;
using ll = long long;

#define MAX_BIT 35

const regex maskPattern("mask = ([X01]{36})");
const regex memPattern("mem\\[(\\d+)\\] = (\\d+)");

ll getBit(ll bit, ll value) {
    return ((1L << bit) & value) != 0L ? 1L : 0L;
}

void setBit(ll bit, ll bitVal, ll& value) {
    if (bitVal == 0L) {
        value &= ~(1L << bit);
    } else {
        value |= (1L << bit);
    }
}

void part1(const vector<string>& inputLines) {
    unordered_map<ll, ll> memMap;
    string bitMask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    smatch match;

    for (const string& input : inputLines) {
        if (regex_match(input, match, maskPattern)) {
            bitMask = match[1].str();
        } else if (regex_match(input, match, memPattern)) {
            ll address = stoll(match[1].str());
            ll decimalVal = stoll(match[2].str());

            if (memMap.find(address) == memMap.end()) {
                memMap[address] = 0L;
            }

            ll& editVal = memMap[address];
            for (int bit = 0; bit <= MAX_BIT; bit++) {
                char bitMaskVal = bitMask[bitMask.length() - bit - 1];

                if (bitMaskVal == '0') {
                    setBit(bit, 0L, editVal);
                } else if (bitMaskVal == '1') {
                    setBit(bit, 1L, editVal);
                } else {
                    setBit(bit, getBit(bit, decimalVal), editVal);
                }
            }
        } else {
            cout << "INVALID INPUT: " << input << endl;
            break;
        }
    }

    ll sum = 0L;
    for (auto it : memMap) {
        sum += it.second;
    }

    cout << "Part 1 Memory Sum: " << sum << endl;
}

vector<ll> getAddresses(ll address, const string& bitMask) {
    int xCount = 0;
    for (char c : bitMask) {
        if (c == 'X') {
            xCount++;
        }
    }

    vector<ll> returnAddresses(pow(2, xCount), address);
    for (int i = 0; i < returnAddresses.size(); i++) {
        for (int bit = 0, xBit = 0; bit <= MAX_BIT; bit++) {
            char bitMaskVal = bitMask[bitMask.size() - bit - 1];

            if (bitMaskVal == '1') {
                setBit(bit, 1L, returnAddresses[i]);
            } else if (bitMaskVal == 'X') {
                setBit(bit, getBit(xBit, i), returnAddresses[i]);
                xBit++;
            }
        }
    }

    return returnAddresses;
}

void part2(const vector<string>& inputLines) {
    unordered_map<ll, ll> memMap;
    string bitMask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    smatch match;

    for (const string& input : inputLines) {
        if (regex_match(input, match, maskPattern)) {
            bitMask = match[1].str();
        } else if (regex_match(input, match, memPattern)) {
            ll address = stoll(match[1].str());
            ll decimalVal = stoll(match[2].str());

            vector<ll> realAddresses = getAddresses(address, bitMask);
            for (ll realAddress : realAddresses) {
                memMap[realAddress] = decimalVal;
            }
        } else {
            cout << "INVALID INPUT: " << input << endl;
            break;
        }
    }

    ll sum = 0L;
    for (auto it : memMap) {
        sum += it.second;
    }

    cout << "Part 2 Memory Sum: " << sum << endl;
}

int main() {
    vector<string> inputLines;

    string input;
    while (getline(cin, input)) {
        inputLines.push_back(input);
    }

    part1(inputLines);
    part2(inputLines);

    return 0;
}
