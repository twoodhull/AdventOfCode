#include <iostream>
#include <climits>

#include "IntCodeMachine.h"

#define GRID_SIZE 50
#define SQUARE_SIZE 99

bool pointInTractor(const pair<long long, long long>& point, const IntCodeMachine& constMachine) {
    if (point.first < 0 || point.second < 0) {
        return false;
    }

    IntCodeMachine machine(constMachine);
    machine.queueInput(point.first);
    machine.queueInput(point.second);
    machine.runProgram();
    return machine.readNextOutput() == 1LL;
}

void part1(const IntCodeMachine& constMachine) {
    int count = 0;
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            count += pointInTractor(pair<long long, long long>(x, y), constMachine) ? 1 : 0;
        }
    }

    cout << "Total Spaces: " << count << endl;
}

bool canFit(long long cornerX, long long cornerY, int size, const IntCodeMachine& constMachine) {
    vector<pair<long long, long long>> points {
        make_pair(cornerX, cornerY),
        make_pair(cornerX + size, cornerY),
        make_pair(cornerX, cornerY + size),
        make_pair(cornerX + size, cornerY + size),
    };

    for (auto& point : points) {
        if (!pointInTractor(point, constMachine)) {
            return false;
        }
    }

    return true;
}

void part2(const IntCodeMachine& constMachine) {
    pair<long long, long long> pos(0, SQUARE_SIZE);

    while (true) {
        if (pointInTractor(pos, constMachine)) {
            if (canFit(pos.first, pos.second - SQUARE_SIZE, SQUARE_SIZE, constMachine)) {
                pos.second -= SQUARE_SIZE;
                cout << "Nearest Point: (" << pos.first << ", " << pos.second << ") --> " << (pos.first * 10000 + pos.second) << endl;
                return;
            } else {
                pos.second++;
            }
        } else {
            pos.first++;
        }
    }
}

int main(int argc, char** argv) {
    const IntCodeMachine constMachine = IntCodeMachine::loadMachineFromFile(argv[1]);

    part1(constMachine);
    part2(constMachine);
}