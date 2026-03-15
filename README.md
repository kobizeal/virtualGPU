# Virtual GPU Simulator

A software-based Graphics Processing Unit (GPU) simulator written from scratch in modern C++ (C++17). 

This project models the physical architecture and execution pipeline of a GPU. It features a custom Instruction Set Architecture (ISA), thread-local register files, a central Arithmetic Logic Unit (ALU), and Global Memory (VRAM). The current implementation successfully compiles and executes a multi-threaded 2x2 matrix multiplication kernel using manual loop-unrolling and Multiply-Accumulate (MAC) operations.

## Features

* **Custom Instruction Set Architecture (ISA):** Defines proprietary opcodes for memory management (`LOAD`, `STORE`, `LOAD_INDIRECT`), mathematics (`MULTIPLY`, `ADD`, `MODULO`), and hardware control (`SET_VALUE`, `MOV_TID`).
* **Hardware-Level SIMD Execution:** Simulates parallel thread execution where each thread calculates its own 2D memory strides (Row and Column indices) based on its physical hardware Thread ID.
* **Component-Based Silicon Design:** Separates the architectural logic into distinct hardware components: `GPUCore`, `GlobalMemory`, `RegisterFile`, and `ArithmeticLogicUnit`.
* **Modular "Cartridge" Kernels:** Program instructions are generated in a separate compiler-like kernel file and loaded into the GPU core for execution, mimicking the relationship between Host code and Device code in frameworks like CUDA.
* **CMake Build System:** fully integrated multi-file compilation using CMake.

## System Architecture

The simulator is split into three core layers:

1. **`isa.hpp` (The Blueprint):** The fundamental contract between the software and hardware, defining the `Instruction` struct and `OpCode` enumerations.
2. **`hardware.cpp` (The Silicon):** The physical components. 
    * `GlobalMemory`: A 1D `std::vector<float>` acting as VRAM.
    * `RegisterFile`: 16 general-purpose floating-point registers per thread.
    * `ALU`: Executes math opcodes using switch-case logic.
    * `GPUCore`: The control unit that fetches instructions, manages the thread pool, and routes data between VRAM, Registers, and the ALU.
3. **`kernel.cpp` (The Software):** Generates the vector of instructions (the assembly tape) that performs the operations.

## Example: Writing Assembly

The GPU executes a sequential stream of instructions. Here is a snippet of how the hardware fetches its physical Thread ID and calculates its target Matrix A Row Index:

```cpp
// 1. Get the physical Hardware Thread ID
Instruction instr1{};
instr1.operation = OpCode::MOV_TID;
instr1.destination_register = 1;
program.push_back(instr1);

// 2. Calculate RowIdx (ThreadID // MatrixWidth)
Instruction instr2{};
instr2.operation = OpCode::FLOOR_DIVIDE;
instr2.source_register1 = 1; // ThreadID
instr2.source_register2 = 0; // Matrix Width (pre-loaded)
instr2.destination_register = 2; // Store RowIdx in Reg 2
program.push_back(instr2);
```

## CMake and how to run program

This project uses CMake for dependency management and compilation.

Prerequisites:

A C++17 compatible compiler (GCC, Clang, or MSVC)
CMake (v3.10+)

Steps:

Clone the repository and navigate to the project directory.

Configure the build environment:
```Bash
cmake -S . -B build
```

Compile the executable:
```Bash
cmake --build build
```

Finally, run the program:
```
Linux/macOS: ./build/virtual_gpu
Windows: .\build\Debug\virtual_gpu.exe
```

## Theoretical Output

Within `kernel.cpp`, I created a custom instruction set to compute the dot product of two 2x2 matrices (A x B). 

Given the following matrices in VRAM:

```text
Matrix A:      Matrix B:      
[ 1  2 ]   x   [ 5  6 ]   
[ 3  4 ]       [ 7  8 ]
```
The hardware executes the multiply-accumulate kernel and outputs the correct resulting matrix to VRAM indices 8-11:
```text
Result (A x B):
[ 19  22 ]
[ 43  50 ]
```

## Expected Output

Because physical VRAM is a 1-dimensional contiguous array, the 2D matrices are flattened using row-major ordering. 

* **Matrix A** is loaded into VRAM indices `0, 1, 2, 3`.
* **Matrix B** is loaded into VRAM indices `4, 5, 6, 7`.

The hardware executes the multiply-accumulate kernel and outputs the calculated matrix into the next available memory block at indices `8, 9, 10, 11`.

```text
Resulting Matrix (A x B) stored at VRAM [8] through [11]:

Index:  [8]   [9]   [10]  [11]
Value:   19    22    43    50
```

```text
This is equivalent to what we found in the theoretical output!
[ 19  22 ]
[ 43  50 ]
```
