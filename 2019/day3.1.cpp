#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Vec2 {
    int x, y;

    Vec2() : Vec2(0, 0) {}
    Vec2(int x, int y) : x(x), y(y) {}
};

struct Line {
    bool isHorizontal, isVertical;
    Vec2 start, end;

    Line(Vec2 start, Vec2 end) : 
        start(start),
        end(end)
    {
        isHorizontal = start.y == end.y;
        isVertical = start.x == end.x;
    }

    static bool intersects(Line& l1, Line& l2, Vec2& intersection) {
        if ((l1.isHorizontal && l2.isHorizontal) || (l1.isVertical && l2.isVertical)) {
            return false;
        }

        if (l1.isVertical) {
            return intersects(l2, l1, intersection);
        }

        // l1 is guaranteed horizontal now

        intersection.x = l2.start.x;
        intersection.y = l1.start.y;

        int left = min(l1.start.x, l1.end.x);
        int right = max(l1.start.x, l1.end.x);
        int top = max(l2.start.y, l2.end.y);
        int bot = min(l2.start.y, l2.end.y);

        return (left < l2.start.x && l2.start.x < right) && (bot < l1.start.y && l1.start.y < top);
    }
};

struct Tokenizer {
    const string& str;
    char delim;
    size_t pointer;

    Tokenizer(const string& str, char delim) :
        str(str),
        delim(delim),
        pointer(0) 
    {}

    bool nextToken(string& tok) {
        if (pointer >= str.length()) {
            return false;
        }

        size_t nextDelim = str.find(delim, pointer);

        if (nextDelim == string::npos) {
            tok = str.substr(pointer);
            pointer = str.length();
        } else {
            tok = str.substr(pointer, nextDelim - pointer);
            pointer = nextDelim + 1;
        }

        return true;
    }
};

vector<Line> getLines(string& input) {
    Tokenizer tokenizer(input, ',');
    string token;
    Vec2 currentPos, lastPos;
    int cost = 0;

    vector<Line> ret;
    while (tokenizer.nextToken(token)) {
        lastPos = currentPos;

        int length = stoi(token.substr(1));

        switch(token[0]) {
            case 'U':
                currentPos.y += length;
                break;
            case 'D':
                currentPos.y -= length;
                break;
            case 'L':
                currentPos.x -= length;
                break;
            case 'R':
                currentPos.x += length;
                break;
            default:
                break;
        }

        ret.push_back(Line(lastPos, currentPos));
        cost += length;
    }

    return ret;
}

int main() {
    vector<Line> lines1;
    vector<Line> lines2;

    string input;
    cin >> input;
    lines1 = getLines(input);
    cin >> input;
    lines2 = getLines(input);

    int minCost = INT_MAX;
    for (Line& l1 : lines1) {
        for (Line& l2 : lines2) {
            Vec2 intersection;

            if (Line::intersects(l1, l2, intersection)) {
                minCost = min(minCost, abs(intersection.x) + abs(intersection.y));
            }
        }
    }

    cout << minCost << endl;

    return 0;
}