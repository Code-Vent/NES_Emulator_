#include<stdint.h>
#include<stdbool.h>
#include"bus.h"

#ifndef NES_APU2A03_H
#define NES_APU2A03_H

typedef struct _APU2A03 APU2A03;

APU2A03* allocAPU(struct Bus* bus, uint16_t start_address, uint16_t size);
void deallocAPU(APU2A03*);


#endif //NES_APU2A03_H