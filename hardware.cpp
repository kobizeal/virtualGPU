#include "hardware.hpp"
#include <iostream>

// ==========================================
// GLOBAL MEMORY
// ==========================================

GlobalMemory::GlobalMemory() {
    vram.resize(1024);
}

void GlobalMemory::write(int index, float value) {
    vram[index] = value;
}

void GlobalMemory::writeBlock(int start_index, const std::vector<float>& data) {
    for (int i = 0; i < data.size(); i++) {
        vram[start_index + i] = data[i];
    }
}

// ==========================================
// REGISTER FILE
// ==========================================

RegisterFile::RegisterFile() {
    registers.resize(16);
    threadIdx = 0;
}

void RegisterFile::writeRegister(int index, float value) {
    registers[index] = value;
}

// ==========================================
// ARITHMETIC LOGIC UNIT (ALU)
// ==========================================

ArithmeticLogicUnit::ArithmeticLogicUnit() {
    last_calculated_result = 0.0f;
}

float ArithmeticLogicUnit::execute(OpCode operation, float operand1, float operand2) {
    switch (operation) {
        case OpCode::ADD:
            last_calculated_result = operand1 + operand2;
            break;
        case OpCode::SUBTRACT:
            last_calculated_result = operand1 - operand2;
            break;
        case OpCode::MULTIPLY:
            last_calculated_result = operand1 * operand2;
            break;
        case OpCode::DIVIDE:
            last_calculated_result = operand1 / operand2;
            break;
        case OpCode::FLOOR_DIVIDE:
            last_calculated_result = static_cast<float>(static_cast<int>(operand1) / static_cast<int>(operand2));
            break;
        case OpCode::MODULO:
            last_calculated_result = static_cast<float>(static_cast<int>(operand1) % static_cast<int>(operand2));
            break;
    }
    return last_calculated_result;
}

// ==========================================
// GPU CORE
// ==========================================

GPUCore::GPUCore() {
    threads.resize(4);
    for (int i = 0; i < 4; i++) {
        threads[i].setThreadId(i);
    }
}

void GPUCore::copyHostToDevice(int vram_start_index, const std::vector<float>& host_data) {
    vram.writeBlock(vram_start_index, host_data);
}

void GPUCore::executeInstruction(Instruction instr) {
    for (int i = 0; i < threads.size(); i++) {
        RegisterFile& current_thread = threads[i];

        if (instr.operation == OpCode::LOAD_INDIRECT) {
            int register_index = instr.source_register1;
            int vram_address = static_cast<int>(current_thread.readRegister(register_index));
            float fetched_data = vram.read(vram_address);
            current_thread.writeRegister(instr.destination_register, fetched_data);
        }
        else if (instr.operation == OpCode::LOAD) {
            int vram_address = instr.source_register1 + i;
            float fetched_data = vram.read(vram_address);
            current_thread.writeRegister(instr.destination_register, fetched_data);
        }
        else if (instr.operation == OpCode::STORE) {
            int vram_address = instr.destination_register + i; // Assuming destination holds VRAM base
            float answer_to_save = current_thread.readRegister(instr.source_register1); // Assuming source holds value
            vram.write(vram_address, answer_to_save);
        }
        else if (instr.operation == OpCode::SET_VALUE) {
            current_thread.writeRegister(instr.destination_register, static_cast<float>(instr.source_register1));
        }
        else if (instr.operation == OpCode::MOV_TID) {
            int threadIdx = current_thread.getThreadId();
            current_thread.writeRegister(instr.destination_register, static_cast<float>(threadIdx));
        }
        else {
            float val1 = current_thread.readRegister(instr.source_register1);
            float val2 = current_thread.readRegister(instr.source_register2);
            float result = alu.execute(instr.operation, val1, val2);
            current_thread.writeRegister(instr.destination_register, result);
        }
    }
}

void GPUCore::executeProgram(const std::vector<Instruction>& program) {
    for (const Instruction& instr : program) {
        executeInstruction(instr);
    }
}

void GPUCore::printRegister(int index) {
    for (RegisterFile& thread : threads) {
        std::cout << "Register " << index << " contains: " << thread.readRegister(index) << "\n";
    }
}

void GPUCore::printVRAM(int start_index, int end_index) {
    for (int i = start_index; i <= end_index; i++) {
        std::cout << vram.read(i) << "\t";
    }
    std::cout << "\n";
}