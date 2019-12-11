#include <fstream>
#include <iostream>

#include "IntCodeMachine.h"

using namespace std;

map<int, int> g_opcodeParams = {
    {ADD, 3},
    {MULT, 3},
    {INPUT, 1},
    {OUTPUT, 1},
    {JUMP_IF_TRUE, 2},
    {JUMP_IF_FALSE, 2},
    {LESS_THAN, 3},
    {EQUALS, 3},
    {MODIFY_RELATIVE_BASE, 1},
};

Tokenizer::Tokenizer(const string& str, char delim) :
    str(str),
    delim(delim),
    pointer(0)
{}

bool Tokenizer::nextToken(string& tok) {
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

IntCodeMachine::IntCodeMachine(const vector<long long>& inputProgram) :
    state(MachineState::Ready),
    programPointer(0),
    relativeBase(0),
    program(inputProgram)
{}

vector<long long> IntCodeMachine::loadProgramFromFile(char* fileName) {
    ifstream file;
    file.open(string(fileName));

    string input;
    file >> input;
    file.close();

    Tokenizer tokenizer(input, ',');
    vector<long long> program;

    string token;
    while (tokenizer.nextToken(token)) {
        program.push_back(stoll(token));
    }

    return program;
}

IntCodeMachine IntCodeMachine::loadMachineFromFile(char* fileName) {
    return IntCodeMachine(loadProgramFromFile(fileName));
}

void IntCodeMachine::runProgram() {
    if (state == MachineState::Done) {
        throw RunningWhileDoneException();
    }

    if (state == MachineState::WaitingForInput && inputQueue.empty()) {
        throw RunningWithoutInputException();
    }

    state = MachineState::Running;
    while (state == MachineState::Running) {
        int opcode = (int)program[programPointer] % 100;
        int paramMode1 = (int)(program[programPointer] / 100) % 10;
        int paramMode2 = (int)(program[programPointer] / 1000) % 10;
        int paramMode3 = (int)(program[programPointer] / 10000) % 10;

        bool shouldIncInstructionPointer = true;

        switch(opcode) {
            case ADD:
                writeValue(programPointer + 3, paramMode3, readValue(programPointer + 1, paramMode1) + readValue(programPointer + 2, paramMode2));
                incrementProgramPointer(g_opcodeParams[ADD] + 1);
                break;
            case MULT:
                writeValue(programPointer + 3, paramMode3, readValue(programPointer + 1, paramMode1) * readValue(programPointer + 2, paramMode2));
                incrementProgramPointer(g_opcodeParams[MULT] + 1);
                break;
            case INPUT:
                if (inputQueue.empty()) {
                    state = MachineState::WaitingForInput;
                } else {
                    writeValue(programPointer + 1, paramMode1, inputQueue.front());
                    inputQueue.pop();
                    incrementProgramPointer(g_opcodeParams[INPUT] + 1);
                }
                break;
            case OUTPUT: {
                long long outputValue = readValue(programPointer + 1, paramMode1);
                outputQueue.emplace(outputValue);
                outputHistory.push_back(outputValue);
                incrementProgramPointer(g_opcodeParams[OUTPUT] + 1);
                break;
            }
            case JUMP_IF_TRUE:
                if (readValue(programPointer + 1, paramMode1) != 0) {
                    setProgramPointer(readValue(programPointer + 2, paramMode2));
                } else {
                    incrementProgramPointer(g_opcodeParams[JUMP_IF_TRUE] + 1);
                }
                break;
            case JUMP_IF_FALSE:
                if (readValue(programPointer + 1, paramMode1) == 0) {
                    setProgramPointer(readValue(programPointer + 2, paramMode2));
                } else {
                    incrementProgramPointer(g_opcodeParams[JUMP_IF_FALSE] + 1);
                }
                break;
            case LESS_THAN:
                writeValue(programPointer + 3, paramMode3, (readValue(programPointer + 1, paramMode1) < readValue(programPointer + 2, paramMode2)) ? 1 : 0);
                incrementProgramPointer(g_opcodeParams[LESS_THAN] + 1);
                break;
            case EQUALS:
                writeValue(programPointer + 3, paramMode3, (readValue(programPointer + 1, paramMode1) == readValue(programPointer + 2, paramMode2)) ? 1 : 0);
                incrementProgramPointer(g_opcodeParams[EQUALS] + 1);
                break;
            case MODIFY_RELATIVE_BASE:
                relativeBase += readValue(programPointer + 1, paramMode1);
                incrementProgramPointer(g_opcodeParams[MODIFY_RELATIVE_BASE] + 1);
                break;
            case 99:
                state = MachineState::Done;
                break;
            default:
                throw InvalidOpcodeException();
                break;
        }
    }
}

void IntCodeMachine::queueInput(vector<long long> inputList) {
    for (int i : inputList) {
        inputQueue.emplace(i);
    }
}

void IntCodeMachine::queueInput(long long input) {
    inputQueue.emplace(input);
}

long long IntCodeMachine::readValue(int index, int paramMode) {
    int readIndex = -1;

    switch (paramMode) {
        case PARAM_POSITION:
            readIndex = (int)program[index];
            break;
        case PARAM_IMMEDIATE:
            readIndex = index;
            break;
        case PARAM_RELATIVE:
            readIndex = (int)program[index] + relativeBase;
            break;
        default:
            throw InvalidReadParamException();
    }

    validateIndex(readIndex);
    return program[readIndex];
}

void IntCodeMachine::writeValue(int index, int paramMode, long long value) {
    int writeIndex = -1;

    switch (paramMode) {
        case PARAM_POSITION:
            writeIndex = (int)program[index];
            break;
        case PARAM_RELATIVE:
            writeIndex = (int)program[index] + relativeBase;
            break;
        case PARAM_IMMEDIATE:
        default:
            throw InvalidWriteParamException();
    }

    validateIndex(writeIndex);
    program[writeIndex] = value;
}

long long IntCodeMachine::readNextOutput() {
    long long ret = outputQueue.front();
    outputQueue.pop();
    return ret;
}

vector<long long> IntCodeMachine::readOutput() {
    vector<long long> ret(outputQueue.size());

    int index = 0;
    while (!outputQueue.empty()) {
        ret[index++] = outputQueue.front();
        outputQueue.pop();
    }

    return ret;
}

void IntCodeMachine::validateIndex(int index) {
    if (program.size() <= index) {
        program.resize(index + 1, 0);
    }
}
