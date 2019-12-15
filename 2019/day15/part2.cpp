#include <iostream>
#include <queue>
#include <set>

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

int main(int argc, char** argv) {
    IntCodeMachine machine = IntCodeMachine::loadMachineFromFile(argv[1]);

    int dirCount = 4;
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};
    int dir[] = {4, 2, 3, 1}; // Set up direction inputs to match delta array

    set<pair<int, int>> used;
    set<pair<int, int>> spaces;
    pair<int, int> oxygenLocation;

    spaces.insert(pair<int, int>(0, 0));
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
                used.insert(newPos);

                IntCodeMachine newMachine(cur.machine);

                newMachine.queueInput(dir[i]);
                newMachine.runProgram();

                switch (newMachine.readNextOutput()) {
                    case 0:
                        break;
                    case 2:
                        oxygenLocation = newPos;
                        // fall through
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
        }
    }

    {
        used.clear();
        used.insert(oxygenLocation);

        queue<int> q;
        q.push(oxygenLocation.first);
        q.push(oxygenLocation.second);
        q.push(0);

        int maxSteps = 0;
        while (!q.empty()) {
            int y = q.front();
            q.pop();
            int x = q.front();
            q.pop();
            int steps = q.front();
            q.pop();

            if (steps > maxSteps) {
                maxSteps = steps;
            }

            for (int i = 0; i < dirCount; i++) {
                pair<int, int> newPos(y + dy[i], x + dx[i]);

                if (used.find(newPos) != used.end() || spaces.find(newPos) == spaces.end()) {
                    continue;
                }

                used.insert(newPos);
                q.push(newPos.first);
                q.push(newPos.second);
                q.push(steps + 1);
            }
        }

        cout << "It will take " << maxSteps << " minutes" << endl;
    }
    

    return 0;
}
