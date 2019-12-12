#include <iostream>
#include <map>
#include <set>

#include "IntCodeMachine.h"

#define DIRECTION_COUNT 4

#define BLACK 0
#define WHITE 1

using namespace std;

int main(int argc, char** argv) {
    IntCodeMachine machine = IntCodeMachine::loadMachineFromFile(argv[1]);

    map<pair<int, int>, int> painted;
    set<pair<int, int>> white;

    pair<int, int> position(0, 0);
    int dir = 0;
    int dy[] = {1, 0, -1, 0};
    int dx[] = {0, 1, 0, -1};
    int count = 0;

    while (machine.getState() != MachineState::Done) {
        int color = white.find(position) != white.end() ? WHITE : BLACK;
        machine.queueInput(color);
        machine.runProgram();

        if (machine.readNextOutput() == WHITE) {
            white.insert(position);
        } else {
            white.erase(position);
        }

        painted[position]++;

        if (machine.readNextOutput() == 0) {
            dir = (DIRECTION_COUNT + dir - 1) % DIRECTION_COUNT;
        } else {
            dir = (dir + 1) % DIRECTION_COUNT;
        }

        position.first += dx[dir];
        position.second += dy[dir];
    }

    cout << painted.size() << endl;

    return 0;
}
