#include <cstdio>
#include <vector>

using namespace std;

void part1(const vector<int>& values) {
    for (int i = 0; i < values.size(); i++) {
        for (int j = i + 1; j < values.size(); j++) {
            if (values[i] + values[j] == 2020) {
                printf("%d * %d = %d\n", values[i], values[j], values[i] * values[j]);
                return;
            }
        }
    }
}

void part2(const vector<int>& values) {
    for (int i = 0; i < values.size(); i++) {
        for (int j = i + 1; j < values.size(); j++) {
            for (int k = j + 1; k < values.size(); k++) {
                if (values[i] + values[j] + values[k] == 2020) {
                    printf("%d * %d * %d = %d\n", values[i], values[j], values[k], values[i] * values[j] * values[k]);
                    return;
                }
            }
        }
    }
}

int main() {
    vector<int> values;

    int num;
    while (scanf("%d", &num) != EOF) {
        values.push_back(num);
    }

    part1(values);
    part2(values);

    return 0;
}
