//
// Created by Code-Vent on 2/16/2025.
//
#include<stdint.h>
#include<stdbool.h>
#include"bus.h"

#ifndef NES_CPU6502_H
#define NES_CPU6502_H

//struct Clock



typedef struct _CPU6502 CPU6502;

struct Registers
{
    uint16_t pc;
    uint8_t a;
    uint8_t x, y;
    uint8_t sp;
    uint8_t sr;
};

CPU6502* allocCPU(struct Bus*, uint16_t stack_base_addr);
void getCPURegisters(struct Registers*);
void deallocCPU(CPU6502*);
void hardReset(CPU6502* cpu);
bool execute(CPU6502* cpu);

#endif //NES_CPU6502_H
