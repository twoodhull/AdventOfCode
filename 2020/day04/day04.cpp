#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <set>
#include <map>

using namespace std;

#define MIN_BYR 1920
#define MAX_BYR 2002
#define MIN_IYR 2010
#define MAX_IYR 2020
#define MIN_EYR 2020
#define MAX_EYR 2030
#define MIN_HGT_CM 150
#define MAX_HGT_CM 193
#define MIN_HGT_IN 59
#define MAX_HGT_IN 76

#define KEY_REGEX "(\\w{3}):([^\\s]+)"
#define BYR_REGEX "\\b(byr):(\\d{4})\\b"
#define IYR_REGEX "\\b(iyr):(\\d{4})\\b"
#define EYR_REGEX "\\b(eyr):(\\d{4})\\b"
#define HGT_REGEX "\\b(hgt):(\\d+(?:cm|in))\\b"
#define HCL_REGEX "\\b(hcl):(#[0-9a-f]{6})\\b"
#define ECL_REGEX "\\b(ecl):(amb|blu|brn|gry|grn|hzl|oth)\\b"
#define PID_REGEX "\\b(pid):(\\d{9})\\b"
#define CID_REGEX "\\b(cid):([^\\s]+)\\b"

#define PASSPORT_REGEX BYR_REGEX "|" IYR_REGEX "|" EYR_REGEX "|" HGT_REGEX "|" HCL_REGEX "|" ECL_REGEX "|" PID_REGEX "|" CID_REGEX
#define REGEX_GROUPS 16

const set<string> requiredKeys = { "ecl", "pid", "eyr", "hcl", "byr", "iyr", "hgt" };

bool validatePassport(map<string, string>& passportMap) {
    int byr = stoi(passportMap["byr"]);
    if (byr < MIN_BYR || MAX_BYR < byr) {
        return false;
    }

    int iyr = stoi(passportMap["iyr"]);
    if (iyr < MIN_IYR || MAX_IYR < iyr) {
        return false;
    }

    int eyr = stoi(passportMap["eyr"]);
    if (eyr < MIN_EYR || MAX_EYR < eyr) {
        return false;
    }

    regex heightPattern("(\\d+)(cm|in)");
    smatch heightMatch;
    if (regex_match(passportMap["hgt"], heightMatch, heightPattern)) {
        int hgt = stoi(heightMatch[1]);

        if (heightMatch[2] == "cm" && (hgt < MIN_HGT_CM || MAX_HGT_CM < hgt)) {
            return false;
        } else if (heightMatch[2] == "in" && (hgt < MIN_HGT_IN || MAX_HGT_IN < hgt)) {
            return false;
        }
    } else {
        return false;
    }

    // All other values are validated via regex and the validateKeys method

    return true;
}

template<typename Iterable>
bool validateKeys(Iterable& passportKeys) {
    bool valid = true;
    for (const string& key : requiredKeys) {
        valid = valid && passportKeys.find(key) != passportKeys.end();
    }

    return valid;
}

int main() {
    vector<vector<string>> passports;
    string input;

    bool addPassport = true;
    while (getline(cin, input)) {
        if (addPassport) {
            addPassport = false;
            passports.push_back(vector<string>());
        }

        if (input.size() == 0) {
            addPassport = true;
        } else {
            passports.back().push_back(input);
        }
    }

    int validKeys = 0, validPassports = 0;
    regex keyPattern(KEY_REGEX);
    regex passportPattern(PASSPORT_REGEX);
    smatch match;

    for (vector<string>& passportLines : passports) {
        set<string> passportKeys;
        map<string, string> passportMap;

        for (string& line : passportLines) {
            string subject(line);
            while(regex_search(subject, match, keyPattern)) {
                passportKeys.insert(match[1]);

                subject = match.suffix().str();
            }

            subject = string(line);
            while (regex_search(subject, match, passportPattern)) {
                for (int i = 1; i < REGEX_GROUPS; i+=2) {
                    if (match[i] != "") {
                        passportMap[match[i]] = match[i + 1];
                    }
                }

                subject = match.suffix().str();
            }
        }

        if (validateKeys(passportKeys)) {
            validKeys++;
        }

        if (validateKeys(passportMap) && validatePassport(passportMap)) {
            validPassports++;
        }
    }

    cout << "Part 1 Valid Passports: " << validKeys << endl;
    cout << "Part 2 Valid Passports: " << validPassports << endl;

    return 0;
}
