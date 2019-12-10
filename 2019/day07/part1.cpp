#include <iostream>
#include <vector>
#include <climits>
#include <set>

#include "IntCodeMachine.h"

int getMaxVal(const vector<int>& program, int depth, int input, set<int>& used) {
    int maxVal = INT_MIN;

    for (int i = 0; i < 5; i++) {
        if (used.find(i) != used.end()) {
            continue;
        }

        IntCodeMachine machine(program);

        machine.queueInput(i);
        machine.queueInput(input);
        machine.runProgram();

        if (depth == 4) { // At amplifier E
            maxVal = max(maxVal, machine.readNextOutput());
        } else {
            used.insert(i);
            maxVal = max(maxVal, getMaxVal(program, depth + 1, machine.readNextOutput(), used));
            used.erase(i);
        }
    }

    return maxVal;
}

int main(int argc, char** argv) {
    vector<int> program = IntCodeMachine::loadProgramFromFile(argv[1]);
    set<int> used;

    cout << getMaxVal(program, 0, 0, used) << endl;

    return 0;
}