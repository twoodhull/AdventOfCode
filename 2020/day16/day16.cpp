#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int main() {
    unordered_map<string, int> keyMap;
    unordered_map<int, string> reverseKeyMap;

    regex rulePattern("([\\w\\s]+): (\\d+)-(\\d+) or (\\d+)-(\\d+)");
    regex numberPattern("\\d+");
    smatch match;

    vector<pair<pair<int, int>, pair<int, int>>> validRanges;
    vector<int> myTickets;

    string input;
    int index = 0;
    while (getline(cin, input)) {
        if (input == "") {
            break;
        }

        if (regex_match(input, match, rulePattern)) {
            keyMap[match[1].str()] = index;
            reverseKeyMap[index] = match[1].str();
            index++;

            validRanges.push_back(make_pair(
                make_pair(stoi(match[2].str()), stoi(match[3].str())),
                make_pair(stoi(match[4].str()), stoi(match[5].str()))
            ));
        }
    }

    getline(cin, input);
    getline(cin, input);

    while (regex_search(input, match, numberPattern)) {
        myTickets.push_back(stoi(match[0].str()));
        input = match.suffix().str();
    }

    vector<unordered_set<string>> potentialKeys(myTickets.size());
    for (auto& keySet : potentialKeys) {
        for (auto it : keyMap) {
            keySet.insert(it.first);
        }
    }

    getline(cin, input);
    getline(cin, input);

    int invalidSum = 0;
    while (getline(cin, input)) {
        vector<unordered_set<string>> validKeys;
        bool validTicket = true;

        while (regex_search(input, match, numberPattern)) {
            int ticketNum = stoi(match[0].str());
            validKeys.push_back(unordered_set<string>());

            bool inRange = false;
            for (int i = 0; i < validRanges.size(); i++) {
                auto& range = validRanges[i];
                if (
                    (range.first.first <= ticketNum && ticketNum <= range.first.second)
                    || (range.second.first <= ticketNum && ticketNum <= range.second.second)
                ) {
                    inRange = true;
                    validKeys.back().insert(reverseKeyMap[i]);
                }
            }

            if (!inRange) {
                invalidSum += ticketNum;
                validTicket = false;
            }

            input = match.suffix().str();
        }

        if (validTicket) {
            for (auto it : keyMap) {
                for (int i = 0; i < potentialKeys.size(); i++) {
                    if (validKeys[i].find(it.first) == validKeys[i].end()) {
                        potentialKeys[i].erase(it.first);
                    }
                }
            }
        }
    }

    cout << "Invalid Sum: " << invalidSum << endl;

    bool modified = true;
    while (modified) {
        modified = false;

        for (int i = 0; i < potentialKeys.size(); i++) {
            if (potentialKeys[i].size() == 1) {
                string removalKey = *potentialKeys[i].begin();

                for (int j = 0; j < potentialKeys.size(); j++) {
                    if (i == j) continue;

                    if (potentialKeys[j].find(removalKey) != potentialKeys[j].end()) {
                        modified = true;
                        potentialKeys[j].erase(removalKey);
                    }
                }
            }
        }
    }

    long long ticketProduct = 1;
    for (int i = 0; i < potentialKeys.size(); i++) {
        string key = *potentialKeys[i].begin();
        if (key.rfind("departure") == 0) {
            ticketProduct *= myTickets[i];
        }
    }

    cout << "Ticket Product: " << ticketProduct << endl;

    return 0;
}
