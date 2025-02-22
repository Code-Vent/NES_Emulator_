//
// Created by Code-Vent on 2/16/2025.
//
#include<stdlib.h>
#include<stdbool.h>
#include"bus.h"
#include"peripheral.h"

struct Bus* allocBus() {
    struct Bus* bus = malloc(sizeof(struct Bus));
    assert(bus != NULL);
    for (int i = 0; i < MAX_PERIPHERALS; ++i)
        bus->peripherals[i] = NULL;
    bus->count = 0;
    bus->error = false;
    return  bus;
}

void deallocBus(struct Bus* bus) {
    assert(bus != NULL);
    for (int i = 0; i < MAX_PERIPHERALS; ++i)
        if (bus->peripherals[i] != NULL)
            free(bus->peripherals[i]);
    if (bus != NULL)
        free(bus);
}

struct Peripheral* addPeripheral(struct Bus* bus, uint16_t start_address, uint16_t end_address) {
    if (bus->count < MAX_PERIPHERALS) {
        bus->peripherals[bus->count] = allocPeripheral(start_address, end_address);
        return bus->peripherals[bus->count++];
    }
    bus->error = true;
    return NULL;
}
bool write(struct Bus* bus, uint16_t address, uint8_t data) {
    bus->last_address = address;
    for (int i = 0; i < MAX_PERIPHERALS; ++i) {
        if (containsAddress(bus->peripherals[i], address)) {
            if (bus->peripherals[i]->write != NULL)
                bus->peripherals[i]->write(address, data);
            return true;
        }
    }
    bus->error = true;
    return false;
}

uint8_t read(struct Bus* bus, uint16_t address) {
    bus->last_address = address;
    for (int i = 0; i < MAX_PERIPHERALS; ++i) {
        if (containsAddress(bus->peripherals[i], address)) {
            if (bus->peripherals[i]->read != NULL)
                return bus->peripherals[i]->read(address);
        }
    }
    bus->error = true;
    return 0;
}
