#include <iostream>
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct Bag {
    unordered_map<string, int> contained;
    unordered_set<string> containedBy;
};

int getBags(const string& color, unordered_map<string, Bag>& bags) {
    int total = 0;

    Bag& bag = bags[color];
    for (auto it : bag.contained) {
        total += it.second + (it.second * getBags(it.first, bags));
    }

    return total;
}

int main() {
    unordered_map<string, Bag> bags;

    regex bagPattern("([\\w\\s]+) bags contain ");
    regex containPattern("no other bags|(\\d) ([\\w\\s]+) bags?");

    string input;
    while (getline(cin, input)) {
        smatch match;

        regex_search(input, match, bagPattern);

        string bagColor = match[1].str();
        if (bags.find(bagColor) == bags.end()) {
            bags[bagColor] = Bag();
        }

        input = match.suffix().str();
        while (regex_search(input, match, containPattern)) {
            if (match[0].str() == "no other bags") {
                break;
            }

            int amount = stoi(match[1].str());
            string color = match[2].str();

            bags[bagColor].contained[color] = amount;

            if (bags.find(color) == bags.end()) {
                bags[color] = Bag();
            }

            bags[color].containedBy.insert(bagColor);

            input = match.suffix().str();
        }
    }

    unordered_set<string> containedBy;
    queue<string> q;
    q.emplace("shiny gold");

    while (!q.empty()) {
        Bag& cur = bags[q.front()];
        q.pop();

        for (auto& color : cur.containedBy) {
            if (containedBy.find(color) == containedBy.end()) {
                containedBy.insert(color);
                q.emplace(color);
            }
        }
    }

    cout << "Total Colors: " << containedBy.size() << endl;
    cout << "Total Bags: " << getBags("shiny gold", bags) << endl;

    return 0;
}