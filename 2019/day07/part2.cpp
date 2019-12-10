#include <iostream>
#include <vector>
#include <climits>
#include <set>

#include "IntCodeMachine.h"

int getResult(const vector<int>& program, const vector<int>& permutation) {
    vector<IntCodeMachine> machines;

    for (int i : permutation) {
        IntCodeMachine machine(program);
        machine.queueInput(i);
        machines.push_back(machine);
    }
    machines[0].queueInput(0);

    int machineIndex = 0;
    int ret = -1;
    while (true) {
        if (machines[machineIndex].getState() == MachineState::Done) {
            if (machineIndex == 4) {
                ret = machines[machineIndex].getOutputHistory().back();
                break;
            }

            machineIndex = (machineIndex + 1) % machines.size();
            continue;
        }

        machines[machineIndex].runProgram();
        machines[(machineIndex + 1) % machines.size()].queueInput(machines[machineIndex].readOutput());

        machineIndex = (machineIndex + 1) % machines.size();
    }

    return ret;
}

int go(const vector<int>& program, vector<int>& permutation, int depth, set<int>& used) {
    if (depth == 5) {
        return getResult(program, permutation);
    }

    int maxVal = INT_MIN;
    for (int i = 5; i <= 9; i++) {
        if (used.find(i) != used.end()) {
            continue;
        }

        used.insert(i);
        permutation.push_back(i);
        maxVal = max(maxVal, go(program, permutation, depth + 1, used));
        permutation.pop_back();
        used.erase(i);
    }

    return maxVal;
}

int main(int argc, char** argv) {
    vector<int> program = IntCodeMachine::loadProgramFromFile(argv[1]);
    vector<int> permutations;
    set<int> used;

    cout << go(program, permutations, 0, used) << endl;

    return 0;
}