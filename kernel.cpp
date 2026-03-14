#include "kernel.hpp"

std::vector<Instruction> buildMatrixMultKernel(int width) {
    std::vector<Instruction> program;

    // Register overview:
    // 0: Width of Matrices
    // 1: ThreadIdx
    // 2: RowIdx (Index of Matrix A)
    // 3: ColIdx (Index of Matrix B)
    // 4: VRAM Address of A
    // 5: Value from VRAM Address (Matrix A)
    // 6: Base Address of B in VRAM
    // 7: VRAM Address of B
    // 8: Value from VRAM Address (Matrix B)
    // 9: Result of Multiplication (Register 5 * Register 8)
    // 10: Temporary Register to move by 1 to next column (Matrix A)
    // 11: Result of Second Multiplication
    // 12: Addition of First and Second Multiplication

    // Set first register to width of matrix for multiplication
    Instruction instr0{};
    instr0.operation = OpCode::SET_VALUE;
    instr0.source_register1 = width;
    instr0.destination_register = 0;
    program.push_back(instr0);

    // Get ThreadIdx
    Instruction instr1{};
    instr1.operation = OpCode::MOV_TID;
    instr1.destination_register = 1;
    program.push_back(instr1);

    // Begin Matrix Multiplication
    // Split in steps, first multiplication -> second multiplication -> add -> store in VRAM


    // FIRST MULTIPLICATION

    // Get RowIdx
    Instruction instr2{};
    instr2.operation = OpCode::FLOOR_DIVIDE;
    instr2.source_register1 = 1;
    instr2.source_register2 = 0;
    instr2.destination_register = 2;
    program.push_back(instr2);

    // Get ColumnIdx
    Instruction instr3{};
    instr3.operation = OpCode::MODULO;
    instr3.source_register1 = 1;
    instr3.source_register2 = 0;
    instr3.destination_register = 3;
    program.push_back(instr3);

    // Get VRAM Address for Matrix A
    Instruction instr4{};
    instr4.operation = OpCode::MULTIPLY;
    instr4.source_register1 = 2;
    instr4.source_register2 = 0;
    instr4.destination_register = 4;
    program.push_back(instr4);

    // Get Value of A (from VRAM address)
    Instruction instr5{};
    instr5.operation = OpCode::LOAD_INDIRECT;
    instr5.source_register1 = 4;
    instr5.destination_register = 5;
    program.push_back(instr5);

    // Set Base Address of B (Size of Matrix A: 4)
    Instruction instr6{};
    instr6.operation = OpCode::SET_VALUE;
    instr6.source_register1 = 4;
    instr6.destination_register = 6;
    program.push_back(instr6);

    // Get VRAM Address for Matrix B
    Instruction instr7{};
    instr7.operation = OpCode::ADD;
    instr7.source_register1 = 6;
    instr7.source_register2 = 3;
    instr7.destination_register = 7;
    program.push_back(instr7);

    // Get Value of B (from VRAM address)
    Instruction instr8{};
    instr8.operation = OpCode::LOAD_INDIRECT;
    instr8.source_register1 = 7;
    instr8.destination_register = 8;
    program.push_back(instr8);

    // Multiply A and B
    Instruction instr9{};
    instr9.operation = OpCode::MULTIPLY;
    instr9.source_register1 = 8;
    instr9.source_register2 = 5;
    instr9.destination_register = 9;
    program.push_back(instr9);

    // Second Multiplication

    // Need to move to next column in Matrix A
    // Temp register set to 1 so it can be added to VRAM
    Instruction instr10{};
    instr10.operation = OpCode::SET_VALUE;
    instr10.source_register1 = 1;
    instr10.destination_register = 10;
    program.push_back(instr10);

    // Add 1 to RowAddress (VRAM index)
    Instruction instr11{};
    instr11.operation = OpCode::ADD;
    instr11.source_register1 = 4;
    instr11.source_register2 = 10;
    instr11.destination_register = 4;
    program.push_back(instr11);

    // Get Value of A (from VRAM address)
    Instruction instr12{};
    instr12.operation = OpCode::LOAD_INDIRECT;
    instr12.source_register1 = 4;
    instr12.destination_register = 5;
    program.push_back(instr12);

    // Add offset to address of B (From width)
    Instruction instr13{};
    instr13.operation = OpCode::ADD;
    instr13.source_register1 = 7;
    instr13.source_register2 = 0;
    instr13.destination_register = 7;
    program.push_back(instr13);

    // Get Value of B (from VRAM address)
    Instruction instr14{};
    instr14.operation = OpCode::LOAD_INDIRECT;
    instr14.source_register1 = 7;
    instr14.destination_register = 8;
    program.push_back(instr14);

    // Multiply A and B
    Instruction instr15{};
    instr15.operation = OpCode::MULTIPLY;
    instr15.source_register1 = 8;
    instr15.source_register2 = 5;
    instr15.destination_register = 11;
    program.push_back(instr15);

    // Add First and Second Multiplication
    Instruction instr16{};
    instr16.operation = OpCode::ADD;
    instr16.source_register1 = 11;
    instr16.source_register2 = 9;
    instr16.destination_register = 12;
    program.push_back(instr16);

    // Store Result in VRAM
    Instruction instr17{};
    instr17.operation = OpCode::STORE;
    instr17.source_register1 = 12;
    instr17.destination_register = 8;
    program.push_back(instr17);
    
    return program;
}