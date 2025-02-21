//
// Created by Code-Vent on 2/16/2025.
//

#include"cpu6502.h"
#include"peripheral.h"

#define X_REG_ADDR 0x1100
#define Y_REG_ADDR 0x1101
#define A_REG_ADDR 0x1102

#define FLAG_CARRY 0x01
#define FLAG_ZERO 0x02
#define FLAG_INTERRUPT 0x04
#define FLAG_DECIMAL 0x08
#define FLAG_B 0x10
#define FLAG_ALWAYS_HIGH 0x20
#define FLAG_OVERFLOW 0x40
#define FLAG_NEGATIVE 0x80


static struct Registers core;

struct _CPU6502 {
    struct Bus* bus;
    int16_t     data;
    uint16_t    stack_base;
    bool        error;
    uint8_t     clock_cycles;
    uint8_t     opcode;
};

uint8_t readCoreRegister(uint16_t address) {
    switch (address) {
    case X_REG_ADDR:
        return core.x;
    case Y_REG_ADDR:
        return core.y;
    case A_REG_ADDR:
        return core.a;
    default:
        return 0xEA;
    }
}

void writeCoreRegister(uint16_t address, uint8_t value) {
    switch (address) {
    case X_REG_ADDR:
        core.x = value;
        break;
    case Y_REG_ADDR:
        core.y = value;
        break;
    case A_REG_ADDR:
        core.a = value;
        break;
    default:
        break;
    }
}

typedef void(*core_func)(CPU6502*);
void push(CPU6502* cpu, uint8_t byte);
uint8_t pop(CPU6502* cpu);
void ADC(CPU6502*);
void AND(CPU6502*);
void ASL(CPU6502*);
void BCC(CPU6502*);
void BCS(CPU6502*);
void BEQ(CPU6502*);
void BIT(CPU6502*);
void BMI(CPU6502*);
void BNE(CPU6502*);
void BPL(CPU6502*);
void BRK(CPU6502*);
void BVC(CPU6502*);
void BVS(CPU6502*);
void CLC(CPU6502*);
void CLD(CPU6502*);
void CLI(CPU6502*);
void CLV(CPU6502*);
void CMP(CPU6502*);
void CPX(CPU6502*);
void CPY(CPU6502*);
void DEC(CPU6502*);
void DEX(CPU6502*);
void DEY(CPU6502*);
void EOR(CPU6502*);
void ORA(CPU6502*);
void STA(CPU6502*);
void SBC(CPU6502*);
void LDA(CPU6502*);
void ROL(CPU6502*); 
void LSR(CPU6502*); 
void ROR(CPU6502*); 
void STX(CPU6502*); 
void LDX(CPU6502*); 
void INC(CPU6502*);
void JSR(CPU6502*);
void RTI(CPU6502*);
void RTS(CPU6502*);
void LDY(CPU6502*);
void STY(CPU6502*);
void PHP(CPU6502*);
void PLP(CPU6502*);
void SEC(CPU6502*);
void PHA(CPU6502*);
void PLA(CPU6502*);
void SEI(CPU6502*);
void TYA(CPU6502*);
void TAY(CPU6502*);
void SED(CPU6502*);
void TXA(CPU6502*);
void TXS(CPU6502*);
void TAX(CPU6502*);
void TSX(CPU6502*);
void JMP(CPU6502*);
void INX(CPU6502*);
void INY(CPU6502*);
void NOP(CPU6502* cpu) { return; }
void NUL(CPU6502* cpu) { cpu->error = true; }

////////////////////////////////////////////////////////////////////////////////////////////
/////// ADDRESSING MODES
///////////////////////////////////////////////////////////////////////////////////////////
void IMP(CPU6502*);// IMPLICIT
void IMM(CPU6502*);// IMMEDIATE
void ZPA(CPU6502*);// ZERO_PAGE
void ZPX(CPU6502*);// ZERO_PAGE_INDEX | X_INDEXED
void ZPY(CPU6502*);// ZERO_PAGE_INDEX | Y_INDEXED
void ABS(CPU6502*);// ABSOLUTE
void ABX(CPU6502*);// ABSOLUTE_INDEXED | X_INDEXED
void ABY(CPU6502*);// ABSOLUTE_INDEXED | Y_INDEXED
void XIND(CPU6502*);// INDEXED_INDIRECT | X_INDEXED
void INDY(CPU6502*);// INDIRECT_INDEXED | Y_INDEXED
void ACC(CPU6502*);// ACCUMULATOR
void REL(CPU6502*);// RELATIVE
void IND(CPU6502*);// INDIRECT

typedef void(*core_func)(CPU6502*);
core_func instructions[16][16] = {
    {BRK, ORA, NUL, NUL, NUL, ORA, ASL, NUL, PHP, ORA, ASL, NUL, NUL, ORA, ASL, NUL},
    {BPL, ORA, NUL, NUL, NUL, ORA, ASL, NUL, CLC, ORA, NUL, NUL, NUL, ORA, ASL, NUL},
    {JSR, AND, NUL, NUL, BIT, AND, ROL, NUL, PLP, AND, ROL, NUL, BIT, AND, ROL, NUL},
    {BMI, AND, NUL, NUL, NUL, AND, ROL, NUL, SEC, AND, NUL, NUL, NUL, AND, ROL, NUL},
    {RTI, EOR, NUL, NUL, NUL, EOR, LSR, NUL, PHA, EOR, LSR, NUL, JMP, EOR, LSR, NUL},
    {BVC, EOR, NUL, NUL, NUL, EOR, LSR, NUL, CLI, EOR, NUL, NUL, NUL, EOR, LSR, NUL},
    {RTS, ADC, NUL, NUL, NUL, ADC, ROR, NUL, PLA, ADC, ROR, NUL, JMP, ADC, ROR, NUL},
    {BVS, ADC, NUL, NUL, NUL, ADC, ROR, NUL, SEI, ADC, NUL, NUL, NUL, ADC, ROR, NUL},
    {NUL, STA, NUL, NUL, STY, STA, STX, NUL, DEY, NUL, TXA, NUL, STY, STA, STX, NUL},
    {BCC, STA, NUL, NUL, STY, STA, STX, NUL, TYA, STA, TXS, NUL, NUL, STA, NUL, NUL},
    {LDY, LDA, LDX, NUL, LDY, LDA, LDX, NUL, TAY, LDA, TAX, NUL, LDY, LDA, LDX, NUL},
    {BCS, LDA, NUL, NUL, LDY, LDA, LDX, NUL, CLV, LDA, TSX, NUL, LDY, LDA, LDX, NUL},
    {CPY, CMP, NUL, NUL, CPY, CMP, DEC, NUL, INY, CMP, DEX, NUL, CPY, CMP, DEC, NUL},
    {BNE, CMP, NUL, NUL, NUL, CMP, DEC, NUL, CLD, CMP, NUL, NUL, NUL, CMP, DEC, NUL},
    {CPX, SBC, NUL, NUL, CPX, SBC, INC, NUL, INX, SBC, NOP, NUL, CPX, SBC, INC, NUL},
    {BEQ, SBC, NUL, NUL, NUL, SBC, INC, NUL, SED, SBC, NUL, NUL, NUL, SBC, INC, NUL} 
};

core_func addressing_modes[16][16] = {
    {IMP, XIND, NUL, NUL, NUL, ZPA, ZPA, NUL,  IMP, IMM,  ACC,  NUL,  NUL,  ABS,  ABS,  NUL}, // 0x00 - 0x0F
    {REL, INDY, NUL, NUL, NUL, ZPX, ZPX, NUL,  IMP, ABY,  NUL,  NUL,  NUL,  ABX,  ABX,  NUL}, // 0x10 - 0x1F
    {ABS, XIND, NUL, NUL, ZPA, ZPA, ZPA, NUL,  IMP, IMM,  ACC,  NUL,  ABS,  ABS,  ABS,  NUL}, // 0x20 - 0x2F
    {REL, INDY, NUL, NUL, NUL, ZPX, ZPX, NUL,  IMP, ABY,  NUL,  NUL,  NUL,  ABX,  ABX,  NUL}, // 0x30 - 0x3F
    {IMP, XIND, NUL, NUL, NUL, ZPA, ZPA, NUL,  IMP, IMM,  ACC,  NUL,  ABS,  ABS,  ABS,  NUL}, // 0x40 - 0x4F
    {REL, INDY, NUL, NUL, NUL, ZPX, ZPX, NUL,  IMP, ABY,  NUL,  NUL,  NUL,  ABX,  ABX,  NUL}, // 0x50 - 0x5F
    {IMP, XIND, NUL, NUL, NUL, ZPA, ZPA, NUL,  IMP, IMM,  ACC,  NUL,  IND,  ABS,  ABS,  NUL}, // 0x60 - 0x6F
    {REL, INDY, NUL, NUL, NUL, ZPX, ZPX, NUL,  IMP, ABY,  NUL,  NUL,  NUL,  ABX,  ABX,  NUL}, // 0x70 - 0x7F
    {NUL, XIND, NUL, NUL, ZPA, ZPA, ZPA, NUL,  IMP, NUL,  IMP,  NUL,  ABS,  ABS,  ABS,  NUL}, // 0x80 - 0x8F
    {REL, INDY, NUL, NUL, ZPX, ZPX, ZPY, NUL,  IMP, ABS,  IMP,  NUL,  NUL,  ABX,  NUL,  NUL}, // 0x90 - 0x9F
    {IMM, XIND, IMM, NUL, ZPA, ZPA, ZPA, NUL,  IMP, IMM,  IMP,  NUL,  ABS,  ABS,  ABS,  NUL}, // 0xA0 - 0xAF
    {REL, INDY, NUL, NUL, ZPX, ZPX, ZPY, NUL,  IMP, ABY,  IMP,  NUL,  ABX,  ABX,  ABY,  NUL}, // 0xB0 - 0xBF
    {IMM, XIND, NUL, NUL, ZPA, ZPA, ZPA, NUL,  IMP, IMM,  IMP,  NUL,  ABS,  ABS,  ABS,  NUL}, // 0xC0 - 0xCF
    {REL, INDY, NUL, NUL, NUL, ZPX, ZPX, NUL,  IMP, ABY,  NUL,  NUL,  NUL,  ABX,  ABX,  NUL}, // 0xD0 - 0xDF
    {IMM, XIND, NUL, NUL, ZPA, ZPA, ZPA, NUL,  IMP, IMM,  IMP,  NUL,  ABS,  ABS,  ABS,  NUL}, // 0xE0 - 0xEF
    {REL, INDY, NUL, NUL, NUL, ZPX, ZPX, NUL,  IMP, ABY,  NUL,  NUL,  NUL,  ABX,  ABX,  NUL}  // 0xF0 - 0xFF
};




CPU6502* allocCPU(struct Bus* bus, uint16_t stack_base_addr)
{
    CPU6502* cpu = malloc(sizeof(struct _CPU6502));
    if (cpu != NULL) {
        cpu->stack_base = stack_base_addr;
        cpu->bus = bus;
        struct Peripheral* p = addPeripheral(bus, X_REG_ADDR, A_REG_ADDR);
        p->write = writeCoreRegister;
        p->read = readCoreRegister;
    }
    return cpu;
}

void deallocCPU(CPU6502* cpu) {
    assert(cpu != NULL);
    free(cpu);
}

struct Registers getRegisters(CPU6502* cpu) {
    assert(cpu != NULL);
    return core;
}

bool execute(CPU6502* cpu) {
    assert(cpu != NULL);

    cpu->opcode = read(cpu->bus, core.pc++);
    uint8_t row = (cpu->opcode & 0xF0) >> 4;
    uint8_t col = (cpu->opcode & 0x0F);
    addressing_modes[row][col](cpu);
    instructions[row][col](cpu);
    
    return !cpu->error;
}

void hardReset(CPU6502* cpu) {
    if (cpu == NULL)
        return;
    assert(cpu->bus != NULL);
    core.pc = (read(cpu->bus, 0xFFFD) << 8) | read(cpu->bus, 0xFFFC);
    core.x = core.y = 0;
    core.a = 0;
    core.sr = FLAG_ALWAYS_HIGH | FLAG_INTERRUPT;
    core.sp = 0xFF;
    cpu->data = 0;
    cpu->error = false;
}

void push(CPU6502* cpu, uint8_t byte) {
    assert(core.sp > 0x00);
    write(cpu->bus, cpu->stack_base + core.sp, byte);
    core.sp--;
}

uint8_t pop(CPU6502* cpu) {
    assert(core.sp < 0xFF);
    core.sp++;
    return read(cpu->bus, cpu->stack_base + core.sp);
}

void ADC(CPU6502* cpu) {
    assert(cpu->bus);
    uint8_t carry = (core.sr & FLAG_CARRY);
    uint16_t result = core.a + cpu->data + carry;
    if (result > 0xFF) core.sr |= FLAG_CARRY;
    if (result == 0) core.sr |= FLAG_ZERO;
    if ((result ^ core.a) & (result ^ cpu->data) & 0x80) core.sr |= FLAG_OVERFLOW;
    if (result | FLAG_NEGATIVE) core.sr |= FLAG_NEGATIVE;
    core.a = (uint8_t)result;
}

void AND(CPU6502* cpu) {
    assert(cpu->bus);
    core.a &= (cpu->data & 0x00FF);
    if (core.a == 0) core.sr |= FLAG_ZERO;
    if (core.a & FLAG_NEGATIVE) core.sr |= FLAG_NEGATIVE;
}

void ASL(CPU6502* cpu) {
    assert(cpu->bus);
    core.sr |= ((uint8_t)(cpu->data >> 7) & FLAG_CARRY);
    cpu->data <<= 1;
    if(cpu->data == 0) core.sr |= FLAG_ZERO;
    if (cpu->data & FLAG_NEGATIVE) core.sr |= FLAG_NEGATIVE;
    write(cpu->bus, cpu->bus->last_address, (uint8_t)cpu->data);
}

void BCC(CPU6502* cpu) {
    if (~(core.sr & FLAG_CARRY))
        core.pc += cpu->data;
}

void BCS(CPU6502* cpu) {
    if (core.sr & FLAG_CARRY)
        core.pc += cpu->data;
}

void BEQ(CPU6502* cpu) {
    if (core.sr & FLAG_ZERO)
        core.pc += cpu->data;
}

void BIT(CPU6502* cpu) {
    uint8_t result = (uint8_t)cpu->data & core.a;
    core.sr |= ((uint8_t)cpu->data & FLAG_OVERFLOW & FLAG_NEGATIVE);
    if (result == 0)
        core.sr |= FLAG_ZERO;
}

void BMI(CPU6502* cpu) {
    if (core.sr & FLAG_NEGATIVE)
        core.pc += cpu->data;
}

void BNE(CPU6502* cpu) {
    if (~(core.sr & FLAG_ZERO))
        core.pc += cpu->data;
}

void BPL(CPU6502* cpu) {
    if (~(core.sr & FLAG_NEGATIVE))
        core.pc += cpu->data;
}

void BRK(CPU6502* cpu) {
    core.pc++;
    push(cpu, (core.pc >> 8) & 0x00FF);
    push(cpu, core.pc & 0x00FF);
    core.sr |= FLAG_INTERRUPT;
    core.sr |= FLAG_B;
    push(cpu, core.sr);
    
    core.sr &= ~FLAG_B;

    core.pc = (uint16_t)read(cpu->bus, 0xFFFE) | ((uint16_t)read(cpu->bus, 0xFFFF) << 8);
}

void BVC(CPU6502* cpu) {
    if (~(core.sr & FLAG_OVERFLOW))
        core.pc += cpu->data;
}

void BVS(CPU6502* cpu) {
    if (core.sr | FLAG_OVERFLOW)
        core.pc += cpu->data;
}

void CLC(CPU6502* cpu) {
    core.sr &= ~FLAG_CARRY;
}

void CLD(CPU6502* cpu) {
    core.sr &= ~FLAG_DECIMAL;
}

void CLI(CPU6502* cpu) {
    core.sr &= ~FLAG_INTERRUPT;
}

void CLV(CPU6502* cpu) {
    core.sr &= ~FLAG_OVERFLOW;
}

void CMP(CPU6502* cpu) {
    if (core.a >= (uint8_t)cpu->data) 
        core.sr |= FLAG_CARRY;
    else
        core.sr |= FLAG_NEGATIVE;
    if (core.a == (uint8_t)cpu->data)core.sr |= FLAG_ZERO;
}

void CPX(CPU6502* cpu) {
    if (core.x >= (uint8_t)cpu->data)
        core.sr |= FLAG_CARRY;
    else
        core.sr |= FLAG_NEGATIVE;
    if (core.x == (uint8_t)cpu->data)core.sr |= FLAG_ZERO;
}

void CPY(CPU6502* cpu) {
    if (core.y >= (uint8_t)cpu->data)
        core.sr |= FLAG_CARRY;
    else
        core.sr |= FLAG_NEGATIVE;
    if (core.y == (uint8_t)cpu->data)core.sr |= FLAG_ZERO;
}

void STA(CPU6502* cpu) {
    write(cpu->bus, cpu->bus->last_address, core.a);
}

void LDA(CPU6502* cpu) {
    core.a = (uint8_t)cpu->data;// read(cpu->bus, cpu->bus->last_address);
}

void SBC(CPU6502* cpu) {
    uint8_t val = ~cpu->data;
    uint8_t result = core.a + val + (core.sr & FLAG_CARRY);
    if ((int8_t)core.a < 0)core.sr |= FLAG_CARRY;
    if (core.a & 0x80)core.sr |= FLAG_NEGATIVE;
    if (core.a == 0)core.sr |= FLAG_ZERO;
    if ((result ^ core.a) & (result ^ val) & 0x80)core.sr |= FLAG_OVERFLOW;
    core.a = result;
}

void DEC(CPU6502* cpu) {
    uint8_t val = (uint8_t)cpu->data - 1;
    if (val & 0x80)
        core.sr |= FLAG_NEGATIVE;
    if (0 == val)core.sr |= FLAG_ZERO;
    write(cpu->bus, cpu->bus->last_address, val);
}

void DEX(CPU6502* cpu) {
    cpu->data = read(cpu->bus, X_REG_ADDR);
    DEC(cpu);
}

void DEY(CPU6502* cpu) {
    cpu->data = read(cpu->bus, Y_REG_ADDR);
    DEC(cpu);
}

void EOR(CPU6502* cpu) {
    core.a ^= (uint8_t)cpu->data;
    if (core.a & 0x80)
        core.sr |= FLAG_NEGATIVE;
    if (0 == core.a)core.sr |= FLAG_ZERO;
}

void ORA(CPU6502* cpu) {
    core.a |= (uint8_t)cpu->data;
    if (core.a & 0x80)
        core.sr |= FLAG_NEGATIVE;
    if (0 == core.a)core.sr |= FLAG_ZERO;
}

void ROL(CPU6502*cpu)
{
    uint16_t temp = (uint16_t)(cpu->data << 1) | (core.sr & FLAG_CARRY);
    if(temp & 0xFF00)
        core.sr |= FLAG_CARRY;
    if((temp & 0x00FF) == 0x0000)
        core.sr |= FLAG_ZERO;
    if(temp & 0x0080)
        core.sr |= FLAG_NEGATIVE;
    
    write(cpu->bus, cpu->bus->last_address, temp & 0x00FF);
}

void LSR(CPU6502* cpu) {
    if(cpu->data & 0x0001)
        core.sr |= FLAG_CARRY;
    uint16_t temp = cpu->data >> 1;
   if((temp & 0x00FF) == 0x0000)
       core.sr |= FLAG_ZERO;
    if(temp & 0x0080)
        core.sr |= FLAG_NEGATIVE;
    write(cpu->bus, cpu->bus->last_address, temp & 0x00FF);
}

void ROR(CPU6502* cpu) {
    uint16_t temp = (uint16_t)(core.sr & FLAG_CARRY << 7) | (cpu->data >> 1);
    if(cpu->data & 0x01)
        core.sr |= FLAG_CARRY;
    if((temp & 0x00FF) == 0x00)
        core.sr |= FLAG_ZERO;
    if(temp & 0x0080)
        core.sr |= FLAG_NEGATIVE;
    write(cpu->bus, cpu->bus->last_address, temp & 0x00FF);
}

void STX(CPU6502* cpu) {
    write(cpu->bus, cpu->bus->last_address, core.x);
}

void LDX(CPU6502* cpu) {
    core.x = (uint8_t)cpu->data;// read(cpu->bus, cpu->bus->last_address);
}

void INC(CPU6502* cpu) {
    cpu->data += 1;
    write(cpu->bus, cpu->bus->last_address, cpu->data & 0x00FF);
    if((cpu->data & 0x00FF) == 0x0000)
        core.sr |= FLAG_ZERO;
    if(cpu->data & 0x0080)
        FLAG_NEGATIVE;
}

void JSR(CPU6502* cpu) {
    core.pc--;
    push(cpu, (core.pc >> 8) & 0x00FF);
    push(cpu, core.pc & 0x00FF);
    core.pc = cpu->data;
}

void RTI(CPU6502* cpu) {
    core.sr = pop(cpu);
    core.sr |= FLAG_ALWAYS_HIGH;
    core.pc = pop(cpu);
    core.pc |= pop(cpu) << 8;
}

void RTS(CPU6502* cpu) {
    core.pc = pop(cpu);
    core.pc |= pop(cpu) << 8;
    core.pc++;
}

void LDY(CPU6502* cpu) {
    core.y = (uint8_t)cpu->data;
    if(core.y == 0x00) core.sr |= FLAG_ZERO;
    if(core.y & 0x80) core.sr |= FLAG_NEGATIVE;
}

void STY(CPU6502* cpu) {
    write(cpu->bus, cpu->bus->last_address, core.y);
}

void PHP(CPU6502* cpu) {
    push(cpu, core.sr & FLAG_B | FLAG_ALWAYS_HIGH);
    core.sr &= (~FLAG_B & ~FLAG_ALWAYS_HIGH);
}

void PLP(CPU6502* cpu) {
    core.sr = pop(cpu);
    core.sp |= FLAG_ALWAYS_HIGH;
}

void SEC(CPU6502* cpu) {
    core.sp |= FLAG_CARRY;
}

void PHA(CPU6502* cpu) {
    push(cpu, core.a);
}

void PLA(CPU6502* cpu) {
    core.a = pop(cpu);
    if(core.a == 0x00) core.sp |= FLAG_ZERO;
    if(core.a & 0x80) core.sp |= FLAG_NEGATIVE;
}

void SEI(CPU6502* cpu) {
    core.sp |= FLAG_INTERRUPT;
}

void TYA(CPU6502* cpu) {
    core.a = core.y;
    if (core.a == 0x00) core.sp |= FLAG_ZERO;
    if (core.a & 0x80) core.sp |= FLAG_NEGATIVE;
}

void TAY(CPU6502* cpu) {
    core.y = core.a;
    if (core.y == 0x00) core.sp |= FLAG_ZERO;
    if (core.y & 0x80) core.sp |= FLAG_NEGATIVE;
}

void SED(CPU6502* cpu) {
    core.sp |= FLAG_DECIMAL;
}

void TXA(CPU6502* cpu) {
    core.a = core.x;
    if (core.a == 0x00) core.sp |= FLAG_ZERO;
    if (core.a & 0x80) core.sp |= FLAG_NEGATIVE;
}

void TXS(CPU6502* cpu) {
    core.sp = core.x;
}

void TAX(CPU6502* cpu) {
    core.x = core.a;
    if (core.x == 0x00) core.sp |= FLAG_ZERO;
    if (core.x & 0x80) core.sp |= FLAG_NEGATIVE;
}

void TSX(CPU6502* cpu) {
    core.x = core.sp;
    if (core.x == 0x00) core.sp |= FLAG_ZERO;
    if (core.x & 0x80) core.sp |= FLAG_NEGATIVE;
}

void INX(CPU6502* cpu) {
    cpu->data = read(cpu->bus, X_REG_ADDR);
    INC(cpu);
}

void INY(CPU6502* cpu) {
    cpu->data = read(cpu->bus, Y_REG_ADDR);
    INC(cpu);
}

void JMP(CPU6502* cpu) {
    core.pc = cpu->bus->last_address;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//// ADDRESSING MODES
//////////////////////////////////////////////////////////////////////////////////////////////////


void IMP(CPU6502* cpu) {
}

void XIND(CPU6502* cpu) {
    uint16_t t = read(cpu->bus, core.pc);
    core.pc++;
    uint16_t lo = read(cpu->bus, (uint16_t)(t + (uint16_t)core.x) & 0x00FF);
    uint16_t hi = read(cpu->bus, (uint16_t)(t + (uint16_t)core.x + 1) & 0x00FF);

    uint16_t addr_abs = (hi << 8) | lo;

    cpu->data = read(cpu->bus, addr_abs);
}

void IMM(CPU6502* cpu) {
    cpu->data = read(cpu->bus, core.pc); 
    core.pc++;
}

void ABS(CPU6502* cpu) {
    uint16_t lo = read(cpu->bus, core.pc);
    core.pc++;
    uint16_t hi = read(cpu->bus, core.pc);
    core.pc++;

    uint16_t address = (hi << 8) | lo;
    cpu->data = read(cpu->bus, address);
    
}

void IND(CPU6502* cpu) {
    uint16_t ptr_lo = read(cpu->bus, core.pc);
    core.pc++;
    uint16_t ptr_hi = read(cpu->bus, core.pc);
    core.pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;
    uint16_t addr_abs = 0;
    if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
    {
        addr_abs = (read(cpu->bus, ptr & 0xFF00) << 8) | read(cpu->bus, ptr + 0);
    }
    else // Behave normally
    {
        addr_abs = (read(cpu->bus, ptr + 1) << 8) | read(cpu->bus, ptr + 0);
    }
    cpu->data = read(cpu->bus, addr_abs);
}

void INDY(CPU6502* cpu) {
    uint16_t t = read(cpu->bus, core.pc);
    core.pc++;
    uint16_t lo = read(cpu->bus, t & 0x00FF);
    uint16_t hi = read(cpu->bus, (t + 1) & 0x00FF);

    uint16_t addr_abs = (hi << 8) | lo;
    addr_abs += core.y;

    if ((addr_abs & 0xFF00) != (hi << 8))
        cpu->clock_cycles++;
    cpu->data = read(cpu->bus, addr_abs);

}

void ZPA(CPU6502* cpu) {
    uint16_t address = read(cpu->bus, core.pc);
    cpu->data = read(cpu->bus, address & 0x00FF);
    core.pc++;
}

void ZPX(CPU6502* cpu) {
    uint16_t address = read(cpu->bus, core.pc) + core.x;
    cpu->data = read(cpu->bus, address & 0x00FF);
    core.pc++;
}

void ZPY(CPU6502* cpu) {
    uint16_t address = read(cpu->bus, core.pc) + core.y;
    cpu->data = read(cpu->bus, address & 0x00FF);
    core.pc++;
}

void ABY(CPU6502* cpu) {
    uint16_t lo = read(cpu->bus, core.pc);
    core.pc++;
    uint16_t hi = read(cpu->bus, core.pc);
    core.pc++;

    uint16_t address = (hi << 8) | lo;
    address += core.y;
    if ((address & 0xFF00) != (hi << 8))
        cpu->clock_cycles++;
    cpu->data = read(cpu->bus, address);
}

void ABX(CPU6502* cpu) {
    uint16_t lo = read(cpu->bus, core.pc);
    core.pc++;
    uint16_t hi = read(cpu->bus, core.pc);
    core.pc++;

    uint16_t address = (hi << 8) | lo;
    address += core.x;
    if ((address & 0xFF00) != (hi << 8))
        cpu->clock_cycles++;
    cpu->data = read(cpu->bus, address);
}

void REL(CPU6502* cpu) {
    cpu->data = (int8_t)read(cpu->bus, core.pc);
    core.pc++;
}

void ACC(CPU6502* cpu) {
    cpu->data = read(cpu->bus, A_REG_ADDR);
}