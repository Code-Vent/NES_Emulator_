//
// Created by Code-Vent on 2/16/2025.

#include<stdint.h>
#include "bus.h"

#ifndef NES_PICTURE_PROCESSING_UNIT_H
#define NES_PICTURE_PROCESSING_UNIT_H


union ControlSettings {
    struct PPUCTRLBits{
        uint8_t name_table_base_addr : 2;
        uint8_t vram_incr : 1;
        uint8_t sprite_pattern_addr : 1;
        uint8_t background_pattern_addr : 1;
        uint8_t sprite_size : 1;
        uint8_t ppu_master : 1;
        uint8_t vblank_nmi_enable : 1;
    }bits;
    uint8_t ppuctrl;
};

union RenderingSettings {
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
    uint8_t ppumask;
};

typedef struct _PPU2C02 PPU2C02;

PPU2C02* allocPPU(struct Bus* main_bus, struct Bus* g_bus, uint16_t start_address, uint16_t end_address);
void deallocPPU(PPU2C02*);
struct PPUCTRLBits* getControlSettings();
struct PPUMASKBits* getRenderingSettings();



#endif //NES_PICTURE_PROCESSING_UNIT_H
