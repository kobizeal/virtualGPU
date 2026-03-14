#include "hardware.hpp"
#include "kernel.hpp"

int main() {
    GPUCore core;
    int width = 2;

    std::vector<float> array_A = {1, 2, 3, 4};
    std::vector<float> array_B = {5, 6, 7, 8};

    core.copyHostToDevice(0, array_A);
    core.copyHostToDevice(4, array_B);

    std::vector<Instruction> my_program = buildMatrixMultKernel(width);

    core.executeProgram(my_program);

    core.printVRAM(8, 11);

    return 0;
}