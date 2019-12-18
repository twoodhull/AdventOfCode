#pragma once

#include <string>
#include <vector>
#include <map>
#include <queue>

using namespace std;

#define PARAM_POSITION 0
#define PARAM_IMMEDIATE 1
#define PARAM_RELATIVE 2

#define ADD 1
#define MULT 2
#define INPUT 3
#define OUTPUT 4
#define JUMP_IF_TRUE 5
#define JUMP_IF_FALSE 6
#define LESS_THAN 7
#define EQUALS 8
#define MODIFY_RELATIVE_BASE 9

class RunningWhileDoneException : public exception {};
class RunningWithoutInputException : public exception {};
class InvalidReadParamException : public exception {};
class InvalidWriteParamException : public exception {};
class InvalidOpcodeException : public exception {};

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
    IntCodeMachine(const vector<long long>& inputProgram);
    IntCodeMachine(const IntCodeMachine& from);

    static vector<long long> loadProgramFromFile(char* fileName);
    static IntCodeMachine loadMachineFromFile(char* fileName);

    void setAddress(int address, long long value) { program[address] = value; }
    void runProgram();

    void queueInput(vector<long long> inputList);
    void queueInput(long long input);
    void queueInput(const string& inputString);

    bool hasOutput() const { return !outputQueue.empty(); }
    long long readNextOutput();
    vector<long long> readOutput();
    string readOutputString();
    const vector<long long> getOutputHistory() const { return outputHistory; }

    inline MachineState getState() const { return state; }

private:
    MachineState state;
    int programPointer;
    int relativeBase;

    vector<long long> program;
    queue<long long> inputQueue;
    queue<long long> outputQueue;
    vector<long long> outputHistory;

    long long readValue(int index, int paramMode);
    void writeValue(int index, int paramMode, long long value);
    void validateIndex(int index);
    inline void incrementProgramPointer(int inc) { programPointer += inc; }
    inline void setProgramPointer(int newPointer) { programPointer = newPointer; }
};
