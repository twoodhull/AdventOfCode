#include <cstdio>
#include <utility>
#include <cstdlib>

using namespace std;

using ll = long long;

const ll dx[] = {1, 0, -1, 0};
const ll dy[] = {0, -1, 0, 1};

int main() {
    pair<ll, ll> position_part1(0, 0);
    pair<ll, ll> position_part2(0, 0);
    pair<ll, ll> waypoint(10, 1);
    int dir = 0;

    char action;
    int value;
    while (scanf(" %c%d", &action, &value) != EOF) {
        switch (action) {
            case 'N':
                position_part1.second += value;
                waypoint.second += value;
                break;
            case 'S':
                position_part1.second -= value;
                waypoint.second -= value;
                break;
            case 'E':
                position_part1.first += value;
                waypoint.first += value;
                break;
            case 'W':
                position_part1.first -= value;
                waypoint.first -= value;
                break;
            case 'R': {
                dir = (dir + (value / 90)) % 4;

                for (int rot = value; rot > 0; rot -= 90) {
                    ll tmp = waypoint.first;
                    waypoint.first = waypoint.second;
                    waypoint.second = -tmp;
                }
                break;
            }
            case 'L': {
                dir = (dir - (value / 90)) % 4;
                while (dir < 0) { dir += 4; }

                for (int rot = value; rot > 0; rot -= 90) {
                    ll tmp = waypoint.first;
                    waypoint.first = -waypoint.second;
                    waypoint.second = tmp;
                }
                break;
            }
            case 'F':
                position_part1.first += dx[dir] * value;
                position_part1.second += dy[dir] * value;

                position_part2.first += value * waypoint.first;
                position_part2.second += value * waypoint.second;
                break;
            default:
                break;
        }
    }

    printf("Part 1: %d + %d = %d\n", abs(position_part1.first), abs(position_part1.second), abs(position_part1.first) + abs(position_part1.second));
    printf("Part 2: %d + %d = %d\n", abs(position_part2.first), abs(position_part2.second), abs(position_part2.first) + abs(position_part2.second));

    return 0;
}
