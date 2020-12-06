#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

int main() {
    vector<unordered_set<char>> answers;
    vector<unordered_set<char>> commonAnswers;

    string input;
    bool addunordered_set = true;
    while (getline(cin, input)) {
        if (addunordered_set) {
            commonAnswers.push_back(unordered_set<char>());
            for (char c : input) {
                commonAnswers.back().insert(c);
            }

            answers.push_back(unordered_set<char>());
            addunordered_set = false;
        }

        if (input.size() == 0) {
            addunordered_set = true;
        } else {
            unordered_set<char> inputChars;

            for (char c : input) {
                inputChars.insert(c);
                answers.back().insert(c);
            }

            vector<char> eraseChars;
            for (char c : commonAnswers.back()) {
                if (inputChars.find(c) == inputChars.end()) {
                    eraseChars.push_back(c);
                }
            }

            for (char c : eraseChars) {
                commonAnswers.back().erase(c);
            }
        }
    }

    int answerCount = 0;
    for (unordered_set<char>& groupAnswers : answers) {
        answerCount += groupAnswers.size();
    }

    int commonAnswerCount = 0;
    for (unordered_set<char>& groupAnswers : commonAnswers) {
        commonAnswerCount += groupAnswers.size();
    }

    cout << "Total Answers: " << answerCount << endl;
    cout << "Total Common Answers: " << commonAnswerCount << endl;
}