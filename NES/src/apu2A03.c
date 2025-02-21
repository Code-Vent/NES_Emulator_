#include"apu2A03.h"
#include"peripheral.h"

#define APU_MEM_SIZE 64


struct _APU2A03 {
	uint16_t start_address;
	uint8_t regs[APU_MEM_SIZE];
}apu;

uint8_t readApuRegister(uint16_t address) {
	return apu.regs[address - apu.start_address];
}

void writeApuRegister(uint16_t address, uint8_t value) {
	apu.regs[address - apu.start_address] = value;
}


APU2A03* allocAPU(struct Bus* bus, uint16_t start_address, uint16_t size)
{
	APU2A03* apu = malloc(sizeof(struct _APU2A03));
	if (apu != NULL) {
		struct Peripheral* p = addPeripheral(bus, start_address, start_address + size - 1);
		p->read = readApuRegister;
		p->write = writeApuRegister;
		apu->start_address = start_address;
	}
	return apu;
}

void deallocAPU(APU2A03* apu) {
	free(apu);
}
