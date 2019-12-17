#include <iostream>
#include <vector>
#include <set>
#include <map>

#include "IntCodeMachine.h"

using namespace std;

ostream& operator<<(ostream& os, const pair<int, int>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template<class T>
ostream& operator<<(ostream& os, const vector<T>& vec) {
    os << "[";

    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (it > vec.begin()) {
            os << ", ";
        }

        os << *it;
    }

    os << "]";
    return os;
}

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define A 0
#define B 1
#define C 2

#define L -1
#define R -2

int dy[] = {-1, 0, 1, 0};
int dx[] = {0, 1, 0, -1};

int getRobotDirection(char robot) {
    switch (robot) {
        case 'v': return SOUTH;
        case '^': return NORTH;
        case '>': return EAST;
        case '<': return WEST;
        default:
            cout << "Invalid Robot: " << robot << endl;
            return -1;
    }
}

int getDirection(int deltaY, int deltaX) {
    deltaY /= abs(deltaY);
    deltaX /= abs(deltaX);

    for (int i = NORTH; i <= WEST; i++) {
        if (dy[i] == deltaY && dx[i] == deltaX) {
            return i;
        }
    }

    return -1;
}

string getFunctionString(vector<int>::iterator begin, vector<int>::iterator end, bool addNewLine = true) {
    string ret = "";

    for (auto it = begin; it != end; ++it) {
        if (*it == L) {
            ret += 'L';
        } else if (*it == R) {
            ret += 'R';
        } else {
            ret += to_string(*it);
        }

        if (it + 1 != end) {
            ret += ',';
        }
    }

    if (addNewLine) {
        ret += '\n';
    }
    return ret;
}

string getSequenceString(const vector<int>& sequence) {
    string ret = "";
    for (int i : sequence) {
        if (i == A) {
            ret.append("A,");
        } else if (i == B) {
            ret.append("B,");
        } else if (i == C) {
            ret.append("C,");
        }
    }

    ret[ret.length() - 1] = '\n';
    return ret;
}

bool inBounds(const pair<int, int>& pos, const vector<vector<char>>& scaffold) {
    return pos.first >= 0 && pos.second >= 0 && pos.first < scaffold.size() && pos.second < scaffold[pos.first].size();
}

int main(int argc, char** argv) {
    IntCodeMachine scaffoldPrinter = IntCodeMachine::loadMachineFromFile(argv[1]);
    IntCodeMachine scaffoldRunner = scaffoldPrinter;

    scaffoldPrinter.runProgram();
    vector<vector<char>> scaffold(1, vector<char>());

    for (long long l : scaffoldPrinter.readOutput()) {
        if ((char)l == '\n') {
            scaffold.push_back(vector<char>());
        } else {
            scaffold.back().push_back((char)l);
        }
    }

    while (scaffold.back().empty()) {
        scaffold.pop_back();
    }

    int alignmentSum = 0;
    char robot = '.';
    pair<int, int> robotPosition;

    for (int i = 1; i < scaffold.size() - 1; i++) {
        for (int j = 1; j < scaffold[i].size() - 1; j++) {
            if (scaffold[i][j] != '.') {
                if (scaffold[i][j] != '#') {
                    robot = scaffold[i][j];
                    robotPosition.first = i;
                    robotPosition.second = j;
                }

                bool isIntersection = true;
                for (int k = 0; k < 4 && isIntersection; k++) {
                    isIntersection = scaffold[i + dy[k]][j + dx[k]] != '.';
                }

                if (isIntersection) {
                    alignmentSum += i * j;
                }
            }
        }
    }

    cout << "Alignment sum: " << alignmentSum << endl;

    int dir = getRobotDirection(robot);
    vector<int> moves;

    while (true) {
        int left = (dir + 3) % 4;
        int right = (dir + 1) % 4;

        pair<int, int> leftPos(robotPosition.first + dy[left], robotPosition.second + dx[left]);
        pair<int, int> rightPos(robotPosition.first + dy[right], robotPosition.second + dx[right]);
        pair<int, int> nextPos;

        if (inBounds(leftPos, scaffold) && scaffold[leftPos.first][leftPos.second] == '#') {
            moves.push_back(L);
            dir = left;
            nextPos = leftPos;
        } else if (inBounds(rightPos, scaffold) && scaffold[rightPos.first][rightPos.second] == '#') {
            moves.push_back(R);
            dir = right;
            nextPos = rightPos;
        } else { // Reached the final end cap
            break;
        }

        int steps = 0;
        while (inBounds(nextPos, scaffold) && scaffold[nextPos.first][nextPos.second] != '.') {
            steps++;
            robotPosition = nextPos;
            nextPos.first += dy[dir];
            nextPos.second += dx[dir];
        }

        moves.push_back(steps);
    }

    const string constMoveString = getFunctionString(moves.begin(), moves.end());
    string sequenceString = "";
    string functionA = "";
    string functionB = "";
    string functionC = "";

    for (int aEnd = 2; aEnd < moves.size() && sequenceString.length() == 0; aEnd+=2) {
        functionA = getFunctionString(moves.begin(), moves.begin() + aEnd, false);

        if (functionA.length() > 20) {
            break;
        }

        for (int bStart = aEnd; bStart < moves.size() && sequenceString.length() == 0; bStart+=2) {
            for (int bEnd = bStart + 2; bEnd <= moves.size() && sequenceString.length() == 0; bEnd+=2) {
                functionB = getFunctionString(moves.begin() + bStart, moves.begin() + bEnd, false);

                if (functionB.length() > 20) {
                    break;
                }

                for (int cStart = bEnd; cStart < moves.size() && sequenceString.length() == 0; cStart+=2) {
                    for (int cEnd = cStart + 2; cEnd <= moves.size() && sequenceString.length() == 0; cEnd+=2) {
                        functionC = getFunctionString(moves.begin() + cStart, moves.begin() + cEnd, false);

                        if (functionC.length() > 20) {
                            break;
                        }

                        vector<int> sequence;

                        string moveString = constMoveString;
                        while (moveString.length() > 0) {
                            size_t newPos = -1;

                            if (moveString.find(functionA) == 0) {
                                sequence.push_back(A);
                                newPos = functionA.length() + 1;
                            } else if (moveString.find(functionB) == 0) {
                                sequence.push_back(B);
                                newPos = functionB.length() + 1;
                            } else if (moveString.find(functionC) == 0) {
                                sequence.push_back(C);
                                newPos = functionC.length() + 1;
                            } else {
                                break;
                            }

                            moveString = moveString.substr(newPos);
                        }

                        if ((sequence.size() * 2) - 1 > 20) {
                            continue;
                        }

                        if (moveString.length() == 0) {
                            sequenceString = getSequenceString(sequence);
                        }
                    }
                }
            }
        }
    }

    functionA += '\n';
    functionB += '\n';
    functionC += '\n';

    scaffoldRunner.setAddress(0, 2);
    scaffoldRunner.queueInput(sequenceString);
    scaffoldRunner.queueInput(functionA);
    scaffoldRunner.queueInput(functionB);
    scaffoldRunner.queueInput(functionC);
    scaffoldRunner.queueInput("n\n");
    scaffoldRunner.runProgram();

    cout << "Dust Collected: " << scaffoldRunner.readOutput().back() << endl;

    return 0;
}
