#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
using SeatMap = vector<vector<vector<pair<int, int>>>>;

const vector<int> dx = {0, 1, 1, 1, 0, -1, -1, -1};
const vector<int> dy = {-1, -1, 0, 1, 1, 1, 0, -1};

void fillSeatMap(const vector<string>& grid, SeatMap& seatMap, int maxDist) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].length(); j++) {
            seatMap[i][j] = vector<pair<int, int>>();

            for (int k = 0; k < dx.size(); k++) {
                for (int dist = 1; dist <= maxDist; dist++) {
                    int y = i + (dy[k] * dist);
                    int x = j + (dx[k] * dist);

                    if (y >= 0 && x >= 0 && y < grid.size() && x < grid[y].length() && grid[y][x] != '.') {
                        seatMap[i][j].push_back(make_pair(y, x));
                        break;
                    }
                }
            }
        }
    }
}

int getOccupied(int y, int x, const vector<string>& grid, const SeatMap& seatMap) {
    int occupied = 0;

    for (auto& it : seatMap[y][x]) {
        if (grid[it.first][it.second] == '#') {
            occupied++;
        }
    }

    return occupied;
}

bool stepGrid(vector<string>& grid, vector<vector<int>>& occupiedCache, const SeatMap& seatMap, int occupiedRequirement) {
    bool changed = false;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].length(); j++) {
            occupiedCache[i][j] = getOccupied(i, j, grid, seatMap);
        }
    }

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].length(); j++) {
            if (grid[i][j] == 'L' && occupiedCache[i][j] == 0) {
                grid[i][j] = '#';
                changed = true;
            } else if (grid[i][j] == '#' && occupiedCache[i][j] >= occupiedRequirement) {
                grid[i][j] = 'L';
                changed = true;
            }
        }
    }

    return changed;
}

int getTotalOccupied(const vector<string>& grid) {
    int occupied = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].length(); j++) {
            if (grid[i][j] == '#') {
                occupied++;
            }
        }
    }

    return occupied;
}

int main() {
    vector<string> grid;

    string input;
    while (getline(cin, input)) {
        grid.push_back(input);
    }
    vector<string> originalGrid(grid);
    SeatMap seatMap(grid.size(), vector<vector<pair<int, int>>>(grid.front().length()));
    fillSeatMap(grid, seatMap, 1);

    vector<vector<int>> occupiedCache(grid.size(), vector<int>(grid.front().length()));
    while (stepGrid(grid, occupiedCache, seatMap, 4));

    cout << "Part 1 occupied seats: " << getTotalOccupied(grid) << endl;

    grid = originalGrid;
    fillSeatMap(grid, seatMap, max(grid.size(), grid.front().length()));
    while (stepGrid(grid, occupiedCache, seatMap, 5));

    cout << "Part 2 occupied seats: " << getTotalOccupied(grid) << endl;

    return 0;
}