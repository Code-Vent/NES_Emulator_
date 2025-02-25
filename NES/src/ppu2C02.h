//
// Created by Code-Vent on 2/16/2025.

#include<stdint.h>
#include "bus.h"

#ifndef NES_PICTURE_PROCESSING_UNIT_H
#define NES_PICTURE_PROCESSING_UNIT_H


typedef union PPUCTRL {
    struct PPUCTRLBits{
        uint8_t name_table_base_addr : 2;
        uint8_t vram_incr : 1;
        uint8_t sprite_pattern_addr : 1;
        uint8_t background_pattern_addr : 1;
        uint8_t sprite_size : 1;
        uint8_t ppu_master : 1;
        uint8_t vblank_nmi_enable : 1;
    }bits;
    uint8_t byte;
}ppuctrl_t;

typedef union PPUMASK {
    struct PPUMASKBits {
        uint8_t grey_scale : 1;
        uint8_t show_leftmost_background : 1;
        uint8_t show_leftmost_sprite : 1;
        uint8_t enable_background_rendering : 1;
        uint8_t enable_sprite_rendering : 1;
        uint8_t emphasize_red : 1;
        uint8_t emphasize_green : 1;
        uint8_t emphasize_blue : 1;
    }bits;
    uint8_t byte;
}ppumask_t;

typedef union PPUSTATUS {
    struct PPUSTATUSBits {
        uint8_t ppu_identifier : 5;
        uint8_t sprite_overflow_flag : 1;
        uint8_t sprite0_hit_flag : 1;
        uint8_t vblank_flag : 1;
    }bits;
    uint8_t byte;
}ppustatus_t;

typedef union REG16 {
    uint16_t word;
    uint8_t  bytes[2];
}reg16_t;

struct PPURegisters {
    ppuctrl_t   ppuctrl;
    ppumask_t   ppumask;
    ppustatus_t ppustatus;
    uint8_t     oamaddr;
    uint8_t     oamdata;
    reg16_t     ppuscroll;
    reg16_t     ppuaddr;
    uint8_t     ppudata;
    uint8_t     oamdma;
};

typedef struct _PPU2C02 PPU2C02;

PPU2C02* allocPPU(struct Bus* main_bus, uint16_t start_address, uint16_t end_address, struct Bus* ppu_bus, struct Bus* g_bus);
void deallocPPU(PPU2C02*);
void getPPURegisters(struct PPURegisters*);




#endif //NES_PICTURE_PROCESSING_UNIT_H
