//
// Created by Tao on 2/16/2025.
//

#include"peripheral.h"


struct Peripheral* allocPeripheral(uint16_t start_address, uint16_t end_address) {
    struct Peripheral* p = (struct Peripheral*)malloc(sizeof(struct Peripheral));
    assert(p != NULL);
    p->start_address = start_address;
    p->end_address = end_address;
    p->read = NULL;
    p->write = NULL;
    return p;
}

bool containsAddress(struct Peripheral* p, uint16_t addr) {
    if (p == NULL)
        return false;
    if (p->start_address <= addr && p->end_address >= addr)
        return true;
    return false;
}
