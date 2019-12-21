#include <iostream>
#include <vector>
#include <queue>

#include "IntCodeMachine.h"

using namespace std;

bool inAsciiRange(long long ll) {
    return 0 <= ll && ll < 256;
}

int main(int argc, char** argv) {
    const IntCodeMachine constMachine = IntCodeMachine::loadMachineFromFile(argv[1]);

    string commands = "";
    // Try to land immediately after a gap
    commands += "NOT C T\n";
    commands += "AND D T\n";
    commands += "OR T J\n";
    // Jump if there's a gap directly ahead
    commands += "NOT A T\n";
    commands += "OR T J\n";
    // Walk
    commands += "WALK\n";

    IntCodeMachine machine(constMachine);
    machine.queueInput(commands);
    machine.runProgram();

    for (long long ll : machine.readOutput()) {
        if (inAsciiRange(ll)) {
            cout << ((char)ll);
        } else {
            cout << "Part 1: " << ll << endl;
        }
    }

    commands = "";
    // Set T to true
    commands += "NOT T T\n";
    // Jump if there's a gap between now and 4 steps from now.
    commands += "AND A T\n";
    commands += "AND B T\n";
    commands += "AND C T\n";
    commands += "NOT T T\n";
    commands += "AND D T\n";
    // Jump if there's floor 5 or 8 steps from now.
    commands += "OR E J\n";
    commands += "OR H J\n";
    // AND the previous two boolean statements together.
    commands += "AND T J\n";
    // Run
    commands += "RUN\n";
    machine = constMachine;
    machine.queueInput(commands);
    machine.runProgram();

    for (long long ll : machine.readOutput()) {
        if (inAsciiRange(ll)) {
            cout << ((char)ll);
        } else {
            cout << "Part 2: " << ll << endl;
        }
    }

    return 0;
}