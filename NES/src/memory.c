//
// Created by Code-Vent on 2/16/2025.
//
#include"memory.h"
#include<stdint.h>
#include<assert.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>

#define RAM_SIZE 2048
#define CHR_RAM_SIZE 0x3F00
#define PGR_ROM_SIZE 0xBFE0
#define PPU_PALETTE_SIZE 0x20
#define PPU_OAM_SIZE 0x100
#define STACK_SIZE 256

struct Ram {
    uint16_t start_address;
    uint8_t data[RAM_SIZE + STACK_SIZE];
}ram;

struct ChrRam {
    uint16_t start_address;
    uint8_t data[CHR_RAM_SIZE];
}chr_ram;

struct PrgRom {
    uint16_t start_address;
    uint8_t data[PGR_ROM_SIZE];
}prg_rom;

struct Stack {
    uint16_t start_address;
}stack;

struct PPUPalette {
    uint16_t start_address;
    uint8_t data[PPU_PALETTE_SIZE];
}palette;

struct PPUOAM {
    uint16_t start_address;
    uint8_t data[PPU_OAM_SIZE];
}oam;



uint8_t readRam(uint16_t address) {
    return ram.data[(address & 0x7FF) - ram.start_address];
}

void writeRam(uint16_t address, uint8_t value) {

    ram.data[(address & 0x7FF) - ram.start_address] = value;
}

uint8_t readChrRam(uint16_t address) {
    return chr_ram.data[address - chr_ram.start_address];
}

void writeChrRam(uint16_t address, uint8_t value) {
    chr_ram.data[address - chr_ram.start_address] = value;
}

uint8_t readPrgRom(uint16_t address) {
    return prg_rom.data[address - prg_rom.start_address];
}

void writePrgRom(uint16_t address, uint8_t value) {
    prg_rom.data[address - prg_rom.start_address] = value;
}

uint8_t readPalette(uint16_t address) {
    return palette.data[(address & 0x3FFF) - palette.start_address];
}
void writePalette(uint16_t address, uint8_t value) {
    palette.data[(address & 0x3FFF) - palette.start_address] = value;
}

uint8_t readOAM(uint16_t address) {
    return oam.data[address - oam.start_address];
}
void writeOAM(uint16_t address, uint8_t value) {
    oam.data[address - oam.start_address] = value;
}

uint8_t readStack(uint16_t address) {
    return ram.data[address - stack.start_address];
}

void writeStack(uint16_t address, uint8_t value) {
    ram.data[address - stack.start_address] = value;
}

void allocRam(struct Bus* bus, uint16_t start_address, uint16_t size) {
    assert(bus != NULL);
    struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
    p->read = readRam;
    p->write = writeRam;
    ram.start_address = start_address;
}

void allocCHR_RAM(struct Bus* bus, uint16_t start_address, uint16_t size) {
    assert(bus != NULL);
    struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
    p->read = readChrRam;
    p->write = writeChrRam;
    chr_ram.start_address = start_address;
    srand(time(0));
    for (uint16_t i = 0; i < CHR_RAM_SIZE; ++i) {
        writeChrRam(start_address + i, rand());
    }
}

void allocPGR_ROM(struct Bus* bus, uint16_t start_address, uint16_t size) {
    assert(bus != NULL);
    struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
    p->read = readPrgRom;
    p->write = writePrgRom;
    prg_rom.start_address = start_address;
}

void allocStack(struct Bus* bus, uint16_t start_address, uint16_t size) {
    assert(bus != NULL);
    struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
    p->read = readStack;
    p->write = writeStack;
    stack.start_address = start_address;
}

void allocPalette(struct Bus* bus, uint16_t start_address, uint16_t size) {
    assert(bus != NULL);
    struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
    p->read = readPalette;
    p->write = writePalette;
    palette.start_address = start_address;
}

void allocOAM(struct Bus* bus, uint16_t start_address, uint16_t size) {
    assert(bus != NULL);
    struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
    p->read = readOAM;
    p->write = writeOAM;
    oam.start_address = start_address;
}