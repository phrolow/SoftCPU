#include <stdlib.h>

int main() {
    system("Y:/gthb/SoftCPU/asm/asm.exe prog.asm prog.bin");
    system("Y:/gthb/SoftCPU/cpu/cpu.exe Y:/gthb/SoftCPU/asm/prog.bin");

    return 0;
}