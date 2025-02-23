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
#define VRAM_SIZE 0x4000
#define CARTRIDGE_SIZE 0xBFE0
#define STACK_SIZE 256

struct Ram {
    uint16_t start_address;
    uint8_t data[RAM_SIZE + STACK_SIZE];
}ram;

struct VRam {
    uint16_t start_address;
    uint8_t data[VRAM_SIZE];
}v_ram;

struct Stack {
    uint16_t start_address;
}stack;

struct Cartridge {
    uint16_t start_address;
    uint8_t data[CARTRIDGE_SIZE];
}cartridge;



uint8_t readRam(uint16_t address) {
    return ram.data[(address & 0x7FF) - ram.start_address];
}

void writeRam(uint16_t address, uint8_t value) {

    ram.data[(address & 0x7FF) - ram.start_address] = value;
}

uint8_t readVRam(uint16_t address) {
    return v_ram.data[address - v_ram.start_address];
}

void writeVRam(uint16_t address, uint8_t value) {
    v_ram.data[address - v_ram.start_address] = value;
}

uint8_t readCartridge(uint16_t address) {
    return cartridge.data[address - cartridge.start_address];
}
void writeCartridge(uint16_t address, uint8_t value) {
    cartridge.data[address - cartridge.start_address] = value;
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

void allocVRam(struct Bus* bus, uint16_t start_address, uint16_t size) {
    assert(bus != NULL);
    struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
    p->read = readVRam;
    p->write = writeVRam;
    v_ram.start_address = start_address;
    srand(time(0));
    for (uint16_t i = 0; i < VRAM_SIZE; ++i) {
        writeVRam(start_address + i, rand());
    }
}

void allocStack(struct Bus* bus, uint16_t start_address, uint16_t size) {
    assert(bus != NULL);
    struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
    p->read = readStack;
    p->write = writeStack;
    stack.start_address = start_address;
}

void allocCartridge(struct Bus* bus, uint16_t start_address, uint16_t size) {
    assert(bus != NULL);
    struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
    p->read = readCartridge;
    p->write = writeCartridge;
    cartridge.start_address = start_address;
    cartridge.data[0] = 0xCA;
}

uint8_t directReadCartridge(uint16_t address) {
    assert(address < CARTRIDGE_SIZE);
    return cartridge.data[address];
}
void directWriteCartridge(uint16_t address, uint8_t value) {
    assert(address < CARTRIDGE_SIZE);
    cartridge.data[address] = value;
}