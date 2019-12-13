#include <iostream>

#include "IntCodeMachine.h"

#define BLOCK 2

using namespace std;

int main(int argc, char** argv) {
    IntCodeMachine machine = IntCodeMachine::loadMachineFromFile(argv[1]);

    machine.runProgram();
    vector<long long> output = machine.readOutput();

    int count = 0;
    for (int i = 0; i < output.size(); i += 3) {
        if (output[i + 2] == BLOCK) {
            count++;
        }
    }

    cout << count << endl;;

    return 0;
}
