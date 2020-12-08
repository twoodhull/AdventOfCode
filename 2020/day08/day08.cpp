#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Instruction {
public:
    enum Operation {
        ACC,
        JMP,
        NOP,
    };

    static Operation operationFromString(const string& str) {
        auto it = s_stringOpMap.find(str);
        if (it == s_stringOpMap.end()) {
            // Throw exception
        }
        return it->second;
    }

    Instruction(const string& instructionString) {
        smatch match;

        if (!regex_match(instructionString, match, s_instructionPattern)) {
            // Throw exception
        }

        m_operation = operationFromString(match[1].str());
        m_opVal = stoi(match[2].str());
    }

    Operation getOperation() { return m_operation; }
    void setOperation(Operation op) { m_operation = op; }
    int getOpVal() { return m_opVal; }

private:
    const static regex s_instructionPattern;
    const static unordered_map<string, Operation> s_stringOpMap;

    Operation m_operation;
    int m_opVal;
};

const regex Instruction::s_instructionPattern = regex("(\\w+)\\s+\\+?(-?\\d+)");
const unordered_map<string, Instruction::Operation> Instruction::s_stringOpMap = {
    {"acc", Instruction::ACC},
    {"jmp", Instruction::JMP},
    {"nop", Instruction::NOP},
};

class Machine {
public:
    static Machine fromFile(const string& filePath) {
        vector<Instruction> instructions;

        ifstream file;
        file.open(filePath);

        string input;
        while (getline(file, input)) {
            instructions.push_back(Instruction(input));
        }

        return Machine(move(instructions));
    }

    Machine(vector<Instruction>&& instructions)
        : m_instructions(instructions)
        , m_accumulator(0)
        , m_instructionCounter(0)
    {}

    void nextOp() {
        if (!hasNextOp()) {
            // Throw exception
        }

        switch (m_instructions[m_instructionCounter].getOperation()) {
            case Instruction::ACC:
                m_accumulator += m_instructions[m_instructionCounter].getOpVal();
                m_instructionCounter++;
                break;
            case Instruction::JMP:
                m_instructionCounter += m_instructions[m_instructionCounter].getOpVal();
                break;
            case Instruction::NOP:
                m_instructionCounter++;
                break;
        }
    }

    bool hasNextOp() { return m_instructionCounter < m_instructions.size(); }

    void reset() {
        m_accumulator = 0;
        m_instructionCounter = 0;
    }

    int getAccumulator() { return m_accumulator; }
    int getInstructionCounter() { return m_instructionCounter; }

    Instruction& editInstruction(int index) { return m_instructions[index]; }
    int instructionCount() { return m_instructions.size(); }

private:
    vector<Instruction> m_instructions;

    int m_accumulator;
    int m_instructionCounter;
};

bool runMachine(Machine& machine) {
    unordered_set<int> instructionsCounters;
    while (machine.hasNextOp()) {
        if (instructionsCounters.find(machine.getInstructionCounter()) != instructionsCounters.end()) {
            return false;
        }

        instructionsCounters.insert(machine.getInstructionCounter());
        machine.nextOp();
    }

    return true;
}

int main() {
    //string filePath;
    //cin >> filePath;

    Machine machine = Machine::fromFile("/home/tyler/Documents/GitProjects/AdventOfCode/2020/day08/input.txt");
    runMachine(machine);
    cout << "Accumulator at loop: " << machine.getAccumulator() << endl;

    for (int i = 0; i < machine.instructionCount(); i++) {
        machine.reset();

        Instruction& instruction = machine.editInstruction(i);

        if (instruction.getOperation() == Instruction::JMP) {
            instruction.setOperation(Instruction::NOP);
        } else if (instruction.getOperation() == Instruction::NOP) {
            instruction.setOperation(Instruction::JMP);
        }

        if (runMachine(machine)) {
            break;
        }

        if (instruction.getOperation() == Instruction::JMP) {
            instruction.setOperation(Instruction::NOP);
        } else if (instruction.getOperation() == Instruction::NOP) {
            instruction.setOperation(Instruction::JMP);
        }
    }

    cout << "Accumulator after fix: " << machine.getAccumulator() << endl;

    return 0;
}