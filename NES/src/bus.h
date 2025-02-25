//
// Created by Code-Vent on 2/16/2025.
//
#include<stdint.h>
#include"peripheral.h"

#ifndef NES_BUS_H
#define NES_BUS_H

#define MAX_PERIPHERALS 15
struct Bus {
    struct Peripheral* peripherals[MAX_PERIPHERALS];
    bool error;
    uint8_t count;
    uint16_t last_address;
};

struct Bus* allocBus();
void deallocBus(struct Bus* bus);
struct Peripheral* addPeripheral(struct Bus* bus, uint16_t start_address, uint16_t end_address);
void write(struct Bus* bus, uint16_t address, uint8_t data);
uint8_t read(struct Bus* bus, uint16_t address);
struct Peripheral* findPeripheral(struct Bus* bus, uint16_t start_address);

#endif //NES_BUS_H
