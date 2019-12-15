#include <iostream>
#include <queue>
#include <set>
#include <climits>
#include <unistd.h>

#include "IntCodeMachine.h"

using namespace std;

struct State {
    State(const pair<int, int>& position, const IntCodeMachine& baseMachine) :
        position(position),
        machine(baseMachine)
    {}

    const pair<int, int> position;
    const IntCodeMachine machine;
};

void drawState(int minX, int minY, int maxX, int maxY, set<pair<int, int>>& walls, set<pair<int, int>>& spaces) {
    usleep(10000);
    cout << "\033[s";

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            pair<int, int> pos(y, x);

            if (walls.find(pos) != walls.end()) {
                cout << u8"\u2588\u2588";
            } else if (spaces.find(pos) != spaces.end()) {
                cout << "  ";
            } else {
                cout << u8"\u2591\u2591";
            }
        }
        cout << endl;
    }
    cout << endl;

    cout << "\033[u";
}

int main(int argc, char** argv) {
    IntCodeMachine machine = IntCodeMachine::loadMachineFromFile(argv[1]);

    int dirCount = 4;
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};
    int dir[] = {4, 2, 3, 1}; // Set up direction inputs to match delta array

    set<pair<int, int>> used;

    int minX = INT_MAX, maxX = INT_MIN, minY = INT_MAX, maxY = INT_MIN;
    used.insert(pair<int, int>(0, 0));

    {
        State start(pair<int, int>(0, 0), machine);
        queue<State> q;
        q.push(start);

        while (!q.empty()) {
            State cur = q.front();
            q.pop();

            for (int i = 0; i < dirCount; i++) {
                pair<int, int> newPos(cur.position.first + dy[i], cur.position.second + dx[i]);
                if (used.find(newPos) != used.end()) {
                    continue;
                }

                minX = min(minX, newPos.second);
                maxX = max(maxX, newPos.second);
                minY = min(minY, newPos.first);
                maxY = max(maxY, newPos.first);

                used.insert(newPos);

                IntCodeMachine newMachine(cur.machine);

                newMachine.queueInput(dir[i]);
                newMachine.runProgram();

                switch (newMachine.readNextOutput()) {
                    case 0:
                        break;
                    case 2:
                    case 1: {
                        State next(newPos, newMachine);
                        q.push(next);
                        break;
                    }
                    default:
                        throw exception();
                }
            }
        }
    }

    {
        used.clear();
        used.insert(pair<int, int>(0, 0));
        set<pair<int, int>> walls;
        set<pair<int, int>> spaces;

        drawState(minX, minY, maxX, maxY, walls, spaces);
        sleep(3);

        spaces.insert(pair<int, int>(0, 0));
        State start(pair<int, int>(0, 0), machine);
        queue<State> q;
        q.push(start);

        while (!q.empty()) {
            State cur = q.front();
            q.pop();

            for (int i = 0; i < dirCount; i++) {
                pair<int, int> newPos(cur.position.first + dy[i], cur.position.second + dx[i]);
                if (used.find(newPos) != used.end()) {
                    continue;
                }

                minX = min(minX, newPos.second);
                maxX = max(maxX, newPos.second);
                minY = min(minY, newPos.first);
                maxY = max(maxY, newPos.first);

                used.insert(newPos);

                IntCodeMachine newMachine(cur.machine);

                newMachine.queueInput(dir[i]);
                newMachine.runProgram();

                switch (newMachine.readNextOutput()) {
                    case 0:
                        walls.insert(newPos);
                        break;
                    case 2:
                    case 1: {
                        spaces.insert(newPos);
                        State next(newPos, newMachine);
                        q.push(next);
                        break;
                    }
                    default:
                        throw exception();
                }
            }

            drawState(minX, minY, maxX, maxY, walls, spaces);
        }
    }

    return 0;
}
