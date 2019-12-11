#include <iostream>
#include <vector>

#include "IntCodeMachine.h"


int main(int argc, char** argv) {
    IntCodeMachine machine = IntCodeMachine::loadMachineFromFile(argv[1]);
    machine.queueInput(1);
    machine.runProgram();

    while (machine.hasOutput()) {
        cout << machine.readNextOutput() << endl;
    }

    return 0;
}
