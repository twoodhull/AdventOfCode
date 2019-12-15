#include <iostream>
#include <queue>
#include <set>

#include "IntCodeMachine.h"

using namespace std;

struct State {
    State(int steps, const pair<int, int>& position, const IntCodeMachine& baseMachine) :
        steps(steps),
        position(position),
        machine(baseMachine)
    {}

    const int steps;
    const pair<int, int> position;
    const IntCodeMachine machine;
};

int main(int argc, char** argv) {
    IntCodeMachine machine = IntCodeMachine::loadMachineFromFile(argv[1]);

    int dirCount = 4;
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};
    int dir[] = {4, 2, 3, 1}; // Set up direction inputs to match delta array

    set<pair<int, int>> used;
    used.insert(pair<int, int>(0, 0));

    State start(0, pair<int, int>(0, 0), machine);
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
            used.insert(newPos);

            IntCodeMachine newMachine(cur.machine);

            newMachine.queueInput(dir[i]);
            newMachine.runProgram();

            switch (newMachine.readNextOutput()) {
                case 0:
                    break;
                case 1: {
                    State next(cur.steps + 1, newPos, newMachine);
                    q.push(next);
                    break;
                }
                case 2:
                    cout << "It took " << (cur.steps + 1) << " steps" << endl;
                    return 0;
                default:
                    throw exception();
            }
        }
    }

    return 0;
}
