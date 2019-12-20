#include <climits>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

#define WALL 0
#define SPACE 1
#define PORTAL 2
#define START 3
#define END 4

#define OUTER_VOID '!'

#define DIR_COUNT 4
int dy[] = {0, 0, 1, -1};
int dx[] = {1, -1, 0, 0};

struct State {
    pair<int, int> position;
    int steps = 0;
    int depth = 0;

    bool operator<(const State& other) const {
        if (position == other.position) {
            return depth < other.depth;
        }

        return position < other.position;
    }
};

struct Portal {
    pair<int, int> portalLocation;
    int deltaDepth = 0;

    Portal() : Portal(make_pair(0, 0), 0) {}
    Portal(const pair<int, int>& portalLocation, int deltaDepth) :
        portalLocation(portalLocation),
        deltaDepth(deltaDepth)
    {}
};

bool inBounds(const vector<string>& maze, const pair<int, int>& position) {
    return position.first >= 0 && position.second >= 0 && position.first < maze.size() && position.second < maze[position.first].length();
}

bool inBounds(const vector<vector<int>>& maze, const pair<int, int>& position) {
    return position.first >= 0 && position.second >= 0 && position.first < maze.size() && position.second < maze[position.first].size();
}

bool getAdjacentSpace(const vector<string>& maze, const pair<int, int>& position, pair<int, int>& outAdjacent) {
    bool found = false;

    for (int dir = 0; dir < DIR_COUNT; dir++) {
        outAdjacent = make_pair(position.first + dy[dir], position.second + dx[dir]);

        if (inBounds(maze, outAdjacent) && maze[outAdjacent.first][outAdjacent.second] == '.') {
            found = true;
            break;
        }
    }

    return found;
}

bool isOutsidePortal(const vector<string>& maze, const pair<int, int>& position) {
    for (int dir = 0; dir < DIR_COUNT; dir++) {
        pair<int, int> next(position.first + dy[dir], position.second + dx[dir]);

        if (!inBounds(maze, next)) {
            return true;
        }
        if (maze[next.first][next.second] == OUTER_VOID) {
            return true;
        }
    }

    return false;
}

void parseMaze(vector<vector<int>>& outMaze, map<pair<int, int>, Portal>& outPortals) {
    vector<string> maze;
    string line;

    while (getline(cin, line)) {
        maze.push_back(line);
    }

    queue<pair<int, int>> q;
    q.push(make_pair(0, 0));

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();

        if (!inBounds(maze, cur) || maze[cur.first][cur.second] == '#' || maze[cur.first][cur.second] == '.') {
            continue;
        }

        if (maze[cur.first][cur.second] == ' ') {
            maze[cur.first][cur.second] = OUTER_VOID;
        }

        q.push(make_pair(cur.first + 1, cur.second));
        q.push(make_pair(cur.first, cur.second + 1));
    }

    outMaze = vector<vector<int>>(maze.size(), vector<int>(maze[0].size(), 0));

    pair<int, int> start;
    pair<int, int> end;
    map<string, vector<Portal>> portalMap;

    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze[i].size(); j++) {
            if (maze[i][j] == '.') {
                outMaze[i][j] = SPACE;
            } else if ('A' <= maze[i][j] && maze[i][j] <= 'Z') {
                pair<int, int> position(i, j);
                pair<int, int> adjacent;

                if (getAdjacentSpace(maze, position, adjacent)) {
                    string portalName = "";

                    if (i < adjacent.first) {
                        portalName += maze[i-1][j];
                        portalName += maze[i][j];
                    } else if (i > adjacent.first) {
                        portalName += maze[i][j];
                        portalName += maze[i+1][j];
                    } else if (j < adjacent.second) {
                        portalName += maze[i][j-1];
                        portalName += maze[i][j];
                    } else if (j > adjacent.second) {
                        portalName += maze[i][j];
                        portalName += maze[i][j+1];
                    }

                    if (portalName == "AA") {
                        start = adjacent;
                    } else if (portalName == "ZZ") {
                        end = adjacent;
                    } else {
                        portalMap[portalName].push_back(Portal(adjacent, isOutsidePortal(maze, position) ? 1 : -1));
                    }
                }
            }
        }
    }

    outMaze[start.first][start.second] = START;
    outMaze[end.first][end.second] = END;

    for (auto it : portalMap) {
        pair<int, int> portal1 = it.second[0].portalLocation;
        pair<int, int> portal2 = it.second[1].portalLocation;

        outPortals[portal1] = it.second[1];
        outPortals[portal2] = it.second[0];

        outMaze[portal1.first][portal1.second] = PORTAL;
        outMaze[portal2.first][portal2.second] = PORTAL;
    }
}

int bfs(vector<vector<int>>& maze, map<pair<int, int>, Portal>& portals, State& start, bool usePortalDelta) {
    set<State> visited;
    visited.insert(start);
    queue<State> q;
    q.push(start);

    int minSteps = INT_MAX;
    while (!q.empty()) {
        State cur = q.front();
        q.pop();

        if (maze[cur.position.first][cur.position.second] == END) {
            if (!usePortalDelta || (usePortalDelta && cur.depth == 0)) {
                minSteps = cur.steps;
                break;
            }
        }
        
        if (maze[cur.position.first][cur.position.second] == PORTAL) {
            Portal& portal = portals[cur.position];

            if (!usePortalDelta || (usePortalDelta && portal.deltaDepth + cur.depth >= 0)) {
                State next;
                next.position = portal.portalLocation;
                next.steps = cur.steps + 1;
                next.depth = cur.depth + (usePortalDelta ? portals[cur.position].deltaDepth : 0);

                if (visited.find(next) == visited.end()) {
                    q.push(next);
                    visited.insert(next);
                }   
            }
        }

        for (int dir = 0; dir < DIR_COUNT; dir++) {
            State next;
            next.position = make_pair(cur.position.first + dy[dir], cur.position.second + dx[dir]);
            next.steps = cur.steps + 1;
            next.depth = cur.depth;

            if (inBounds(maze, next.position) && maze[next.position.first][next.position.second] != WALL && visited.find(next) == visited.end()) {
                q.push(next);
                visited.insert(next);
            }
        }
    }

    return minSteps;
}

int main() {
    vector<vector<int>> maze;
    map<pair<int, int>, Portal> portals;

    parseMaze(maze, portals);

    State start;
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze[i].size(); j++) {
            if (maze[i][j] == START) {
                start.position = make_pair(i, j);
            }
        }
    }

    cout << "Part 1: " << bfs(maze, portals, start, false) << " steps." << endl;
    cout << "Part 2: " << bfs(maze, portals, start, true) << " steps." << endl;

    return 0;
}