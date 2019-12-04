#include <iostream>
#include <vector>

using namespace std;

#define DIG_COUNT 6

int lo, hi;

int isValid(vector<int>& digits) {
    int val = 0;
    int last = -1;
    bool repeat = false;

    for (int i : digits) {
        repeat = repeat || i == last;
        last = i;

        val *= 10;
        val += i;
    }

    if (val < lo || val > hi) {
        return 0;
    }

    return repeat ? 1 : 0;
}

int go(int index, int last, vector<int>& digits) {
    if (index == DIG_COUNT) {
        return isValid(digits);
    }

    int ret = 0;
    for (int i = last; i <= 9; i++) {
        digits[index] = i;
        ret += go(index + 1, i, digits);
    }

    return ret;
}

int main() {
    cin >> lo >> hi;
    vector<int> digits(DIG_COUNT);

    cout << go(0, 0, digits) << endl;

    return 0;
}