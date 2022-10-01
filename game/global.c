/* GBDK library */
#include <gb/gb.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <rand.h>

/* Global variables */
UINT8 state = 0;
int thunder_counter = 0;

/* General functions */
void cicled_delay(UINT8 cicles){
    UINT8 l;
    for(l = 0; l < cicles; l++) {
        wait_vbl_done();
    }
}

UINT8 generate_random_number(UINT8 upper) {
    int num = rand() & upper;
    return num;
}

void init_randomizer(){
	UWORD seed = DIV_REG;
	seed |= (UWORD)DIV_REG << 8;
	initarand(seed);
}

/* Sound functions */
void play_sound(UINT8 fx) {
    if (fx == 0) { // Hammer Pickup Sound        
        NR52_REG = 0xFFU;
        NR51_REG = 0xFF;
        NR50_REG = 0x77;
        NR10_REG = 0x32;
        NR11_REG = 0xc0;
        NR12_REG = 0x2f;
        NR13_REG = 0x6f;
        NR14_REG = 0x90;
    }
    if (fx == 1) { // Hazard Grave
        NR52_REG = 0xFFU;
        NR51_REG = 0xFF;
        NR50_REG = 0x55;
        NR10_REG = 4 | (1 << 3) | (1 << 4); 
        NR11_REG = 1 | (1 << 6); 
        NR12_REG = 1 | (0 << 3) | (15 << 4); 
        NR13_REG = 0xFF & 920; 
        NR14_REG = 0xC3; 
    }
    if (fx == 2) { // Thunder
        NR52_REG = 0xFFU;
        NR51_REG = 0xFF;
        NR50_REG = 0x11;
        NR41_REG = 0x1FU;//0x1FU;
        NR42_REG = 0xF7U;//0xF7U;
        NR43_REG = 0x24U;//0x24U;
        NR44_REG = 0xC3;//0x80U;
    }
}

/* Visual FX Functions */
void fadeout() {
    UINT8 i;
    for (i=0; i<4; i++) {
        if (i == 0) {
            BGP_REG = 0xE4; // Background palette
            OBP0_REG = 0xE4; // Sprite palette
        }
        if (i == 1) {
            BGP_REG = 0x90;
            OBP0_REG = 0x90;
        }
        if (i == 2) {
            BGP_REG = 0x80;
            OBP0_REG = 0x80;
        }
        if (i == 3) {
            BGP_REG = 0x00;
            OBP0_REG = 0x00;
        }
        cicled_delay(2);
    }
}

void fadein() {
    UINT8 i;
    for (i=0; i<4; i++) {
        if (i == 0) {
            OBP0_REG = 0x00; // Sprite palette
            BGP_REG = 0x00; // Background palette
        }
        if (i == 1) {
            OBP0_REG = 0x80;
            BGP_REG = 0x80;
        }
        if (i == 2) {
            OBP0_REG = 0x90;
            BGP_REG = 0x90;
        }
        if (i == 3) {
            OBP0_REG = 0xE4; 
            BGP_REG = 0xE4;
        }
        cicled_delay(2);
    }
}

void thunder() {
    thunder_counter+=1;
    if (thunder_counter >= 0 && thunder_counter < 2) {
        BGP_REG = 0xE4; 
        OBP0_REG = 0xE4; 
    }
    if (thunder_counter > 3 && thunder_counter < 5) {
        BGP_REG = 0x90;
        OBP0_REG = 0x90;
    }
    if (thunder_counter > 6 && thunder_counter < 8) {
        BGP_REG = 0x80;
        OBP0_REG = 0x80;
        play_sound(2);
    }
    if (thunder_counter > 9 && thunder_counter < 11) {
        BGP_REG = 0x00;
        OBP0_REG = 0x00;
    }
    if (thunder_counter > 12 && thunder_counter < 14) {
        OBP0_REG = 0x00;
        BGP_REG = 0x00; 
    }
    if (thunder_counter > 15 && thunder_counter < 17) {
        OBP0_REG = 0x80;
        BGP_REG = 0x80;
    }
    if (thunder_counter > 18 && thunder_counter < 20) {
        OBP0_REG = 0x90;
        BGP_REG = 0x90;
    }
    if (thunder_counter > 21) {
        OBP0_REG = 0xE4; 
        BGP_REG = 0xE4;
    }    
}