#ifndef VIRTUAL_GPU_HARDWARE_HPP
#define VIRTUAL_GPU_HARDWARE_HPP

#include "isa.hpp"
#include <vector>

class GlobalMemory {
private:
    std::vector<float> vram;

public:
    GlobalMemory();

    float read(int index) { return vram[index]; }
    void write(int index, float value);
    void writeBlock(int start_index, const std::vector<float>& data);
};

class RegisterFile {
private:
    std::vector<float> registers;
    int threadIdx;

public:
    RegisterFile();

    void setThreadId(int id) { threadIdx = id; }
    int getThreadId() { return threadIdx; }
    float readRegister(int index) { return registers[index]; }
    void writeRegister(int index, float value);
};

class ArithmeticLogicUnit {
private:
    float last_calculated_result;

public:
    ArithmeticLogicUnit();
    float execute(OpCode operation, float operand1, float operand2);
};

class GPUCore {
private:
    std::vector<RegisterFile> threads;
    ArithmeticLogicUnit alu;
    GlobalMemory vram;

public:
    GPUCore();

    // Hardware control
    void copyHostToDevice(int vram_start_index, const std::vector<float>& host_data);
    void executeInstruction(Instruction instr);
    void executeProgram(const std::vector<Instruction>& program);

    // Debugging
    void printRegister(int index);
    void printVRAM(int start_index, int end_index);
};

#endif