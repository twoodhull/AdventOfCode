#include <iostream>

#include "IntCodeMachine.h"

#define EMPTY 0
#define WALL 1
#define BLOCK 2
#define PADDLE 3
#define BALL 4

using namespace std;

int main(int argc, char** argv) {
    IntCodeMachine machine = IntCodeMachine::loadMachineFromFile(argv[1]);
    machine.setAddress(0, 2);

    machine.runProgram();
    vector<long long> output = machine.readOutput();

    pair<long long, long long> ball;
    pair<long long, long long> paddle;

    for (int i = 0; i < output.size(); i += 3) {
        if (output[i + 2] == BALL) {
            ball.first = output[i];
            ball.second = output[i + 1];
        }
        if (output[i + 2] == PADDLE) {
            paddle.first = output[i];
            paddle.second = output[i + 1];
        }
    }

    long long score = -1;
    while (machine.getState() != MachineState::Done) {
        long long input = 0;
        if (paddle.first < ball.first) {
            input = 1;
        }
        if (paddle.first > ball.first) {
            input = -1;
        }

        machine.queueInput(input);
        machine.runProgram();

        output = machine.readOutput();
        for (int i = 0; i < output.size(); i += 3) {
            if (output[i] == -1) {
                score = output[i + 2];
            }
            if (output[i + 2] == BALL) {
            ball.first = output[i];
            ball.second = output[i + 1];
            }
            if (output[i + 2] == PADDLE) {
                paddle.first = output[i];
                paddle.second = output[i + 1];
            }
        }
    }

    cout << score << endl;;

    return 0;
}
