#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

#define PARAM_POSITION 0
#define PARAM_IMMEDIATE 1

#define ADD 1
#define MULT 2
#define INPUT 3
#define OUTPUT 4

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

vector<int> getProgram(char* fileName) {
    ifstream file;
    file.open(string(fileName));

    string input;
    file >> input;
    file.close();

    Tokenizer tokenizer(input, ',');
    vector<int> ret;

    string token;
    while (tokenizer.nextToken(token)) {
        ret.push_back(stoi(token));
    }

    return ret;
}

int* getParamPointer(vector<int>& program, int index, int paramMode) {
    switch (paramMode) {
        case PARAM_POSITION:
            return &(program[program[index]]);
        case PARAM_IMMEDIATE:
            return &(program[index]);
        default:
            throw exception();
    }
}

vector<int> runProgram(const vector<int>& inputProgram) {
    vector<int> program(inputProgram);

    int i = 0;
    while (true) {
        int opcode = program[i] % 100;
        int paramMode1 = (program[i] / 100) % 10;
        int paramMode2 = (program[i] / 1000) % 10;
        int paramMode3 = (program[i] / 10000) % 10;

        int paramCount = 0;
        switch(opcode) {
            case ADD:
                *getParamPointer(program, i + 3, paramMode3) = *getParamPointer(program, i + 1, paramMode1) + *getParamPointer(program, i + 2, paramMode2);
                paramCount = 3;
                break;
            case MULT:
                *getParamPointer(program, i + 3, paramMode3) = *getParamPointer(program, i + 1, paramMode1) * *getParamPointer(program, i + 2, paramMode2);
                paramCount = 3;
                break;
            case INPUT:
                cout << "Input: ";
                cin >> *getParamPointer(program, i + 1, paramMode1);
                paramCount = 1;
                break;
            case OUTPUT:
                cout << ">> " << *getParamPointer(program, i + 1, paramMode1) << endl;
                paramCount = 1;
                break;
            case 99:
                return program;
            default:
                break;
        }

        i += paramCount + 1; // Increment past the parameters + 1 for current instruction
    }

    throw exception();
}

int main(int argc, char** argsv) {
    vector<int> program = getProgram(argsv[1]);

    vector<int> result = runProgram(program);

    return 0;
}
