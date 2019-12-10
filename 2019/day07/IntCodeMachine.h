#pragma once

#include <string>
#include <vector>
#include <map>
#include <queue>

using namespace std;

#define PARAM_POSITION 0
#define PARAM_IMMEDIATE 1

#define ADD 1
#define MULT 2
#define INPUT 3
#define OUTPUT 4
#define JUMP_IF_TRUE 5
#define JUMP_IF_FALSE 6
#define LESS_THAN 7
#define EQUALS 8

struct Tokenizer {
    const string& str;
    char delim;
    size_t pointer;

    Tokenizer(const string& str, char delim);
    bool nextToken(string& tok);
};

enum class MachineState {
    Ready,
    Running,
    WaitingForInput,
    Done,
};

class IntCodeMachine {
public:
    IntCodeMachine() = delete;
    IntCodeMachine(const vector<int>& inputProgram);

    static vector<int> loadProgramFromFile(char* fileName);
    static IntCodeMachine loadMachineFromFile(char* fileName);
    
    void runProgram();

    void queueInput(vector<int> inputList);
    void queueInput(int input);

    bool hasOutput() { return !outputQueue.empty(); }
    int readNextOutput();
    vector<int> readOutput();
    const vector<int> getOutputHistory() { return outputHistory; }

    inline MachineState getState() { return state; }

private:
    MachineState state;
    int programPointer;

    vector<int> program;
    queue<int> inputQueue;
    queue<int> outputQueue;
    vector<int> outputHistory;

    int readValue(int index, int paramMode);
    void writeValue(int index, int paramMode, int value);
};