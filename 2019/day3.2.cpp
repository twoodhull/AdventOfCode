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
    int startCost;

    Line(Vec2 start, Vec2 end, int startCost) : 
        start(start),
        end(end),
        startCost(startCost) 
    {
        isHorizontal = start.y == end.y;
        isVertical = start.x == end.x;
    }

    int stepsToPoint(Vec2& point) {
        return abs(start.x - point.x) + abs(start.y - point.y) + startCost;
    }

    static bool intersects(Line& l1, Line& l2, int& cost) {
        if ((l1.isHorizontal && l2.isHorizontal) || (l1.isVertical && l2.isVertical)) {
            return false;
        }

        if (l1.isVertical) {
            return intersects(l2, l1, cost);
        }

        // l1 is guaranteed horizontal now

        Vec2 intersection(l2.start.x, l1.start.y);
        cost = l1.stepsToPoint(intersection) + l2.stepsToPoint(intersection);

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

        ret.push_back(Line(lastPos, currentPos, cost));
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
            int cost;

            if (Line::intersects(l1, l2, cost)) {
                minCost = min(minCost, cost);
            }
        }
    }

    cout << minCost << endl;

    return 0;
}