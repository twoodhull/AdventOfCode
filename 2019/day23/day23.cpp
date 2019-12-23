#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>

#include "IntCodeMachine.h"

using namespace std;

#define MACHINE_COUNT 50
#define NAT_ADDRESS 255

long long part1(const vector<long long>& program) {
    vector<IntCodeMachine> computers;
    unordered_map<int, queue<long long>> inputQueues;
    for (int i = 0; i < MACHINE_COUNT; i++) {
        computers.push_back(IntCodeMachine(program));
        
        inputQueues[i].push(i);
        inputQueues[i].push(-1);
    }

    for (bool programRan = true; programRan; ) {
        programRan = false;

        for (int i = 0; i < computers.size(); i++) {
            if (computers[i].getState() == MachineState::Done) {
                continue;
            }

            queue<long long>& readQueue = inputQueues[i];

            if (readQueue.empty()) {
                computers[i].queueInput(-1);
            } else {
                computers[i].queueInput(readQueue.front());
                readQueue.pop();
                computers[i].queueInput(readQueue.front());
                readQueue.pop();
            }
            computers[i].runProgram();
            programRan = true;

            vector<long long> output = computers[i].readOutput();
            for (int j = 0; j < output.size(); j+=3) {
                int address = output[j];

                if (address < computers.size()) {
                    inputQueues[address].push(output[j+1]);
                    inputQueues[address].push(output[j+2]);
                } else if (address == NAT_ADDRESS) {
                    return output[j+2];
                }
            }
        }
    }

    return -1LL;
}

long long part2(const vector<long long>& program) {
    vector<IntCodeMachine> computers;
    unordered_map<int, queue<long long>> inputQueues;
    long long lastNAT = LLONG_MIN;

    for (int i = 0; i < MACHINE_COUNT; i++) {
        computers.push_back(IntCodeMachine(program));
        
        inputQueues[i].push(i);
        inputQueues[i].push(-1);
    }

    queue<long long>& natQueue = inputQueues[NAT_ADDRESS];
    for (bool programRan = true, readInput, queuedOutput; programRan; ) {
        programRan = false;
        readInput = false;
        queuedOutput = false;

        for (int i = 0; i < computers.size(); i++) {
            if (computers[i].getState() == MachineState::Done) {
                continue;
            }

            queue<long long>& readQueue = inputQueues[i];

            if (readQueue.empty()) {
                computers[i].queueInput(-1);
            } else {
                computers[i].queueInput(readQueue.front());
                readQueue.pop();
                computers[i].queueInput(readQueue.front());
                readQueue.pop();
                readInput = true;
            }
            computers[i].runProgram();
            programRan = true;

            vector<long long> output = computers[i].readOutput();
            for (int j = 0; j < output.size(); j+=3) {
                int address = output[j];
                queuedOutput = true;

                inputQueues[address].push(output[j+1]);
                inputQueues[address].push(output[j+2]);
            }
        }

        if (!readInput && !queuedOutput) {
            while (natQueue.size() > 2) {
                natQueue.pop();
            }

            int x = natQueue.front();
            natQueue.pop();
            int y = natQueue.front();
            natQueue.pop();

            if (y == lastNAT) {
                return y;
            }

            lastNAT = y;
            computers[0].queueInput(x);
            computers[0].queueInput(y);
        }
    }

    return -1LL;
}

int main(int argc, char** argv) {
    const vector<long long> program = IntCodeMachine::loadProgramFromFile(argv[1]);

    cout << "Part 1: " << part1(program) << endl;
    cout << "Part 2: " << part2(program) << endl;

    return 0;
}