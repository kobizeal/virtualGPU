#ifndef ISA_HPP
#define ISA_HPP

#include <vector>

enum class OpCode {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    FLOOR_DIVIDE, // Integer-style division
    MODULO,       // Remainder
    LOAD,         // Direct VRAM access (Base + TID)
    STORE,        // Write to VRAM (Base + TID)
    LOAD_INDIRECT,// Pointer-based VRAM access
    SET_VALUE,    // Load a literal number into a register
    MOV_TID       // Move physical Thread ID into a register
};

struct Instruction {
    OpCode operation;
    
    int source_register1 = 0;
    int source_register2 = 0;
    int destination_register = 0;
};

#endif