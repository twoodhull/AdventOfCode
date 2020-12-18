#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <unordered_map>

using namespace std;
using ll = long long;

const unordered_map<char, int> precedenceMapPart1 = {
    {'+', 0},
    {'*', 0},
};

const unordered_map<char, int> precedenceMapPart2 = {
    {'+', 1},
    {'*', 0},
};

struct Output {
    virtual ll getNumber() const = 0;
    virtual ll performOperation(ll left, ll right) const = 0;

    virtual bool isNumber() const { return false; }
    virtual bool isOperator() const { return false; }
};

struct Number : public Output {
    ll val;

    Number(ll val) : val(val) {}

    virtual bool isNumber() const override { return true; }
    virtual ll getNumber() const override { return val; }
    virtual ll performOperation(ll left, ll right) const override { throw exception(); }
};

struct Operator : public Output {
    char op;

    Operator(char op) : op(op) {}

    virtual bool isOperator() const override { return true; }
    virtual ll getNumber() const override { throw exception(); }
    virtual ll performOperation(ll left, ll right) const override {
        switch (op) {
            case '+':
                return left + right;
            case '*':
                return left * right;
            default:
                throw exception();
        }
    }
};

ll reduce(const string& operation, const unordered_map<char, int>& precedenceMap) {
    stack<char> operatorStack;
    queue<Output*> outputQueue;

    for (int i = operation.length() - 1; i >= 0;) {
        if (operation[i] == ' ') {
            while (i >= 0 && operation[i] == ' ') i--;
            continue;
        }

        if (isdigit(operation[i])) {
            ll number = 0L;
            ll multiplier = 1L;
            while (isdigit(operation[i])) {
                number += multiplier * (ll)(operation[i] - '0');
                multiplier *= 10L;
                i--;
            }

            outputQueue.push(new Number(number));
            continue;
        }

        if (operation[i] == ')') {
            operatorStack.push(operation[i]);
            i--;
            continue;
        }

        if (operation[i] == '+' || operation[i] == '*') {
            while (!operatorStack.empty() && operatorStack.top() != ')' && (precedenceMap.at(operation[i]) < precedenceMap.at(operatorStack.top()))) {
                outputQueue.push(new Operator(operatorStack.top()));
                operatorStack.pop();
            }

            operatorStack.push(operation[i]);
            i--;
            continue;
        }

        if (operation[i] == '(') {
            while (operatorStack.top() != ')') {
                outputQueue.push(new Operator(operatorStack.top()));
                operatorStack.pop();
            }
            operatorStack.pop();
            i--;
            continue;
        }
    }

    while (!operatorStack.empty()) {
        outputQueue.push(new Operator(operatorStack.top()));
        operatorStack.pop();
    }

    stack<ll> numberStack;
    while (!outputQueue.empty()) {
        if (outputQueue.front()->isOperator()) {
            ll left = numberStack.top();
            numberStack.pop();
            ll right = numberStack.top();
            numberStack.pop();

            numberStack.push(outputQueue.front()->performOperation(left, right));
        } else {
            numberStack.push(outputQueue.front()->getNumber());
        }

        delete outputQueue.front();
        outputQueue.pop();
    }

    return numberStack.top();
}

int main() {
    string input;

    ll part1Sum = 0L;
    ll part2Sum = 0L;
    while (getline(cin, input)) {
        part1Sum += reduce(input, precedenceMapPart1);
        part2Sum += reduce(input, precedenceMapPart2);
    }

    cout << "Part 1 Total Sum: " << part1Sum << endl;
    cout << "Part 2 Total Sum: " << part2Sum << endl;

    return 0;
}
