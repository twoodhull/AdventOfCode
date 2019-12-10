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

IntCodeMachine::IntCodeMachine(const vector<int>& inputProgram) :
    state(MachineState::Ready),
    programPointer(0),
    program(inputProgram)
{}

vector<int> IntCodeMachine::loadProgramFromFile(char* fileName) {
    ifstream file;
    file.open(string(fileName));

    string input;
    file >> input;
    file.close();

    Tokenizer tokenizer(input, ',');
    vector<int> program;

    string token;
    while (tokenizer.nextToken(token)) {
        program.push_back(stoi(token));
    }

    return program;
}

IntCodeMachine IntCodeMachine::loadMachineFromFile(char* fileName) {
    return IntCodeMachine(loadProgramFromFile(fileName));
}

void IntCodeMachine::runProgram() {
    if (state == MachineState::Done) {
        throw exception();
    }

    if (state == MachineState::WaitingForInput && inputQueue.empty()) {
        throw exception();
    }

    state = MachineState::Running;
    while (state == MachineState::Running) {
        int opcode = program[programPointer] % 100;
        int paramMode1 = (program[programPointer] / 100) % 10;
        int paramMode2 = (program[programPointer] / 1000) % 10;
        int paramMode3 = (program[programPointer] / 10000) % 10;

        bool shouldIncInstructionPointer = true;
        switch(opcode) {
            case ADD:
                writeValue(programPointer + 3, paramMode3, readValue(programPointer + 1, paramMode1) + readValue(programPointer + 2, paramMode2));
                break;
            case MULT:
                writeValue(programPointer + 3, paramMode3, readValue(programPointer + 1, paramMode1) * readValue(programPointer + 2, paramMode2));
                break;
            case INPUT:
                if (inputQueue.empty()) {
                    state = MachineState::WaitingForInput;
                    continue;
                }
                writeValue(programPointer + 1, paramMode1, inputQueue.front());
                inputQueue.pop();
                break;
            case OUTPUT: {
                int outputValue = readValue(programPointer + 1, paramMode1);
                outputQueue.emplace(outputValue);
                outputHistory.push_back(outputValue);
                break;
            }
            case JUMP_IF_TRUE:
                if (readValue(programPointer + 1, paramMode1) != 0) {
                    programPointer = readValue(programPointer + 2, paramMode2);
                    shouldIncInstructionPointer = false;
                }
                break;
            case JUMP_IF_FALSE:
                if (readValue(programPointer + 1, paramMode1) == 0) {
                    programPointer = readValue(programPointer + 2, paramMode2);
                    shouldIncInstructionPointer = false;
                }
                break;
            case LESS_THAN:
                writeValue(programPointer + 3, paramMode3, (readValue(programPointer + 1, paramMode1) < readValue(programPointer + 2, paramMode2)) ? 1 : 0);
                break;
            case EQUALS:
                writeValue(programPointer + 3, paramMode3, (readValue(programPointer + 1, paramMode1) == readValue(programPointer + 2, paramMode2)) ? 1 : 0);
                break;
            case 99:
                state = MachineState::Done;
                return;
            default:
                break;
        }

        if (shouldIncInstructionPointer) {
            programPointer += g_opcodeParams[opcode] + 1;
        }
    }
}

void IntCodeMachine::queueInput(vector<int> inputList) {
    for (int i : inputList) {
        inputQueue.emplace(i);
    }
}

void IntCodeMachine::queueInput(int input) {
    inputQueue.emplace(input);
}

int IntCodeMachine::readValue(int index, int paramMode) {
    switch (paramMode) {
        case PARAM_POSITION:
            return program[program[index]];
        case PARAM_IMMEDIATE:
            return program[index];
        default:
            throw exception();
    }

    return -1;
}

void IntCodeMachine::writeValue(int index, int paramMode, int value) {
    switch (paramMode) {
        case PARAM_POSITION:
            program[program[index]] = value;
            break;
        case PARAM_IMMEDIATE:
        default:
            throw exception();
    }
}

int IntCodeMachine::readNextOutput() {
    int ret = outputQueue.front();
    outputQueue.pop();
    return ret;
}

vector<int> IntCodeMachine::readOutput() {
    vector<int> ret(outputQueue.size());

    int index = 0;
    while (!outputQueue.empty()) {
        ret[index++] = outputQueue.front();
        outputQueue.pop();
    }

    return ret;
}