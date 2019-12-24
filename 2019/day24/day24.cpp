#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#define MAP_SIZE 5
#define DIR_COUNT 4
#define MID_X 2
#define MID_Y 2

int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};

enum class Side {
    Top,
    Bottom,
    Left,
    Right,
};

int bitCount(int mask) {
    int count = 0;

    while (mask != 0) {
        count += (mask & 1);
        mask >>= 1;
    }

    return count;
}

int bitPos(int y, int x) {
    return (y * MAP_SIZE) + x;
}

void set(int& mask, int bit) {
    mask |= (1 << bit);
}

bool isSet(int mask, int bit) {
    return (mask & (1 << bit)) != 0;
}

bool inBounds(int y, int x) {
    return y >= 0 && x >= 0 && y < MAP_SIZE && x < MAP_SIZE;
}

int part1_getAdjacent(int y, int x, int bugMask) {
    int ret = 0;

    for (int dir = 0; dir < DIR_COUNT; dir++) {
        int yy = y + dy[dir];
        int xx = x + dx[dir];

        if (inBounds(yy, xx) && isSet(bugMask, bitPos(yy, xx))) {
            ret++;
        }
    }

    return ret;
}

int part1(int bugMask) {
    unordered_set<int> states;
    while (true) {
        if (states.find(bugMask) != states.end()) {
            return bugMask;
        }
        states.insert(bugMask);

        int newMask = 0;
        for (int y = 0; y < MAP_SIZE; y++) {
            for (int x = 0; x < MAP_SIZE; x++) {
                int adjCount = part1_getAdjacent(y, x, bugMask);

                if (isSet(bugMask, bitPos(y, x))) {
                    if (adjCount == 1) {
                        set(newMask, bitPos(y, x));
                    }
                } else {
                    if (adjCount == 1 || adjCount == 2) {
                        set(newMask, bitPos(y, x));
                    }
                }
            }
        }

        bugMask = newMask;
    }

    return -1;
}

int sideBits(int mask, Side side) {
    int x, y, deltaY, deltaX;

    switch (side) {
        case Side::Top:
            x = y = deltaY = 0;
            deltaX = 1;
            break;
        case Side::Bottom:
            x = deltaY = 0;
            y = MAP_SIZE - 1;
            deltaX = 1;
            break;
        case Side::Left:
            x = y = deltaX = 0;
            deltaY = 1;
            break;
        case Side::Right:
            y = deltaX = 0;
            x = MAP_SIZE - 1;
            deltaY = 1;
            break;
    }

    int ret = 0;
    while (inBounds(y, x)) {
        if (isSet(mask, bitPos(y, x))) {
            ret++;
        }
        y += deltaY;
        x += deltaX;
    }

    return ret;
}

int part2_getAdjacent(int y, int x, int depth, unordered_map<int, int>& state) {
    int ret = 0;

    for (int dir = 0; dir < DIR_COUNT; dir++) {
        int yy = y + dy[dir];
        int xx = x + dx[dir];

        if (yy == MID_Y && xx == MID_X) {
            Side side;
            int mask = state[depth + 1];

            if (y < yy) {
                side = Side::Top;
            } else if (y > yy) {
                side = Side::Bottom;
            } else if (x < xx) {
                side = Side::Left;
            } else if (x > xx) {
                side = Side::Right;
            }

            ret += sideBits(mask, side);
        } else if (yy == -1) {
            ret += isSet(state[depth - 1], bitPos(MID_Y - 1, MID_X)) ? 1 : 0;
        } else if (yy == MAP_SIZE) {
            ret += isSet(state[depth - 1], bitPos(MID_Y + 1, MID_X)) ? 1 : 0;
        } else if (xx == -1) {
            ret += isSet(state[depth - 1], bitPos(MID_Y, MID_X - 1)) ? 1 : 0;
        } else if (xx == MAP_SIZE) {
            ret += isSet(state[depth - 1], bitPos(MID_Y, MID_X + 1)) ? 1 : 0;
        } else if (inBounds(yy, xx) && isSet(state[depth], bitPos(yy, xx))) {
            ret++;
        }
    }

    return ret;
}

int part2(int bugMask) {
    unordered_map<int, int> state;
    unordered_map<int ,int> newState;
    int minDepth = 0;
    int maxDepth = 0;

    state[0] = bugMask;

    int count = 0;
    for (int i = 0; i < 200; i++) {
        minDepth--;
        maxDepth++;

        for (int i = minDepth; i <= maxDepth; i++) {
            int curMask = state[i];
            int newMask = 0;

            for (int y = 0; y < MAP_SIZE; y++) {
                for (int x = 0; x < MAP_SIZE; x++) {
                    if (y == MID_Y && x == MID_X) {
                        continue;
                    }

                    int adjCount = part2_getAdjacent(y, x, i, state);
                    if (isSet(curMask, bitPos(y, x))) {
                        if (adjCount == 1) {
                            set(newMask, bitPos(y, x));
                        }
                    } else {
                        if (adjCount == 1 || adjCount == 2) {
                            set(newMask, bitPos(y, x));
                        }
                    }
                }
            }

            newState[i] = newMask;
        }

        for (int i = minDepth; i <= maxDepth; i++) {
            state[i] = newState[i];
        }
    }

    int ret = 0;
    for (int i = minDepth; i <= maxDepth; i++) {
        ret += bitCount(state[i]);
    }

    return ret;
}

int main() {
    string line;

    int bugMask = 0;

    for (int y = 0; y < MAP_SIZE; y++) {
        cin >> line;
        for (int x = 0; x < MAP_SIZE; x++) {
            if (line[x] == '#') {
                set(bugMask, bitPos(y, x));
            }
        }
    }

    cout << "Part 1: " << part1(bugMask) << endl;
    cout << "Part 2: " << part2(bugMask) << endl;

    return 0;
}