#include <iostream>
#include <map>
#include <set>
#include <climits>
#include <vector>

#include "IntCodeMachine.h"

#define DIRECTION_COUNT 4

#define BLACK 0
#define WHITE 1

using namespace std;

int main(int argc, char** argv) {
    IntCodeMachine machine = IntCodeMachine::loadMachineFromFile(argv[1]);

    set<pair<int, int>> white;
    white.insert(pair<int, int>(0, 0));

    pair<int, int> position(0, 0);
    int dir = 0;
    int dy[] = {1, 0, -1, 0};
    int dx[] = {0, 1, 0, -1};

    int minX = INT_MAX, maxX = INT_MIN;
    int minY = INT_MAX, maxY = INT_MIN;

    while (machine.getState() != MachineState::Done) {
        minX = min(minX, position.first);
        minY = min(minY, position.second);
        maxX = max(maxX, position.first);
        maxY = max(maxY, position.second);

        int color = white.find(position) == white.end() ? BLACK : WHITE;
        machine.queueInput(color);
        machine.runProgram();

        if (machine.readNextOutput() == WHITE) {
            white.insert(position);
        } else {
            white.erase(position);
        }

        if (machine.readNextOutput() == 0) {
            dir = (DIRECTION_COUNT + dir - 1) % DIRECTION_COUNT;
        } else {
            dir = (dir + 1) % DIRECTION_COUNT;
        }

        position.first += dx[dir];
        position.second += dy[dir];
    }

    for (int i = maxY; i >= minY; i--) {
        for (int j = minX; j <= maxX; j++) {
            cout << ((white.find(pair<int, int>(j, i)) == white.end()) ? '_' : '#');
        }
        cout << endl;
    }

    return 0;
}
